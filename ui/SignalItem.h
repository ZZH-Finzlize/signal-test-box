/*
@file: SignalItem.h
@author: ZZH
@time: 2022-05-09 10:34:30
@info: 信号元素
*/
#pragma once
#include <QListWidgetItem>
#include "ast.h"
#include "log.h"

class SignalItem :public QListWidgetItem, public ASTExpress_t
{
private:

protected:
    QString sourceCode;//信号表达式
    ASTExpress_t* compileRes;//编译后的语法树节点
    bool dirty;

public:
    SignalItem(const QString& name) :QListWidgetItem(name), compileRes(nullptr), dirty(false) { UI_INFO("New item: %s", name.toStdString().c_str()); }
    virtual ~SignalItem()
    {
        if (nullptr != this->compileRes)
            delete this->compileRes;

        UI_INFO("Delete");
    }

    bool setSourceCode(const QString& code)
    {
        if (this->sourceCode != code)
        {
            this->sourceCode = code;
            this->dirty = true;
            UI_INFO("Update source code: %s", this->text().toStdString().c_str());
        }
    }
    QString getSourceCode(void) const { return this->sourceCode; }

    ASTExpress_t* getCompileRes() { return this->compileRes; }

    bool isDirty(void) const { return this->dirty; }

    bool compile(void);
    
    virtual void calculate(float* output) const override
    {
        if (nullptr != this->compileRes)
            this->compileRes->calculate(output);
    }
};
