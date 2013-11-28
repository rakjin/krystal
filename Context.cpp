#include <set>

#include "Context.h"

#include "SyntaxTree.h"

using namespace std;

Krystal::Context::Context() {
	includedFiles = new map<string, bool>();
	declarations = new map<string, Node*>();
}

bool Krystal::Context::insertIncludedFile(string* fileName)
{
	pair< map<string, bool>::iterator, bool > pr;
	pr = includedFiles->insert( map<string, bool>::value_type(*fileName, false) );
	return pr.second;
}

bool Krystal::Context::markIncludedFileAsProcessed(string* fileName)
{
	map<string, bool>::iterator findIterator = includedFiles->find(*fileName);

	if (findIterator != includedFiles->end())
	{
		findIterator->second = true;
		return true; // success
	}

	return false; // no such fileName
}

string* Krystal::Context::getUnprocessedFileName()
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

bool Krystal::Context::insertDeclaration(string* declarationName, Node* node)
{
	pair< map<string, Node*>::iterator, bool > pr;
	pr = declarations->insert( map<string, Node*>::value_type(*declarationName, node) );
	return pr.second;
}

Node* Krystal::Context::getDeclarationNode(string* declarationName)
{
	map<string, Node*>::iterator findIterator = declarations->find(*declarationName);

	if (findIterator != declarations->end())
	{
		return findIterator->second;
	}

	return NULL;
}

