#pragma once
#include <QString>
#include <QList>
#include <cmath>
#include "calculator.h"
#include "innerFun.h"

class ASTExpress_t
{
private:

protected:

public:
    ASTExpress_t() {}
    virtual ~ASTExpress_t() {}

    virtual void calculate(float* output) const = 0;
    virtual bool isDirty(void) const { return false; }
};

class ASTAdaptor_t :public ASTExpress_t
{
private:

protected:
    ASTExpress_t* child;
public:
    ASTAdaptor_t(ASTExpress_t* pChild) :child(pChild) {}
    //adaptor在被析构时不释放子节点,因为它的作用就是自动断开释放链
    ~ASTAdaptor_t() {}

    virtual void calculate(float* output) const override { this->child->calculate(output); }
    virtual bool isDirty(void) const override { return this->child->isDirty(); }
};

class ASTFunctionCall_t : public ASTExpress_t
{
private:

public:
    typedef void (*calFunc_t)(pFunCallArg_t pArgs, float* output);

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

    virtual void calculate(float* output) const override;
    virtual bool isDirty(void) const override
    {
        if (nullptr != this->args)
        {
            for (auto exp : *this->args)
            {
                if (exp->isDirty())
                    return true;
            }
        }

        return false;
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
            int allNum = Calculator_t::getInst().getTotolPoint();
            for (int i = 0;i < allNum;i++)
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
            auto& calculator = Calculator_t::getInst();
            int allNum = calculator.getTotolPoint();
            for (int i = 0;i < allNum;i++)
                output[i] = calculator.getT(i);
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

    virtual void calculate(float* output) const override;
    virtual bool isDirty(void) const override
    {
        bool res = false;

        if (nullptr != this->left)
            res |= this->left->isDirty();

        if (nullptr != this->right)
            res |= this->right->isDirty();

        return res;
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

    virtual void calculate(float* output) const override;
    virtual bool isDirty(void) const override
    {
        bool res = false;

        if (nullptr != this->left)
            res |= this->left->isDirty();

        if (nullptr != this->right)
            res |= this->right->isDirty();
        
        if (nullptr != this->cond)
            res |= this->cond->isDirty();

        return res;
    }
};
