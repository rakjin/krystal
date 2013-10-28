%{
#include <stdio.h>
#include <ctype.h>

int yyerror(char* s);

%}

%token DIGIT







%%

line	:	expr '\n'		{ putchar('\n'); }
		;

expr	:	expr '+' term	{ putchar('+'); }
		|	expr '-' term	{ putchar('-'); }
		|	term
		;

term	:	DIGIT			{ printf("%d", yylval); }
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
