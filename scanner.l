/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */
 
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

[^\[\]\n= \t][^\[\]\n=]+[^\[\]\n= \t]  { *yylval = yytext; return Waffleshop::IniLoader::Parser::token::STRING; }

"["          { return Waffleshop::IniLoader::Parser::token::SECTION_START; }
"]"          { return Waffleshop::IniLoader::Parser::token::SECTION_END; }
"="          { return Waffleshop::IniLoader::Parser::token::ASSIGNMENT; }
.            { yylloc->step(); }
"\n"+        { yylloc->lines(yyleng); yylloc->step(); }

