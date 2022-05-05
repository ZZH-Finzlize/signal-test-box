/*
@file: calculator.h
@author: ZZH
@time: 2022-05-05 16:47:15
@info: 计算组件
*/
#include "ast.h"

class Calculator_t
{
private:
    ASTExpress_t* root;
    unsigned int totolPoint;
    float fs;
    QString textToParse;
protected:

public:
    Calculator_t()
    {
        this->reset();
    }

    ~Calculator_t()
    {
        if (nullptr != this->root)
            delete this->root;
    }

    void reset(void);

    ASTExpress_t* getASTRoot(void) const;
    void setASTRoot(const ASTExpress_t* const newValue = nullptr);

    unsigned int getTotolPoint(void) const;
    void setTotolPoint(const unsigned int newValue);

    float getFS(void) const;
    void setFS(const float newValue);
};
