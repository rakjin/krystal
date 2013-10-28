%{
#include <stdio.h>
#include <ctype.h>

int yyerror(char* s);

%}

%token ID







%%

program		:
			|	commands
			;

commands	:	command
			|	commands command
			;

command		:	ID
				{
					printf("command\n");
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

