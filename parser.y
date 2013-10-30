%require "2.4.1"
%skeleton "lalr1.cc"
%defines
%locations
%define namespace "Rakjin::Krystal"
%define parser_class_name "Parser"
%parse-param { Rakjin::Krystal::Scanner &scanner }
%parse-param { std::string currentSection }
%parse-param { Rakjin::Krystal::mapData &kstData }
%lex-param   { Rakjin::Krystal::Scanner &scanner }

%code requires {
	#include <string>
	#include <sstream>
	#include <map>

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

%token STRING SECTION_START SECTION_END ASSIGNMENT

%%

input
	: input line
	| line
	;
	
line
	: section
	| value
	;
	
section
	: SECTION_START STRING SECTION_END { currentSection = $2; }
	;
	
value
	: STRING ASSIGNMENT STRING { kstData[currentSection][$1] = $3; }
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

