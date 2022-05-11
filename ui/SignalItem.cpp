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
        COMP_INFO("Compile dirty signal %s", this->text().toStdString().c_str());

        if (nullptr != this->compileRes)
            delete this->compileRes;

        this->compileRes = Compiler_t::getInst().compile(this->sourceCode);

        if (nullptr == this->compileRes)
            return false;

        this->dirty = false;
    }

    COMP_INFO("Signal %s is clean", this->text().toStdString().c_str());
    return true;
}
