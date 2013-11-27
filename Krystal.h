#pragma once

#include <fstream>
#include <set>
#include "Scanner.h"
#include "SyntaxTree.h"
#include "Context.h"

using namespace std;

namespace Rakjin {

	class KstFile {
		public:
			// can instantiate with either a file name or an already open stream
			inline explicit KstFile(const char * const fileName, Context &_context) throw(string);
			inline explicit KstFile(istream &kstStream, Context &_context) throw(string);

			// Get a value from section and key
			string* getParsed();
		private:
			// supress default constructor
			KstFile();
			// supress default copy constructor
			KstFile(KstFile const &rhs);
			// supress default assignment operator
			KstFile &operator=(KstFile const &rhs);
			
			// the kst data
			Node* root;

			Context* context;
	};
	
	/**
	 * Open and parse a file
	 */
	KstFile::KstFile(const char * const fileName, Context &_context) throw(string) {
		ifstream inFile(fileName);
		if (!inFile.good()) {
			throw string("Unable to open file");
		}
		
		context = &_context;

		Krystal::Scanner scanner(&inFile);
		root = NULL;
		Krystal::Parser parser(scanner, root, new string(fileName), context);
		parser.parse();
	}

	/**
	 * Retrieve a value
	 */
	string* KstFile::getParsed() {
		return root->getParsed(0);
	}
}

