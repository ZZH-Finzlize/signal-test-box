#pragma once
#include <QtCore>
#include <QString>
#include <QList>
#include <QListWidgetItem>
#include <cmath>
extern uint32_t calPoint;
extern float fs;

class ASTExpress_t
{
private:
    
protected:
    static int recursionCount;
    
public:
    ASTExpress_t() {}
    virtual ~ASTExpress_t() {}
    static inline void resetRecCounter(int initVal = 0) { recursionCount = initVal; }
    static inline int getRecCounter() { return recursionCount; }

    virtual float calculate(void) const = 0;
    virtual bool compile(void) { return true; }//用于动态编译引用的其他信号
};

class ASTSignal_t : public ASTExpress_t
{
private:
protected:
    QString sigName;
    ASTExpress_t* subRoot;
public:
    ASTSignal_t(char* name):sigName(name), subRoot(nullptr) {}
    ~ASTSignal_t()
    {
        if(nullptr != this->subRoot)
            delete this->subRoot;
    }

    virtual float calculate(void) const override
    {
        if (nullptr != this->subRoot)
        {
            return this->subRoot->calculate();
        }
        return 0;
    }
    
    virtual bool compile(void) override;
};

class ASTFunctionCall_t : public ASTExpress_t
{
private:

public:
    typedef float (*calFunc_t)(const float*);

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

    virtual float calculate(void) const override
    {
        if (nullptr != this->args)
        {
            auto argLen = args->length();
            if (0 != argLen)
            {
                float* pArgs = new float[argLen];
                for (int i = 0;i < argLen;i++)
                    pArgs[i] = args->at(i)->calculate();

                return Calcb(pArgs);
            }
        }

        return Calcb(nullptr);
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

    virtual float calculate(void) const override { return this->value; }
};

class ASTTime_t :public ASTExpress_t
{
private:

protected:

public:
    ASTTime_t() {}
    virtual ~ASTTime_t() {}

    virtual float calculate(void) const override { return calPoint / fs; }
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

    virtual float calculate(void) const override
    {
        float lVal = this->left->calculate();
        float rVal = this->right->calculate();
        switch (this->op)
        {
            case '+':
                return lVal + rVal;

            case '-':
                return lVal - rVal;

            case '*':
                return lVal * rVal;

            case '/':
                return lVal / rVal;

            case '%':
                return std::fmod(lVal, rVal);

            case '^':
                return powf(lVal, rVal);

            case '>':
                return lVal > rVal ? 1 : 0;

            case '<':
                return lVal < rVal ? 1 : 0;
        }
    }

    virtual bool compile(void) override
    {
        if(nullptr != this->left && nullptr != this->right)
        {
            return this->left->compile() && this->right->compile();
        }
        
        return false;
    }
};

extern ASTExpress_t* root;
extern QString textToParse;
