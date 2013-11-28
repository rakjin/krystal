#include <set>

#include "Context.h"

using namespace std;

namespace Rakjin {

	Context::Context() {
		includedFiles = new map<string, bool>();
		declarations = new map<string, int>();
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

	string* Context::getUnprocessedFileName()
	{
		string* fileName = NULL; // if unprocessed file not found, return null

		map<string, bool>::iterator i;
		for(i = includedFiles->begin(); i != includedFiles->end(); i++)
		{
			bool processed = i->second;
			if (processed == false)
			{
				fileName = new string(i->first);
				break;
			}
		}

		return fileName;
	}

	bool Context::insertDeclaration(string* declarationName)
	{
		pair< map<string, int>::iterator, bool > pr;
		pr = declarations->insert( map<string, int>::value_type(*declarationName, 0) );
		return pr.second;
	}
	
}

