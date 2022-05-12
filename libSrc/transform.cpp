/*
@file: transform.cpp
@author: ZZH
@time: 2022-05-05 11:22:55
@info: 变换函数实现文件
*/
#include "transform.h"
#include "fftw3.h"

void fft(pFunCallArg_t pArgs, float* output)
{
    int allCalNum = pArgs->allCalNum;
    float* arg0 = pArgs->args[0];
    const int halfCalNum = allCalNum / 2;

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
