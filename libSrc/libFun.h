#pragma once
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define EXPORT extern "C"

typedef struct
{
    unsigned int allCalNum;
    float* t;
    float** args;
}FunCallArg_t, * pFunCallArg_t;
