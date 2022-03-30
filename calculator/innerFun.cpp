#include <math.h>
#include "innerFun.h"
#include <stdlib.h>
#include <time.h>
static float currentMax = 0;
static float currentMin = 0;

void resetInnerFun(void)
{
    srand(time(NULL));
    currentMax = 0;
    currentMin = 0;
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

float __inner_max(const float* f)
{
    currentMax = __max(*f, currentMax);
    return currentMax;
}

float __inner_min(const float* f)
{
    currentMin = __min(*f, currentMin);
    return currentMin;
}

//使用CPU的rdrand指令产生硬件随机数
float hrand(const float* f)
{
    size_t res;
    asm volatile("rdrand %0":"=r"(res));
    return res;
}
