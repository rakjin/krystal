#ifndef CONTEXT_H
#define CONTEXT_H

#include <set>
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
			set<string>* includedFiles; //table for included files' names during parsing
			set<string>* declarations; //table for declarations(enums, packets) names during parsing
	};

}

#endif // CONTEXT_H
