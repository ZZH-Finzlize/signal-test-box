#include "symTable.h"
#include "innerFun.h"

SigSymTable_t SigSymTable;
FunSymTable_t FunSymTable;


static void __attribute__((constructor)) __init()
{
    // QList<int> aa;
    // aa.append(6);
    FunSymTable.insert("sin", __sin);
    FunSymTable.insert("cos", __cos);
    FunSymTable.insert("rand", __rand);
    FunSymTable.insert("hrand", hrand);
    FunSymTable.insert("min", __inner_min);
    FunSymTable.insert("max", __inner_max);
    // FunSymTable.insert("cos", __cos);
}
