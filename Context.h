#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <string>

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

#endif // CONTEXT_H
