%{
	#include "Scanner.h"
	// used to keep track of location
	#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option nodefault yyclass="Scanner" noyywrap c++

%x					COMMENT_C

L					[a-zA-Z_]
D					[0-9]
ID					{L}({L}|{D})*
COMMENT_CPP			"//"[^\n]*$

%%

%{
	yylloc->step();
%}


{COMMENT_CPP}		{ ; }

"/*"				{ BEGIN(COMMENT_C); }
<COMMENT_C>"*/"		{ BEGIN(INITIAL); }
<COMMENT_C>.		{ ; }


{ID}         { *yylval = yytext; return Rakjin::Krystal::Parser::token::STRING; }

"["          { return Rakjin::Krystal::Parser::token::SECTION_START; }
"]"          { return Rakjin::Krystal::Parser::token::SECTION_END; }
"="          { return Rakjin::Krystal::Parser::token::ASSIGNMENT; }
.            { yylloc->step(); }
"\n"+        { yylloc->lines(yyleng); yylloc->step(); }

