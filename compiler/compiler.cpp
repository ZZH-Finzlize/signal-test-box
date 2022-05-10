/*
@file: compiler.cpp
@author: ZZH
@time: 2022-05-05 17:16:01
@info: 编译组件
*/
#include <QMessageBox>
#include "compiler.h"

Compiler_t Compiler_t::inst(32);

bool Compiler_t::compile(SignalItem* pSignal)
{
    this->recursionCount = 0;
    this->queuePush(pSignal);

    while (not this->queueIsEmpty())//队列会在编译时动态添加新的元素
    {
        if (true == this->recPush())//嵌套计数增加
        {
            SignalItem* pCurrentSig = this->queuePopFirst();//取出首项

            if (false == pCurrentSig->compile())//编译首项
                return false;

            this->recPop();//嵌套计数减少
        }
        else
        {
            QMessageBox::critical(nullptr, QString::fromUtf8("递归终止"), QString::fromUtf8("变量嵌套层数超过最大限制,最大值为15,请检查信号表达式是否存在循环引用或使用了太多嵌套"));
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
