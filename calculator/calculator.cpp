/*
@file: calculator.cpp
@author: ZZH
@time: 2022-05-05 16:47:29
@info: 计算组件
*/
#include "calculator.h"

Calculator_t Calculator_t::inst;

void Calculator_t::allocArgs()
{
    if (nullptr != this->pListOfT)
        delete[] this->pListOfT;
    
    this->pListOfT = new float[this->totolPoint];

    for (int i = 0;i < this->totolPoint;i++)
        this->pListOfT[i] = i / this->fs;
}

void Calculator_t::cleanArgs(void)
{
    if (nullptr != this->pListOfT)
        delete[] this->pListOfT;

    this->pListOfT = nullptr;
}

bool Calculator_t::calculate(ASTExpress_t* exp, float* pRes)
{
    this->allocArgs();//分配计算资源
    
    exp->calculate(pRes);//根节点递归计算语法树的值
    
    this->cleanArgs();//释放计算资源
    return true;
}
