%require "2.4.1"
%skeleton "lalr1.cc"
%defines
%locations
%define namespace "Krystal"
%define parser_class_name "Parser"
%parse-param { Krystal::Scanner &scanner }
%parse-param { Node* &root }
%parse-param { string* fileName }
%parse-param { Krystal::Context* &context }
%lex-param   { Krystal::Scanner &scanner }

%code requires {
	#ifdef _WIN32
	#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
	#endif

	#include <string>
	#include <sstream>
	#include <map>

	#include "SyntaxTree.h"
	#include "Context.h"

	using namespace std;

	#define ALLOW_DUPLICATED_INCLUDE		true
	namespace Krystal {
		// Forward-declare the Scanner class; the Parser needs to be assigned a 
		// Scanner, but the Scanner can't be declared without the Parser
		class Scanner;
	}
}

%code {
	// Prototype for the yylex function
	static int yylex(Krystal::Parser::semantic_type * yylval,
	                 Krystal::Parser::location_type * yylloc,
	                 Krystal::Scanner &scanner);
}

%union {
	int integerValue;
	string* stringValue;
	Node* node;
	NodePacketMember* nodePacketMember;
	list<Node*>* nodes;
	list<NodePacketMember*>* nodePacketMembers;
}

%token BLOCK_BEGIN BLOCK_END
%token LT GT
%token SEMICOLON COMMA
%token MAP LIST
%token STREAM

%token PACKET
%token INCLUDE

%token <stringValue>	ID
%token <stringValue>	STRING_LITERAL
%token <stringValue>	UNKNOWN_CHARACTER

%token <stringValue>	URI

%token <stringValue>	PRIMITIVE_DATA_TYPE
%token <stringValue>	REFERENCE_DATA_TYPE

%type <stringValue>				unknown_command
//%destructor { delete $$; }	unknown_command

%type <node>				kst command include packet packet_member_type packet_member_name
//%destructor { delete $$; }	kst command include packet packet_member_type packet_member_name

%type <nodePacketMember>	packet_member
//%destructor { delete $$; }	packet_member

%type <nodes>				commands
//%destructor { delete $$; }	commands

%type <nodePacketMembers>				packet_members
//%destructor { delete $$; }	packet_members


%%

/* the root node of a .kst file */
kst :
	/* null */
	{
		list<Node*>* emptyCommands = new list<Node*>;
		root = new NodeKst(context, emptyCommands, fileName);
		$$ = root;
	}
	|
	commands
	{
		root = new NodeKst(context, $1, fileName);
		$$ = root;
	}

commands :
	command
	{
		$$ = new list<Node*>;

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
	SEMICOLON
	{
		$$ = NULL;
	}
	|
	unknown_command
	{
		$$ = NULL;
		error(yyloc, string("Unknown Command \"") + *$1 + "\"");
		delete $1;
		YYERROR;
	}

packet :
	PACKET ID BLOCK_BEGIN packet_members BLOCK_END
	{
		$$ = new NodePacket(context, $2, $4);
		bool success = context->insertDeclaration($2, $$);
		if (success == false)
		{
			error(yyloc, string("DUPLICATED packet ") + *$2);
		}
	}
	|
	PACKET ID BLOCK_BEGIN URI packet_members BLOCK_END
	{
		$$ = new NodePacket(context, $2, $5, $4);
		bool success = context->insertDeclaration($2, $$);
		if (success == false)
		{
			error(yyloc, string("DUPLICATED packet ") + *$2);
		}
	}
	|
	PACKET ID BLOCK_BEGIN BLOCK_END
	{
		list<NodePacketMember*>* emptyMembers = new list<NodePacketMember*>();
		$$ = new NodePacket(context, $2, emptyMembers);
		bool success = context->insertDeclaration($2, $$);
		if (success == false)
		{
			error(yyloc, string("DUPLICATED packet ") + *$2);
		}
	}
	|
	PACKET ID BLOCK_BEGIN URI BLOCK_END
	{
		list<NodePacketMember*>* emptyMembers = new list<NodePacketMember*>();
		$$ = new NodePacket(context, $2, emptyMembers, $4);
		bool success = context->insertDeclaration($2, $$);
		if (success == false)
		{
			error(yyloc, string("DUPLICATED packet ") + *$2);
		}
	}

packet_members :
	packet_member
	{
		$$ = new list<NodePacketMember*>;
		$$->push_back( $1 );
	}
	|
	packet_members packet_member
	{
		string* currentMemberName = $2->getName();
		list<NodePacketMember*>::iterator i = $1->begin();
		list<NodePacketMember*>::iterator end = $1->end();
		for (; i != end; i++)
		{
			string* memberName = (*i)->getName();
			if (currentMemberName->compare(*memberName) == 0) //packet member name duplication
			{
				error(yyloc, string("DUPLICATED packet member: ") + *memberName);
			}
		}

		$1->push_back( $2 );
		$$ = $1;
	}

packet_member :
	packet_member_type packet_member_name SEMICOLON
	{
		$$ = new NodePacketMember(context, $1, $2);
	}

packet_member_type :
	PRIMITIVE_DATA_TYPE
	{
		$$ = new NodePacketMemberType(context, token::PRIMITIVE_DATA_TYPE, $1);
	}
	|
	ID
	{
		$$ = new NodePacketMemberType(context, token::REFERENCE_DATA_TYPE, $1);
	}
	|
	LIST LT packet_member_type GT
	{
		$$ = new NodePacketMemberType(context, token::LIST, $3);
	}
	|
	MAP LT packet_member_type COMMA packet_member_type GT
	{
		$$ = new NodePacketMemberType(context, token::MAP, $3, $5);
	}
	|
	STREAM
	{
		$$ = new NodePacketMemberType(context, token::STREAM);
	}

packet_member_name :
	ID
	{
		$$ = new NodePacketMemberName(context, $1);
	}

include :
	INCLUDE STRING_LITERAL
	{
		$$ = new NodeInclude(context, $2);
		bool success = context->insertUnprocessedFile($2);
		if (success == false && ALLOW_DUPLICATED_INCLUDE == false)
		{
			error(yyloc, string("DUPLICATED #include \"") + *$2 + "\"");
		}
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

// Error function throws an exception (string) with the location and error message
void Krystal::Parser::error(const Krystal::Parser::location_type &loc,
                                          const string &msg) {
	ostringstream ret;
	ret << "Parser Error at " << *fileName << " " << loc << ": " << msg;
	throw ret.str();
}

// Now that we have the Parser declared, we can declare the Scanner and implement
// the yylex function
#include "Scanner.h"
static int yylex(Krystal::Parser::semantic_type * yylval,
                 Krystal::Parser::location_type * yylloc,
                 Krystal::Scanner &scanner) {
	return scanner.yylex(yylval, yylloc);
}


#include "SyntaxTree.cpp"

