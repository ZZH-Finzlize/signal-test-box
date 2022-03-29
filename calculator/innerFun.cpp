#include <math.h>
#include "innerFun.h"
#include <stdlib.h>
#include <time.h>

static void __attribute__((constructor)) __init()
{
    srand(time(NULL));
}

float __sin(const float* f)
{
    return sin(*f);
}

float __cos(const float* f)
{
    return cos(*f);
}

float __rand(const float* f)
{
    return rand();
}
