%option noyywrap

%{
#include "krystal.tab.h"
%}

WS			[ \t\n\r]+
L			[a-zA-Z_]
D			[0-9]
ID			{L}({L}|{D})*
NUM			{D}+(\.{D}+)?(E[+\-]?{D}+)?





%%

{WS}		{ ; }

"{"			{ return BLOCK_BEGIN; }
"}"			{ return BLOCK_END; }
"packet"	{ return PACKET; }

{ID}		{ return ID; }
