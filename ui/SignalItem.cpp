/*
@file: SignalItem.cpp
@author: ZZH
@time: 2022-05-09 10:34:07
@info: 信号元素
*/
#include "SignalItem.h"
#include "compiler.h"

bool SignalItem::compile(void)
{
    if (true == this->isDirty())
    {
        this->compileRes = Compiler_t::getInst().compile(this->sourceCode);

        if (nullptr == this->compileRes)
            return false;

        this->dirty = false;
    }

    return true;
}
