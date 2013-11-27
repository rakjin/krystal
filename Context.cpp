#include <set>

#include "Context.h"

using namespace std;

namespace Rakjin {

	Context::Context() {
		includedFiles = new map<string, bool>();
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
		pair< map<string, bool>::iterator, bool > pr;
		pr = includedFiles->insert( map<string, bool>::value_type(*fileName, false) );
		return pr.second;
	}

	bool Context::markIncludedFileAsProcessed(string* fileName)
	{
		map<string, bool>::iterator findIterator = includedFiles->find(*fileName);

		if (findIterator != includedFiles->end())
		{
			findIterator->second = true;
			return true; // success
		}

		return false; // no such fileName
	}

	bool Context::insertDeclaration(string* declarationName)
	{
		bool success = Context::insertStringIntoSet(*declarations, declarationName);
		return success;
	}
	
}

