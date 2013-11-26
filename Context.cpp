#include <set>

#include "Context.h"

using namespace std;

namespace Rakjin {

	Context::Context() {
		includedFiles = new set<string>();
		declarations = new set<string>();
	}

	bool Context::insertStringIntoSet(set<string> &targetSet, string* str)
	{
		// http://msdn.microsoft.com/ko-kr/library/547ckb56.aspx
		pair< set<string>::iterator, bool > pr;
		pr = targetSet.insert(*str);
		return pr.second;
	}

	bool Context::insertIncludedFile(string* fileName)
	{
		bool success = Context::insertStringIntoSet(*includedFiles, fileName);
		return success;
	}

	bool Context::insertDeclaration(string* declarationName)
	{
		bool success = Context::insertStringIntoSet(*declarations, declarationName);
		return success;
	}
	
}

