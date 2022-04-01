#pragma once
#include <QVector>
extern uint32_t allCalNum;

void __sin(QVector<float*>& pArgs, float* output);
void __cos(QVector<float*>& pArgs, float* output);
void __rand(QVector<float*>& pArgs, float* output);
void __hrand(QVector<float*>& pArgs, float* output);
void __inner_max(QVector<float*>& pArgs, float* output);
void __inner_min(QVector<float*>& pArgs, float* output);
void hrand(QVector<float*>& pArgs, float* output);
void __abs(QVector<float*>& pArgs, float* output);
void __fft(QVector<float*>& pArgs, float* output);
void resetInnerFun(void);
