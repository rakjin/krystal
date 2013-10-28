%{
#include <stdio.h>
#include <ctype.h>
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

int main()
{
	if (yyparse() == 0) printf("Parsing OK!\n\n");
	else printf("Parsing Error!\n\n");
}
