%{
	#include "Scanner.h"
	// used to keep track of location
	#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option nodefault yyclass="Scanner" noyywrap c++

%%

%{
	yylloc->step();
%}

[^\[\]\n= \t][^\[\]\n=]+[^\[\]\n= \t]  { *yylval = yytext; return Rakjin::Krystal::Parser::token::STRING; }

"["          { return Rakjin::Krystal::Parser::token::SECTION_START; }
"]"          { return Rakjin::Krystal::Parser::token::SECTION_END; }
"="          { return Rakjin::Krystal::Parser::token::ASSIGNMENT; }
.            { yylloc->step(); }
"\n"+        { yylloc->lines(yyleng); yylloc->step(); }

