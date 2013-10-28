%option noyywrap

%{
#include "krystal.tab.h"
%}

WS			[ \t]+
NL			\n
NR			\r
L			[a-zA-Z_]
D			[0-9]
ID			{L}({L}|{D})*





%%

{WS}		{ ; }
{NL}		{ ; }
{NR}		{ ; }
{ID}		{
				return ID;
			}
