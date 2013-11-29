%{
	#include "Scanner.h"
	// used to keep track of location
	#define YY_USER_ACTION yylloc->columns(yyleng);
%}

%option nodefault yyclass="Scanner" noyywrap c++

%x					COMMENT_C

NL					[\n\r]+
WS					[ \t]+
L					[a-zA-Z_]
D					[0-9]
ID					{L}({L}|{D})*
COMMENT_CPP			"//"[^\n]*$

%%

%{
	yylloc->step();
%}


{NL}				{ yylloc->lines(yyleng); yylloc->step(); }
{WS}				{ yylloc->step(); }

{COMMENT_CPP}		{ yylloc->step(); }

"/*"				{ BEGIN(COMMENT_C); }
<COMMENT_C>"*/"		{ BEGIN(INITIAL); }
<COMMENT_C>{NL}		{ yylloc->lines(yyleng); yylloc->step(); }
<COMMENT_C>.		{ yylloc->step(); }


"{"					{ return Krystal::Parser::token::BLOCK_BEGIN; }
"}"					{ return Krystal::Parser::token::BLOCK_END; }
"<"					{ return Krystal::Parser::token::LT; }
">"					{ return Krystal::Parser::token::GT; }
";"					{ return Krystal::Parser::token::SEMICOLON; }
","					{ return Krystal::Parser::token::COMMA; }

\"(\\.|[^\\"])*\"	{
						yylval->string = new std::string(yytext + 1, yyleng - 2); // +1/-2 to remove quotes
						//std::cout << *(yylval->string);
						return Krystal::Parser::token::STRING_LITERAL;
					}

"packet"			{ return Krystal::Parser::token::PACKET; }
"#include"			{ return Krystal::Parser::token::INCLUDE; }


  /* * * * * * * * * * * * * * * * * * * * * * */
  /* if you change this primitive type rules,  */
  /* you should also edit corresponding lists  */
  /* inside of template.*.h                    */
"bool"		|
"byte"		|
"sbyte"		|
"short"		|
"ushort"	|
"int"		|
"uint"		|
"long"		|
"ulong"		|
"float"		|
"double"	|
"string"			{
						yylval->string = new std::string(yytext, yyleng);
						return Krystal::Parser::token::PRIMITIVE_DATA_TYPE;
					}

"map"				{
						yylval->string = new std::string(yytext, yyleng);
						return Krystal::Parser::token::MAP;
					}

"list"				{
						yylval->string = new std::string(yytext, yyleng);
						return Krystal::Parser::token::LIST;
					}

{ID}				{
						yylval->string = new std::string(yytext, yyleng);
						return Krystal::Parser::token::ID;
					}

.					{
						yylval->string = new std::string(yytext, yyleng);
						return Krystal::Parser::token::UNKNOWN_CHARACTER;
					}

