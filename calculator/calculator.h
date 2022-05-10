/*
@file: calculator.h
@author: ZZH
@time: 2022-05-05 16:47:15
@info: 计算组件
*/
#pragma once
#include "log.h"

class ASTExpress_t;

class Calculator_t
{
private:
    static Calculator_t *inst;

    unsigned int totolPoint;
    float fs;
    float* pListOfT;

    Calculator_t()
    {
        this->totolPoint = 0;
        this->fs = 0;
        this->pListOfT = nullptr;
        COMP_INFO("Init");
    }

    ~Calculator_t()
    {
        COMP_INFO("Destroy");
    }
protected:
    void allocArgs(void);
    void cleanArgs(void);
    float* getPT(void) { return this->pListOfT; }

public:

    friend class ASTFunctionCall_t;

    static Calculator_t& getInst(void)
    {
        if(nullptr == inst)
            inst = new Calculator_t;
        
        return *inst;
    }

    unsigned int getTotolPoint(void) const { return this->totolPoint; }
    void setTotolPoint(const unsigned int newValue) { this->totolPoint = newValue; }

    float getFS(void) const { return this->fs; }
    void setFS(const float newValue) { this->fs = newValue; }

    bool calculate(ASTExpress_t* exp, float* pRes);

    inline float getT(int index) { return this->pListOfT[index]; }
};
