#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <string>

#include "SyntaxTree.h"

using namespace std;

namespace Rakjin {

	class Context {
		public:
			Context();
			bool insertIncludedFile(string* fileName);
			bool markIncludedFileAsProcessed(string* fileName);
			string* getUnprocessedFileName();
			bool insertDeclaration(string* declarationName);
		private:
			map<string, bool>* includedFiles; //table for INCLUDED_FILE_NAME as key and PROCESSED as value
			map<string, int>* declarations; //table for DECLARATION_NAME(enums, packets) as key and HASH as value
	};

}


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
 * Included Files Table
 *   map <string, bool>* includedFiles;
 *     - <name_of_files_to_be_processed, is_processed>
 *     - including Aaa.kst as first element
 * Declarations Table
 *   map<string, int>* declarations;
 *     - <name_of_declarations, hash>
 *     - 0 hash means not yet been calculated
 * 
 * * * * * * * * * * * * * * * * * * * */



#endif // CONTEXT_H
