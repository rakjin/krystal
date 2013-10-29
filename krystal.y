%{
#include <stdio.h>
#include <ctype.h>

int yyerror(char* s);

%}


%union
{
	char* string;
}

%token<string> ID
%token<string>	STRING_LITERAL

%token SEMICOLON

%token<string> BOOL INT UINT

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

packet				:	PACKET BLOCK_BEGIN packet_members BLOCK_END
						{
							printf("packet\n");
						}
					;

packet_members		:	packet_member
					|	packet_members packet_member
					;

packet_member		:	packet_member_type packet_member_name SEMICOLON
					;

packet_member_type	:	BOOL
					|	INT
					|	UINT
						{
							printf("packet_member_type: %s\n", $1);
						}
					;

packet_member_name	:	ID
						{
							printf("packet_member_name: %s\n", $1);
						}
					;

include				:	INCLUDE STRING_LITERAL
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

