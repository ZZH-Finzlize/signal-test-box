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
    unsigned int totolPoint;
    float fs;
protected:

public:
    Calculator_t()
    {
        this->reset();
    }

    ~Calculator_t() {}

    void reset(void);

    unsigned int getTotolPoint(void) const;
    void setTotolPoint(const unsigned int newValue);

    float getFS(void) const;
    void setFS(const float newValue);
};
