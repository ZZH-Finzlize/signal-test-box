#pragma once
#include <QVector>
extern uint32_t allCalNum;
extern bool fftIsCalled;

void __sin(float** pArgs, float* output);
void __cos(float** pArgs, float* output);
void __rand(float** pArgs, float* output);
void __hrand(float** pArgs, float* output);
void __inner_max(float** pArgs, float* output);
void __inner_min(float** pArgs, float* output);
void hrand(float** pArgs, float* output);
void __abs(float** pArgs, float* output);
void __freq(float** pArgs, float* output);
void __fft(float** pArgs, float* output);
void __length(float** pArgs, float* output);
void __angle(float** pArgs, float* output);
void resetInnerFun(void);
