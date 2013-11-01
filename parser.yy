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
	// #define YYSTYPE std::string

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

%union {
	int inteverValue;
	std::string* string;
	Node* node;
	std::list<Node*>* nodes;
}

%token BLOCK_BEGIN BLOCK_END
%token SEMICOLON
%token PACKET
%token INCLUDE

%token <string>	ID
%token <string>	STRING_LITERAL
%token <string>	DATA_TYPE
%token <string> UNKNOWN_CHARACTER

%type <string>				unknown_command
%destructor { delete $$; }	unknown_command

%type <node>				kst command include packet packet_member packet_member_type packet_member_name
%destructor { delete $$; }	kst command include packet packet_member packet_member_type packet_member_name

%type <nodes>				commands packet_members
%destructor { delete $$; }	commands packet_members


%%

/* the root node of a .kst file */
kst :
	/* null */
	{
		std::list<Node*>* commands = new std::list<Node*>;
		$$ = new NodeKst(commands);
	}
	|
	commands
	{
		$$ = new NodeKst($1);
		std::cout << *($$->getParsed()) << "\n";
	}

commands :
	command
	{
		$$ = new std::list<Node*>;

		if ($1 != NULL)
		{
			$$->push_back( $1 );
		}
	}
	|
	commands command
	{
		if ($2 != NULL)
		{
			$$->push_back( $2 );
		}
		$$ = $1;
	}

command :
	packet
	{
		$$ = $1;
	}
	|
	include
	{
		$$ = $1;
	}
	|
	unknown_command
	{
		$$ = NULL;
		error(yyloc, std::string("Unknown Command \"") + *$1 + "\"");
		delete $1;
		YYERROR;
	}

packet :
	PACKET packet_member_name BLOCK_BEGIN packet_members BLOCK_END
	{
		//std::cout << "packet: " << *$2 << "\n";

		//std::list<Node*>::iterator i   = $4->begin();
		//std::list<Node*>::iterator end = $4->end();
		//for (; i != end; ++i)
		//{
		//	std::cout << "packet - member:" << *((*i)->getParsed()) << "\n";
		//}

		$$ = new NodePacket($2, $4);
		//std::cout << *($$->getParsed()) << "\n";
	}

packet_members :
	packet_member
	{
		//std::cout << "\tpacket_member\n";
		//std::cout << "\t\t" << *($1->getParsed()) << "\n";

		$$ = new std::list<Node*>;
		$$->push_back( $1 );
	}
	|
	packet_members packet_member
	{
		//std::cout << "\tpacket_members packet_member\n";
		//std::cout << "\t\t" << *($2->getParsed()) << "\n";

		$1->push_back( $2 );
		$$ = $1;
	}

packet_member :
	packet_member_type packet_member_name SEMICOLON
	{
		$$ = new NodePacketMember($1, $2);
		//std::cout << "packet_member: " << *($$->getParsed()) << "\n";
	}

packet_member_type :
	DATA_TYPE
	{
		//std::cout << "packet_member_type: " << *$1 << "\n";
		$$ = new NodePacketMemberType($1);
	}

packet_member_name :
	ID
	{
		//std::cout << "packet_member_name: " << *$1 << "\n";
		$$ = new NodePacketMemberName($1);
	}

include :
	INCLUDE STRING_LITERAL
	{
		//std::cout << "include directive: " << *$2 << "\n";
		$$ = new NodeInclude($2);
		//std::cout << *($$->getParsed()) << "\n";
	}

unknown_command :
	ID
	{
		$$ = new std::string(*$1);
	}
	|
	UNKNOWN_CHARACTER
	{
		$$ = new std::string(*$1);
	}




	
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

