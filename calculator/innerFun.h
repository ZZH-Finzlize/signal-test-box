#pragma once

typedef struct
{
    unsigned int allCalNum;
    float* t;
    float** args;
}FunCallArg_t, * pFunCallArg_t;



extern bool fftIsCalled;

void __sin(pFunCallArg_t pArgs, float* output);
void __cos(pFunCallArg_t pArgs, float* output);
void __rand(pFunCallArg_t pArgs, float* output);
void __hrand(pFunCallArg_t pArgs, float* output);
void __inner_max(pFunCallArg_t pArgs, float* output);
void __inner_min(pFunCallArg_t pArgs, float* output);
void hrand(pFunCallArg_t pArgs, float* output);
void __abs(pFunCallArg_t pArgs, float* output);
void __freq(pFunCallArg_t pArgs, float* output);
void __fft(pFunCallArg_t pArgs, float* output);
void __length(pFunCallArg_t pArgs, float* output);
void __angle(pFunCallArg_t pArgs, float* output);
void resetInnerFun(void);
