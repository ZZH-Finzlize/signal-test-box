/*
@file: basic.cpp
@author: ZZH
@time: 2022-05-05 11:23:33
@info: 基本函数库实现文件
*/
#include "basic.h"

static float currentMax = 0;
static float currentMin = 0;

void reset(void)
{
    srand(time(NULL));
    currentMax = 0;
    currentMin = 0;
}

void __sin(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = sin(arg0[i]);
}

void __cos(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = sin(arg0[i]);
}

void __rand(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    for (int i = 0;i < allCalNum;i++)
        output[i] = rand();
}

void __inner_max(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = __max(arg0[i], currentMax);
}//8 16 32 64 128 256 512 1024 2048 4096

void __inner_min(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = __min(arg0[i], currentMax);
}

//使用CPU的rdrand指令产生硬件随机数
void hrand(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    for (int i = 0;i < allCalNum;i++)
    {
        asm volatile("rdrand %0":"=r"(*output));
    }
}

void __abs(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = fabs(arg0[i]);
}

void freq(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    
    for (int i = 0;i < allCalNum;i++)
        output[i] = 2 * 3.1415926535 * arg0[i] * pArgs->t[i];
}

void length(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    const int halfCalNum = allCalNum / 2;

    for (int i = 0, j = 0;i < halfCalNum;i++, j += 2)
    {
        output[i] = sqrtf(powf(arg0[j], 2) + powf(arg0[j + 1], 2));
        output[i + halfCalNum] = 0;
    }
}

void angle(pFunCallArg_t pArgs, float* output)
{

}

