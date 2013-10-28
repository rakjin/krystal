%{
#include <stdio.h>
#include <ctype.h>

int yyerror(char* s);

%}


%union
{
	char* string;
}

%token NL
%token<string> ID
%token<string>	STRING_LITERAL

%token INCLUDE
%token PACKET
%token BLOCK_BEGIN BLOCK_END






%%

program				:
					|	commands
					;

commands			:	command
					|	commands command
					;

command				:	packet
					|	include
					|	unknown_command
					|	NL
					;

packet				:	PACKET BLOCK_BEGIN BLOCK_END
						{
							printf("packet declaration\n");
						}
					;

include				:	INCLUDE STRING_LITERAL NL
						{
							printf("include directive: %s\n", $2);
						}

unknown_command		:	ID
						{
							printf("unknown command: %s\n", $1);
						}
					;




%%

int yyerror(char* s)
{
	printf("%s\n", s);
}

int main()
{
	if (yyparse() == 0) printf("Parsing OK!\n\n");
	else printf("Parsing Error!\n\n");
}

