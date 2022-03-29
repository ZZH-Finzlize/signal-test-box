/*
@file: scanner.l
@author: ZZH
@time: 2021-12-27 15:26:44
@info: edl编译器的词法分析器
*/
%option nodefault yylineno noyywrap

%option stack
%pointer

%{
#include "grammar.tab.hpp"
#include <cstring>
using namespace std;
static size_t yycolumn = 1;
void recordLocInfo(void)
{
    yylloc.first_line = yylineno;
    yylloc.last_line = yylineno;
    yylloc.first_column = yycolumn;
    yylloc.last_column = yycolumn + yyleng - 1;
    yycolumn = yylloc.last_column + 1;
}

#define YY_USER_ACTION recordLocInfo();
%}

NormalChar      [\+\-\*\/\(\)={}<>!&\|^%]
Spacer          [;]
Number          [0-9]
HexNum          0x[0-9a-fA-F]+
Symbol          [a-zA-Z_]*[a-zA-Z0-9]+
LineCommit      \/\/.*
IgnoreChar      [ \t\r]
%%
function                                return tk_kwFunction;

bool                                    {yylval.dataType = dataTypes::Bool; return tk_kwType;}
Byte                                    {yylval.dataType = dataTypes::Byte; return tk_kwType;}
int                                     {yylval.dataType = dataTypes::Int; return tk_kwType;}
float                                   {yylval.dataType = dataTypes::Float; return tk_kwType;}
string                                  {yylval.dataType = dataTypes::String; return tk_kwType;}
none                                    {yylval.dataType = dataTypes::None; return tk_kwType;}

if                                      return tk_kwIf;
else                                    return tk_kwEles;
switch                                  return tk_kwSwitch;

for                                     return tk_kwFor;
while                                   return tk_kwWhile;
do                                      return tk_kwDo;
until                                   return tk_kwUntil;
repeat                                  return tk_kwRepeat;

return                                  return tk_kwReturn;
break                                   return tk_kwBreak;
continue                                return tk_kwContinue;
breakall                                return tk_kwBreakAll;

case                                    return tk_kwCase;

request                                 return tk_kwRequest;

import                                  return tk_kwImport;

not                                     return '!';
mod                                     return '%';
xor                                     return '^';
and                                     return '&';
or                                      return '|';

==                                      return tk_opEuq;
\>=                                     return tk_opGequ;
\<=                                     return tk_opLequ;
\!=                                     return tk_opNequ;

{NormalChar}                            return yytext[0];
{Number}+                               {yylval.value = atoi(yytext); return tk_Number;}
-{Number}+                              {yylval.value = atoi(yytext); return tk_Number;}
{Number}+\.{Number}+                    {yylval.fvalue = atof(yytext); return tk_Float;}
{HexNum}                                {sscanf(yytext, "%x", &yylval.value); return tk_Number;}
{LineCommit}                            {}
{Symbol}                                {yylval.id = strdup(yytext); return tk_Symbol;}
{Spacer}+                               {return spacer;}
{IgnoreChar}+                           {}
\n                                      {yycolumn = 1;}
#                                       return 0;
.                                       return yytext[0];
%%

/* int yywrap() 
{
    return 1;
} */
