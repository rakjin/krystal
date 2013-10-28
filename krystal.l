%option noyywrap

%{
#include "krystal.tab.h"
%}

NL					[\n\r]+
WS					[ \t\n\r]+
L					[a-zA-Z_]
D					[0-9]
ID					{L}({L}|{D})*
NUM					{D}+(\.{D}+)?(E[+\-]?{D}+)?
STRING_LITERAL		\"(\\.|[^\\"])*\"

%%

{WS}				{ ; }

"{"					{ return BLOCK_BEGIN; }
"}"					{ return BLOCK_END; }

{STRING_LITERAL}	{ return STRING_LITERAL; }

"packet"			{ return PACKET; }
"#include"			{ return INCLUDE; }

{ID}				{ return ID; }

