#include <math.h>
#include "innerFun.h"
#include <stdlib.h>
#include <time.h>
#include "fftw3.h"
static float currentMax = 0;
static float currentMin = 0;

void resetInnerFun(void)
{
    srand(time(NULL));
    currentMax = 0;
    currentMin = 0;
}

void __sin(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = sin(arg0[i]);
}

void __cos(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = sin(arg0[i]);
}

void __rand(QVector<float*>& pArgs, float* output)
{
    for (int i = 0;i < allCalNum;i++)
        output[i] = rand();
}

void __inner_max(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = __max(arg0[i], currentMax);
}

void __inner_min(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = __min(arg0[i], currentMax);
}

//使用CPU的rdrand指令产生硬件随机数
void hrand(QVector<float*>& pArgs, float* output)
{
    for (int i = 0;i < allCalNum;i++)
    {
        asm volatile("rdrand %0":"=r"(*output));
    }
}

void __abs(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = fabs(arg0[i]);
}

void __fft(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    fftwf_complex* r = (fftwf_complex*) fftwf_malloc(allCalNum * sizeof(fftwf_complex));
    fftwf_plan p = fftwf_plan_dft_r2c_1d(allCalNum, arg0, r, FFTW_ESTIMATE);

    
    fftwf_execute(p);

    
    fftwf_free(r);
    fftwf_destroy_plan(p);
}
