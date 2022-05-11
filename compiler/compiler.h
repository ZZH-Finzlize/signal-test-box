/*
@file: compiler.h
@author: ZZH
@time: 2022-05-05 17:15:49
@info: 编译组件
*/
#include <QList>
#include "compile_common.h"
#include "ast.h"
#include "log.h"
#include "SignalItem.h"

class Compiler_t
{
private:
    ASTExpress_t* root;
    QString textToParse;
    int recursionCount;
    const int maxRecLim;

    QList<SignalItem*> compile_queue;//编译队列

    Compiler_t(int recLim = 15) : maxRecLim(recLim) { COMP_INFO("Init"); }

    ~Compiler_t()
    {
        // if (nullptr != this->root)
        //     delete this->root;

        COMP_INFO("Destroy");
    }

    SignalItem* queuePopFirst(void)
    {
        SignalItem* item = this->compile_queue.first();
        this->compile_queue.pop_front();
        return item;
    }

protected:

    ASTExpress_t* compile(const QString& code);

public:

    friend class SignalItem;

    bool recPush(void)
    {
        if (this->recursionCount < this->maxRecLim)
        {
            this->recursionCount++;
            return true;
        }

        return false;
    }

    bool isRecOutOfLim(void) { return this->recursionCount >= this->maxRecLim; }

    bool recPop(void)
    {
        if (this->recursionCount > 0)
        {
            this->recursionCount--;
            return true;
        }

        return false;
    }

    static Compiler_t& getInst() { static Compiler_t inst(32); return inst; }

    ASTExpress_t* getASTRoot(void) const { return this->root; }
    void setASTRoot(ASTExpress_t* const newValue = nullptr) { this->root = newValue; }

    QString& getParseText(void) { return this->textToParse; }

    bool compile(SignalItem* pSignal);

    void queuePush(SignalItem* pSignal) { this->compile_queue.push_back(pSignal); }
    bool queueIsEmpty(void) const { return this->compile_queue.isEmpty(); }
};

