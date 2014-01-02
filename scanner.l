%{
	#include "Scanner.h"
	// used to keep track of location
	#define YY_USER_ACTION yylloc->columns(yyleng);

	using namespace std;
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
						yylval->stringValue = new string(yytext + 1, yyleng - 2); // +1/-2 to remove quotes
						//cout << *(yylval->stringValue);
						return Krystal::Parser::token::STRING_LITERAL;
					}

"\[uri:"({ID}|"/")+"\]"	{
							yylval->stringValue = new string(yytext + 5, yyleng - 6);
							return Krystal::Parser::token::URI;
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
						yylval->stringValue = new string(yytext, yyleng);
						return Krystal::Parser::token::PRIMITIVE_DATA_TYPE;
					}

"map"				{
						yylval->stringValue = new string(yytext, yyleng);
						return Krystal::Parser::token::MAP;
					}

"list"				{
						yylval->stringValue = new string(yytext, yyleng);
						return Krystal::Parser::token::LIST;
					}

"stream"			{
						yylval->stringValue = new string(yytext, yyleng);
						return Krystal::Parser::token::STREAM;
					}

{ID}				{
						yylval->stringValue = new string(yytext, yyleng);
						return Krystal::Parser::token::ID;
					}

.					{
						yylval->stringValue = new string(yytext, yyleng);
						return Krystal::Parser::token::UNKNOWN_CHARACTER;
					}

