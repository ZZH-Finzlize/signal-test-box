#pragma once
#include <QtCore>
#include <QString>
#include <QList>
#include <QListWidgetItem>
#include <cmath>
extern uint32_t allCalNum;
extern float fs;

class ASTExpress_t
{
private:

protected:
    static int recursionCount;
    static float* pListOfT;
public:
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
    typedef void (*calFunc_t)(QVector<float*>& pArgs, float* output);

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

        QVector<float*> pArgs;

        if (nullptr != this->args)
        {
            auto argLen = args->length();
            if (0 != argLen)
            {
                pArgs.resize(argLen);
                for (int i = 0;i < argLen;i++)
                {
                    pArgs[i] = new float[allCalNum];
                    this->args->at(i)->calculate(pArgs[i]);
                }
            }
        }

        this->Calcb(pArgs, output);

        for (auto pfArr : pArgs)
            delete pfArr;
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

extern ASTExpress_t* root;
extern QString textToParse;
