#include "ast.h"
#include "symTable.h"
#include "grammar.tab.hpp"
ASTExpress_t* root;
uint32_t calPoint;
float fs = 0.01;
QString textToParse;

bool ASTSignal_t::compile(void)
{
    // int savedLineno = yylineno;
    // int savederrno = yyerrorCount;
    ASTExpress_t* savedRoot = root;
    bool res = false;
    QListWidgetItem* item;

    res = SigSymTable.search(this->sigName, item);
    if (res)
    {
        textToParse = item->data(Qt::UserRole + 2).toString();
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

    // yylineno = savedLineno;
    // yyerrorCount = savederrno;
    root = savedRoot;

    return res;
}
