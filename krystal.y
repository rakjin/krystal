%{
#include <stdio.h>
#include <ctype.h>

int yyerror(char* s);

%}

%token ID
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
					|	ID
						{
							printf("command\n");
						}
					;

packet	:			PACKET BLOCK_BEGIN BLOCK_END
						{
							printf("packet declaration\n");
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

