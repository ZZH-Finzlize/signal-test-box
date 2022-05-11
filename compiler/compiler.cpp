/*
@file: compiler.cpp
@author: ZZH
@time: 2022-05-05 17:16:01
@info: 编译组件
*/
#include <QMessageBox>
#include "compiler.h"

bool Compiler_t::compile(SignalItem* pSignal)
{
    this->recursionCount = 0;
    this->queuePush(pSignal);

    while (not this->queueIsEmpty())//队列会在编译时动态添加新的元素
    {
        SignalItem* pCurrentSig = this->queuePopFirst();//取出首项

        try
        {
            if (false == pCurrentSig->compile())//编译首项
                return false;
        }
        catch (const QString& str)
        {
            QMessageBox::critical(nullptr, QString::fromUtf8("编译错误"), str);
            return false;
        }
    }

    return true;
}

ASTExpress_t* Compiler_t::compile(const QString& code)
{
    this->textToParse = code;
    this->root = nullptr;
    resetParser();
    yyparse();

    if (nullptr == this->root or 0 != yyerrorCount)
        return nullptr;

    return this->root;
}
