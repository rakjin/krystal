#include "KstFile.h"

#include <fstream>
#include <set>
#include "Scanner.h"
#include "SyntaxTree.h"
#include "Context.h"

using namespace std;

/**
 * Open and parse a file
 */
Krystal::KstFile::KstFile(string* fileName, Context &_context) throw(string) {
	ifstream inFile(fileName->c_str());
	if (!inFile.good()) {
		throw string("Unable to open file");
	}
	
	context = &_context;

	Krystal::Scanner scanner(&inFile);
	root = NULL;
	Krystal::Parser parser(scanner, root, fileName, context);
	parser.parse();
}

/**
 * Retrieve a value
 */
string* Krystal::KstFile::getParsed() {
	return root->getParsed(0);
}


