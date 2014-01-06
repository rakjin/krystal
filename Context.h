#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <string>

#include "SyntaxTree.h"
	namespace Krystal { class Node; }

using namespace std;

namespace Krystal
{
	class Context;
}

using namespace Krystal;

class Krystal::Context
{
	public:
		Context();
		bool insertUnprocessedFile(string* fileName);
		bool markIncludedFileAsProcessed(string* fileName);
		string* getUnprocessedFileName();

		bool insertDeclaration(string* declarationName, Node* node);
		Node* getDeclarationNode(string* declarationName); // retreive Node* by declaration name from the table
	private:
		map<string, bool>* filesToProcess; //table for INCLUDED_FILE_NAME as key and PROCESSED as value
		map<string, Node*>* declarations; //table for DECLARATION_NAME(enums, packets) as key and Node* as value
};




/* * * * * * * * * * * * * * * * * * * * 
 * Aaa.kst:
 *   #include Bbb.kst
 *   packet A { B b; };
 * 
 * Bbb.kst:
 *   #include Ccc.kst
 *   packet B { C c; };
 * 
 * Ccc.kst:
 *   packet C { int i; };
 * 
 * this should work:
 *   $krystal -cs Aaa.kst > Aaa.cs
 * and hash change of packet C
 * should influence all the others' hash
 * even when processing Aaa.kst solely
 *
 * Table for files to be processed
 *   map <string, bool>* filesToProcess;
 *     - <name_of_files_to_be_processed, is_processed>
 *     - including Aaa.kst as first element
 * Declarations Table
 *   map<string, Node*>* declarations;
 *     - <name_of_declarations, Node instance>
 *     - Node reference to the declaration
 * 
 * * * * * * * * * * * * * * * * * * * */



#endif // CONTEXT_H
