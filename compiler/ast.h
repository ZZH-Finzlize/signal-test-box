#pragma once
#include <QString>
#include <QList>
#include <cmath>
extern uint32_t allCalNum;
extern float fs;

class ASTExpress_t
{
private:

protected:
    static int recursionCount;

public:
    static float* pListOfT;

    ASTExpress_t() {}
    virtual ~ASTExpress_t() {}
    static inline void resetRecCounter(int initVal = 0) { recursionCount = initVal; }
    static inline int getRecCounter() { return recursionCount; }

    static void preCalculateT()
    {
        pListOfT = new float[allCalNum];
        for (int i = 0;i < allCalNum;i++)
            pListOfT[i] = i / fs;
    }

    static void cleanPreCalT()
    {
        if (pListOfT)
            delete[] pListOfT;
        pListOfT = nullptr;
    }

    virtual void calculate(float* output) const = 0;
    virtual bool compile(void) { return true; }//用于动态编译引用的其他信号
};

class ASTSignal_t : public ASTExpress_t
{
private:
protected:
    QString sigName;
    ASTExpress_t* subRoot;
public:
    ASTSignal_t(char* name) :sigName(name), subRoot(nullptr) {}
    ~ASTSignal_t()
    {
        if (nullptr != this->subRoot)
            delete this->subRoot;
    }

    virtual void calculate(float* output) const override
    {
        if (nullptr != this->subRoot)
        {
            this->subRoot->calculate(output);
        }
    }

    virtual bool compile(void) override;
};

class ASTFunctionCall_t : public ASTExpress_t
{
private:

public:
    typedef void (*calFunc_t)(float** pArgs, float* output);

protected:
    calFunc_t Calcb;
    QList<ASTExpress_t*>* args;
public:
    ASTFunctionCall_t(calFunc_t fun, QList<ASTExpress_t*>* args) :Calcb(fun), args(args) {}
    virtual ~ASTFunctionCall_t()
    {
        if (this->args)
        {
            for (auto exp : *args)
                delete exp;

            delete this->args;
        }
    }

    virtual void calculate(float* output) const override
    {
        if (nullptr == output)
            return;

        float** pArgs = nullptr;
        int argLen = 0;

        if (nullptr != this->args)
        {
            argLen = args->length();
            pArgs = new float* [argLen];

            if (0 != argLen)
            {
                for (int i = 0;i < argLen;i++)
                {
                    pArgs[i] = new float[allCalNum];
                    this->args->at(i)->calculate(pArgs[i]);
                }
            }
        }

        this->Calcb(pArgs, output);

        for (int i = 0;i < argLen;i++)
            delete pArgs[i];

        if (nullptr != pArgs)
            delete[] pArgs;
    }

    virtual bool compile(void) override
    {
        bool res = true;
        if (nullptr != this->args)
        {
            for (auto exp : *args)
            {
                res &= exp->compile();
            }
        }

        return res;
    }
};

class ASTNumber_t :public ASTExpress_t
{
private:

protected:
    float value;
public:
    ASTNumber_t(float f) :value(f) {}
    virtual ~ASTNumber_t() {}

    virtual void calculate(float* output) const override
    {
        if (nullptr != output)
        {
            for (int i = 0;i < allCalNum;i++)
                output[i] = this->value;
        }
    }
};

class ASTTime_t :public ASTExpress_t
{
private:

protected:

public:
    ASTTime_t() {}
    virtual ~ASTTime_t() {}

    virtual void calculate(float* output) const override
    {
        if (nullptr != output)
        {
            for (int i = 0;i < allCalNum;i++)
                output[i] = this->pListOfT[i];
        }
    }
};

class ASTOperator_t :public ASTExpress_t
{
private:

protected:
    char op;//运算符
    ASTExpress_t* left, * right;//左子式和右子式

public:
    ASTOperator_t(char op, ASTExpress_t* l, ASTExpress_t* r) :op(op), left(l), right(r) {}
    virtual ~ASTOperator_t()
    {
        if (nullptr != this->left)
            delete this->left;

        if (nullptr != this->right)
            delete this->right;
    }

    virtual void calculate(float* output) const override
    {
        if (nullptr == output)
            return;

        float* lVal = new float[allCalNum];
        this->left->calculate(lVal);

        float* rVal = new float[allCalNum];
        this->right->calculate(rVal);

        switch (this->op)
        {
            case '+':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = lVal[i] + rVal[i];
            }
            break;

            case '-':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = lVal[i] - rVal[i];
            }
            break;

            case '*':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = lVal[i] * rVal[i];
            }
            break;

            case '/':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = lVal[i] / rVal[i];
            }
            break;

            case '%':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = std::fmod(lVal[i], rVal[i]);
            }
            break;

            case '^':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = powf(lVal[i], rVal[i]);
            }
            break;

            case '>':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = lVal[i] > rVal[i];
            }
            break;

            case '<':
            {
                for (int i = 0;i < allCalNum;i++)
                    output[i] = lVal[i] < rVal[i];
            }
            break;
        }

        delete[] lVal;
        delete[] rVal;
    }

    virtual bool compile(void) override
    {
        if (nullptr != this->left && nullptr != this->right)
        {
            return this->left->compile() && this->right->compile();
        }

        return false;
    }
};

class ASTCondition_t :public ASTExpress_t
{
private:

protected:
    ASTExpress_t* cond, * left, * right;
public:
    ASTCondition_t(ASTExpress_t* c, ASTExpress_t* l, ASTExpress_t* r) :cond(c), left(l), right(r) {}
    ~ASTCondition_t()
    {
        if (nullptr != this->cond)
            delete this->cond;

        if (nullptr != this->left)
            delete this->left;

        if (nullptr != this->right)
            delete this->right;
    }

    virtual void calculate(float* output) const override
    {
        if (nullptr != this->cond and nullptr != this->left and nullptr != this->right)
        {
            float* condRes = new float[allCalNum];
            float* leftRes = new float[allCalNum];
            float* rightRes = new float[allCalNum];

            this->cond->calculate(condRes);
            this->left->calculate(leftRes);
            this->right->calculate(rightRes);

            for (int i = 0;i < allCalNum;i++)
                output[i] = 0 != condRes[i] ? leftRes[i] : rightRes[i];

            delete[] condRes;
            delete[] leftRes;
            delete[] rightRes;
        }
        else
        {
            for (int i = 0;i < allCalNum;i++)
                output[i] = 0;
        }
    }

    virtual bool compile(void) override
    {
        if (nullptr != this->cond and nullptr != this->left and nullptr != this->right)
        {
            return this->cond->compile() && this->left->compile() && this->right->compile();
        }

        return false;
    }
};

extern ASTExpress_t* root;
extern QString textToParse;
