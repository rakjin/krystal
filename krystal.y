%{
#include <stdio.h>
#include <ctype.h>

int yyerror(char* s);

%}

%token ID
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
					;

packet				:	PACKET BLOCK_BEGIN BLOCK_END
						{
							printf("packet declaration\n");
						}
					;

include				:	INCLUDE
						{
							printf("include directive\n");
						}

unknown_command		:	ID
						{
							printf("unknown command\n");
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

