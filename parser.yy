%require "2.4.1"
%skeleton "lalr1.cc"
%defines
%locations
%define namespace "Rakjin::Krystal"
%define parser_class_name "Parser"
%parse-param { Rakjin::Krystal::Scanner &scanner }
%parse-param { Node* &root }
%parse-param { std::string* fileName }
%lex-param   { Rakjin::Krystal::Scanner &scanner }

%code requires {
	#ifdef _WIN32
	#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
	#endif

	#include <string>
	#include <sstream>
	#include <map>

	#include "SyntaxTree.h"

	// We want to return a string
	// #define YYSTYPE std::string

	namespace Rakjin {
		namespace Krystal {
			// Forward-declare the Scanner class; the Parser needs to be assigned a 
			// Scanner, but the Scanner can't be declared without the Parser
			class Scanner;
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
%token LT GT
%token SEMICOLON COMMA
%token MAP LIST

%token PACKET
%token INCLUDE

%token <string>	ID
%token <string>	STRING_LITERAL
%token <string>	UNKNOWN_CHARACTER

%token <string>	PRIMITIVE_DATA_TYPE
%token <string>	REFERENCE_DATA_TYPE

%type <string>				unknown_command
//%destructor { delete $$; }	unknown_command

%type <node>				kst command include packet packet_member packet_member_type packet_member_name
//%destructor { delete $$; }	kst command include packet packet_member packet_member_type packet_member_name

%type <nodes>				commands packet_members
//%destructor { delete $$; }	commands packet_members


%%

/* the root node of a .kst file */
kst :
	/* null */
	{
		std::list<Node*>* emptyCommands = new std::list<Node*>;
		root = new NodeKst(emptyCommands, fileName);
		$$ = root;
	}
	|
	commands
	{
		root = new NodeKst($1, fileName);
		$$ = root;
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
	PACKET ID BLOCK_BEGIN packet_members BLOCK_END
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
	PRIMITIVE_DATA_TYPE
	{
		//std::cout << "packet_member_type: " << *$1 << "\n";
		$$ = new NodePacketMemberType(token::PRIMITIVE_DATA_TYPE, $1);
	}
	|
	ID
	{
		$$ = new NodePacketMemberType(token::REFERENCE_DATA_TYPE, $1);
	}
	|
	LIST LT packet_member_type GT
	{
		$$ = new NodePacketMemberType(token::LIST, $3);
	}
	|
	MAP LT packet_member_type COMMA packet_member_type GT
	{
		$$ = new NodePacketMemberType(token::MAP, $3, $5);
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
		$$ = $1;
	}
	|
	UNKNOWN_CHARACTER
	{
		$$ = $1;
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


#include "SyntaxTree.cpp"

