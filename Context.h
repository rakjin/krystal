#ifndef CONTEXT_H
#define CONTEXT_H

#include <set>
#include <map>
#include <string>

using namespace std;

namespace Rakjin {

	class Context {
		public:
			Context();
			bool insertIncludedFile(string* fileName);
			bool insertDeclaration(string* declarationName);
		private:
			bool insertStringIntoSet(set<string> &targetSet, string* str);
			map<string, bool>* includedFiles; //table for INCLUDED_FILE_NAME as key and PROCESSED as value
			set<string>* declarations; //table for declarations(enums, packets) names during parsing
	};

}

#endif // CONTEXT_H
