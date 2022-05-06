/*
@file: compiler.h
@author: ZZH
@time: 2022-05-05 17:15:49
@info: 编译组件
*/
#include "compile_common.h"
#include "ast.h"


class Compiler_t
{
private:
    ASTExpress_t* root;
    QString textToParse;
protected:

public:
    Compiler_t() {}
    ~Compiler_t()
    {
        if (nullptr != this->root)
            delete this->root;
    }

    ASTExpress_t* getASTRoot(void) const;
    void setASTRoot(const ASTExpress_t* const newValue = nullptr);
};
