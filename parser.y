%require "2.4.1"
%skeleton "lalr1.cc"
%defines
%locations
%define namespace "Rakjin::Krystal"
%define parser_class_name "Parser"
%parse-param { Rakjin::Krystal::Scanner &scanner }
%parse-param { Rakjin::Krystal::mapData &kstData }
%lex-param   { Rakjin::Krystal::Scanner &scanner }

%code requires {
	#include <string>
	#include <sstream>
	#include <map>

	#include "expression.h"

	// We want to return a string
	#define YYSTYPE std::string

	namespace Rakjin {
		namespace Krystal {
			// Forward-declare the Scanner class; the Parser needs to be assigned a 
			// Scanner, but the Scanner can't be declared without the Parser
			class Scanner;
		
			// We use a map to store the KST(INI) data
			typedef std::map<std::string, std::map<std::string, std::string> > mapData;
		}
	}
}

%code {
	// Prototype for the yylex function
	static int yylex(Rakjin::Krystal::Parser::semantic_type * yylval,
	                 Rakjin::Krystal::Parser::location_type * yylloc,
	                 Rakjin::Krystal::Scanner &scanner);
}

%token BLOCK_BEGIN BLOCK_END
%token SEMICOLON
%token STRING_LITERAL
%token PACKET
%token INCLUDE
%token BOOL INT UINT
%token ID

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

packet				:	PACKET ID BLOCK_BEGIN packet_members BLOCK_END
						{
							std::cout << "packet: " << $2 << "\n";
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
							std::cout << "packet_member_type: " << $1 << "\n";
						}
					;

packet_member_name	:	ID
						{
							std::cout << "packet_member_name: " << $1 << "\n";
						}
					;

include				:	INCLUDE STRING_LITERAL
						{
							std::cout << "include directive: " << $2 << "\n";
						}

unknown_command		:	ID
						{
							std::cout << "unknown command: " << $1 << "\n";
						}
					;


	
%%

// Error function throws an exception (std::string) with the location and error message
void Rakjin::Krystal::Parser::error(const Rakjin::Krystal::Parser::location_type &loc,
                                          const std::string &msg) {
	std::ostringstream ret;
	ret << "Parser Error at " << loc << ": " << msg;
	throw ret.str();
}

// Now that we have the Parser declared, we can declare the Scanner and implement
// the yylex function
#include "Scanner.h"
static int yylex(Rakjin::Krystal::Parser::semantic_type * yylval,
                 Rakjin::Krystal::Parser::location_type * yylloc,
                 Rakjin::Krystal::Scanner &scanner) {
	return scanner.yylex(yylval, yylloc);
}

