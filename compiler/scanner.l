/*
@file: scanner.l
@author: ZZH
@time: 2022-03-29 21:03:35
@info: 
*/
%option nodefault yylineno noyywrap

%option stack
%pointer

%{
#include "grammar.tab.hpp"
#include <cstring>
#include "math.h"
#include "compiler.h"
#include "log.h"
using namespace std;
size_t yycolumn = 1;
void recordLocInfo(void)
{
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    yylloc.first_column = yycolumn;
    yylloc.last_column = yycolumn + yyleng - 1;
    yycolumn = yylloc.last_column + 1;
}

int user_input(char* buf, int max_size)
{
    auto& textToParse = Compiler_t::getInst().getParseText();
    int sizeToRead = __min(max_size, textToParse.length());
    if(0 != sizeToRead)
    {
        memcpy(buf, textToParse.toStdString().c_str(), sizeToRead);
    }
    textToParse.clear();
    return sizeToRead;
}

#define YY_USER_ACTION recordLocInfo();

#define YY_INPUT(buf,result,max_size) {result = user_input(buf, max_size);}
%}

NormalChar      [\+\-\*\/\(\)<>^%]
Number          [0-9]
HexNum          0x[0-9a-fA-F]+
Symbol          [a-zA-Z_][a-zA-Z0-9_]*
LineCommit      \/\/.*
IgnoreChar      [ \t\r]
%%
if                                      return tk_kwIf;
else                                    return tk_kwEles;

t                                       return tk_Time;
pi                                      {yylval.value = 3.1415926535f; return tk_Number;}

\>=                                     return tk_opGequ;
\<=                                     return tk_opLequ;
\!=                                     return tk_opNequ;

{NormalChar}                            return yytext[0];
{Number}+                               {yylval.value = atof(yytext); return tk_Number;}
-{Number}+                              {yylval.value = atof(yytext); return tk_Number;}
{Number}+\.{Number}+                    {yylval.value = atof(yytext); return tk_Number;}
{LineCommit}                            {}
{Symbol}                                {yylval.id = strdup(yytext); return tk_Symbol;}
{IgnoreChar}+                           {}
\n                                      {yycolumn = 1;}
#                                       return 0;
.                                       return yytext[0];
%%

/* int yywrap() 
{
    return 1;
} */
