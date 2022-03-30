#pragma once
#include <QtCore>
#include <QString>
#include <QList>
#include <cmath>
extern uint32_t calPoint;

class ASTExpress_t
{
private:

protected://= 45 * sin(t / 12) + 15 + 32

public:
    ASTExpress_t() {}
    virtual ~ASTExpress_t() {}

    virtual float calculate(void) const = 0;
};

// class ASTSignal_t : public ASTExpress_t
// {
// private:
// protected:
//     const char* sigName;
// public:
//     ASTSignal_t() {}
//     ~ASTSignal_t() {}

//     virtual float calculate(void) const override;
// };

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

    virtual float calculate(void) const override { return calPoint / 15.0; }
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
        delete this->left;
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
        }
    };
};

extern ASTExpress_t* root;
extern QString textToParse;
extern float fs;
