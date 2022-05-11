#include "ast.h"
#include "symTable.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include "compiler.h"
#include <string>

bool ASTAdaptor_t::isDirty(void) const
{
    auto& compiler = Compiler_t::getInst();
    bool res = false;

    if (true == compiler.recPush())
    {
        res = this->child->isDirty();
        compiler.recPop();
    }
    else
    {
        throw QString::fromUtf8("信号嵌套层数超过最大限制,最大值为15,请检查信号表达式是否存在循环引用或使用了太多嵌套");
    }
    return res;
}

void ASTFunctionCall_t::calculate(float* output) const
{
    if (nullptr == output)
        return;

    auto& calculator = Calculator_t::getInst();

    FunCallArg_t funArgs = {
        calculator.getTotolPoint(),
        calculator.getPT(),
        nullptr
    };

    int argLen = 0;

    if (nullptr != this->args)
    {
        argLen = args->length();
        funArgs.args = new float* [argLen];

        if (0 != argLen)
        {
            for (int i = 0;i < argLen;i++)
            {
                funArgs.args[i] = new float[funArgs.allCalNum];
                this->args->at(i)->calculate(funArgs.args[i]);
            }
        }
    }

    this->Calcb(&funArgs, output);

    for (int i = 0;i < argLen;i++)
        delete funArgs.args[i];

    if (nullptr != funArgs.args)
        delete[] funArgs.args;
}

void ASTOperator_t::calculate(float* output) const
{
    if (nullptr == output)
        return;

    int allNum = Calculator_t::getInst().getTotolPoint();

    float* lVal = new float[allNum];
    this->left->calculate(lVal);

    float* rVal = new float[allNum];
    this->right->calculate(rVal);

    switch (this->op)
    {
        case '+':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = lVal[i] + rVal[i];
        }
        break;

        case '-':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = lVal[i] - rVal[i];
        }
        break;

        case '*':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = lVal[i] * rVal[i];
        }
        break;

        case '/':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = lVal[i] / rVal[i];
        }
        break;

        case '%':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = std::fmod(lVal[i], rVal[i]);
        }
        break;

        case '^':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = powf(lVal[i], rVal[i]);
        }
        break;

        case '>':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = lVal[i] > rVal[i];
        }
        break;

        case '<':
        {
            for (int i = 0;i < allNum;i++)
                output[i] = lVal[i] < rVal[i];
        }
        break;
    }

    delete[] lVal;
    delete[] rVal;
}

void ASTCondition_t::calculate(float* output) const
{
    int allNum = Calculator_t::getInst().getTotolPoint();

    if (nullptr != this->cond and nullptr != this->left and nullptr != this->right)
    {
        float* condRes = new float[allNum];
        float* leftRes = new float[allNum];
        float* rightRes = new float[allNum];

        this->cond->calculate(condRes);
        this->left->calculate(leftRes);
        this->right->calculate(rightRes);

        for (int i = 0;i < allNum;i++)
            output[i] = 0 != condRes[i] ? leftRes[i] : rightRes[i];

        delete[] condRes;
        delete[] leftRes;
        delete[] rightRes;
    }
    else
    {
        for (int i = 0;i < allNum;i++)
            output[i] = 0;
    }
}
