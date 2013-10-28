%option noyywrap

%{
#include "krystal.tab.h"
%}

%x					COMMENT_C

NL					[\n\r]+
WS					[ \t]+
L					[a-zA-Z_]
D					[0-9]
ID					{L}({L}|{D})*
NUM					{D}+(\.{D}+)?(E[+\-]?{D}+)?
COMMENT_CPP			"//"[^\n]*$

%%

{NL}				{ return NL; }
{WS}				{ ; }

{COMMENT_CPP}		{ ; }
"/*"				{ BEGIN(COMMENT_C); }
<COMMENT_C>"*/"		{ BEGIN(INITIAL); }
<COMMENT_C>.		{ ; }

"{"					{ return BLOCK_BEGIN; }
"}"					{ return BLOCK_END; }

\"(\\.|[^\\"])*\"	{
						yylval.string = strdup(yytext);
						return STRING_LITERAL;
					}

"packet"			{ return PACKET; }
"#include"			{ return INCLUDE; }

{ID}				{
						yylval.string = strdup(yytext);
						return ID;
					}

