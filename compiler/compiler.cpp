/*
@file: compiler.cpp
@author: ZZH
@time: 2022-05-05 17:16:01
@info: 编译组件
*/
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLibrary>
#include "compiler.h"
#include "symTable.h"

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

void Compiler_t::loadExtLibs(const QString& path)
{
    QDir dir(path);
    for (auto fn : dir.entryList(QStringList("*.json")))
    {
        QFile libDef("lib/" + fn);
        if (true == libDef.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            COMP_INFO("load lib: %s", fn.toStdString().c_str());
            QJsonObject json = QJsonDocument::fromJson(libDef.readAll()).object();
            QLibrary lib(QString("lib/lib%1").arg(fn.split('.')[0]));
            if (lib.load())
            {
                COMP_INFO("load lib succ");
                for (auto funcName : json.keys())
                {
                    auto value = json[funcName];
                    if (value.isObject())//另有函数名的模式
                    {
                        auto vObject = value.toObject();
                        auto symName = vObject["sym"].toString();
                        auto pf = lib.resolve(symName.toStdString().c_str());
                        if (nullptr != pf)
                        {
                            COMP_INFO("load fun %s", funcName.toStdString().c_str());
                            FunSymTable.insert(funcName, { reinterpret_cast<ASTFunctionCall_t::pf>(pf), (unsigned) vObject["argNum"].toInt() });
                        }
                    }
                    else if (value.isDouble())//数字模式,函数名和符号名一致
                    {
                        auto pf = lib.resolve(funcName.toStdString().c_str());
                        if (nullptr != pf)
                        {
                            COMP_INFO("load fun %s", funcName.toStdString().c_str());
                            FunSymTable.insert(funcName, { reinterpret_cast<ASTFunctionCall_t::pf>(pf), (unsigned) value.toInt() });
                        }
                    }
                }

                auto reset = lib.resolve("reset");//如果库导出了复位函数,则进行复位
                if (nullptr != reset)
                {
                    reset();
                }
            }
        }

        libDef.close();
    }
}
