#include "ast.h"
#include "symTable.h"
#include <QMessageBox>
#include "compile_common.h"
#include <string>

ASTExpress_t* root;
uint32_t allCalNum;
float fs = 0.01;
QString textToParse;

int ASTExpress_t::recursionCount = 0;
float* ASTExpress_t::pListOfT = nullptr;

bool ASTSignal_t::compile(void)
{
    bool res = false;
    if (this->getRecCounter() < 15)
    {
        this->recursionCount++;
        ASTExpress_t* savedRoot = root;
        QListWidgetItem* item;

        if (SigSymTable.search(this->sigName, item))
        {
            textToParse = item->data(Qt::UserRole + 2).toString();
            if (not textToParse.isEmpty())
            {
                resetParser();
                yyparse();
                if (yyerrorCount == 0 || nullptr != root)
                {
                    this->subRoot = root;
                    if (root->compile())
                    {
                        res = true;
                    }
                }
            }
        }

        root = savedRoot;
    }
    else
    {
        QMessageBox::critical(nullptr, QString::fromUtf8("递归终止"), QString::fromUtf8("变量嵌套层数超过最大限制,最大值为15,请检查信号表达式是否存在循环引用或使用了太多嵌套"));
    }

    return res;
}
