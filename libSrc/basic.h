/*
@file: basic.h
@author: ZZH
@time: 2022-05-05 11:24:04
@info: 基本函数库头文件
*/
#include "libFun.h"

EXPORT void reset(void);

EXPORT void __sin(pFunCallArg_t pArgs, float* output);
EXPORT void __cos(pFunCallArg_t pArgs, float* output);
EXPORT void __rand(pFunCallArg_t pArgs, float* output);
EXPORT void hrand(pFunCallArg_t pArgs, float* output);
EXPORT void __inner_max(pFunCallArg_t pArgs, float* output);
EXPORT void __inner_min(pFunCallArg_t pArgs, float* output);
EXPORT void __abs(pFunCallArg_t pArgs, float* output);
EXPORT void freq(pFunCallArg_t pArgs, float* output);
EXPORT void length(pFunCallArg_t pArgs, float* output);
EXPORT void angle(pFunCallArg_t pArgs, float* output);
