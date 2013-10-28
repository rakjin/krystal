%option noyywrap

%{
#include "in2po.tab.h"
%}





%%

[ \t]+		{ ; }
[0-9]		{ yylval = yytext[0] - '0'; return DIGIT; }
[+\-\n]		{ return yytext[0]; }
