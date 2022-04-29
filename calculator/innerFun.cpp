#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <QLibrary>

#include "fftw3.h"
#include "innerFun.h"
#include "ast.h"

static float currentMax = 0;
static float currentMin = 0;

bool fftIsCalled = false;//如果fft函数被调用过,则置位此标志位,通知ui进入频谱模式

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
}//8 16 32 64 128 256 512 1024 2048 4096

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

void __freq(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    for (int i = 0;i < allCalNum;i++)
        output[i] = 2 * 3.1415926535 * arg0[i] * ASTExpress_t::pListOfT[i];
}

void __fft(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    const int halfCalNum = allCalNum / 2;
    fftIsCalled = true;

    fftwf_complex* r = (fftwf_complex*) fftwf_malloc(allCalNum * sizeof(fftwf_complex));
    fftwf_plan p = fftwf_plan_dft_r2c_1d(allCalNum, arg0, r, FFTW_ESTIMATE);

    fftwf_execute(p);
    fftwf_destroy_plan(p);
    
    for (int i = 0, j = 0;i < halfCalNum;i++, j += 2)
    {
        output[j] = r[i][0];
        output[j + 1] = r[i][1];
    }

    fftwf_free(r);
}

void __length(QVector<float*>& pArgs, float* output)
{
    float* arg0 = pArgs[0];
    const int halfCalNum = allCalNum / 2;

    for (int i = 0, j = 0;i < halfCalNum;i++, j += 2)
    {
        output[i] = sqrtf(powf(arg0[j], 2) + powf(arg0[j + 1], 2));
        output[i + halfCalNum] = 0;
    }
}

void __angle(QVector<float*>& pArgs, float* output)
{

}
