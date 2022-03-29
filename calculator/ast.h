#pragma once
#include <QString>
#include <QList>
#include <cmath>

class ASTExpress_t
{
private:

protected://= 45 * sin(t / 12) + 15 + 32

public:
    ASTExpress_t() {}
    ~ASTExpress_t() {}

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

protected:
    typedef float (*calFunc_t)(const float*);
    QList<ASTExpress_t*> args;
public:
    ASTFunctionCall_t() {}
    ~ASTFunctionCall_t() {}

    virtual float calculate(void) const override;
};

class ASTNumber_t :public ASTExpress_t
{
private:

protected:
    float value;
public:
    ASTNumber_t() {}
    ~ASTNumber_t() {}

    virtual float calculate(void) const override { return this->value };
};

class ASTInput_t :public ASTExpress_t
{
private:

protected:

public:
    ASTInput_t() {}
    ~ASTInput_t() {}

    virtual float calculate(void) const override { return 0.1; }//这里应该返回的是当前计算计数值*采样周期后的结果
};

class ASTOperator_t :public ASTExpress_t
{
private:

protected:
    ASTExpress_t* left, * right;//左子式和右子式
    char op;//运算符

public:
    ASTOperator_t() {}
    ~ASTOperator_t() {}

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
