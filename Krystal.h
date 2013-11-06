#pragma once

#include <fstream>
#include "Scanner.h"
#include "SyntaxTree.h"

namespace Rakjin {
	class KstFile {
		public:
			// can instantiate with either a file name or an already open stream
			inline explicit KstFile(const char * const fileName) throw(std::string);
			inline explicit KstFile(std::istream &kstStream) throw(std::string);

			// Get a value from section and key
			std::string* getParsed();
		private:
			// supress default constructor
			KstFile();
			// supress default copy constructor
			KstFile(KstFile const &rhs);
			// supress default assignment operator
			KstFile &operator=(KstFile const &rhs);
			
			// the kst data
			Node* root = NULL;
	};
	
	/**
	 * Open and parse a file
	 */
	KstFile::KstFile(const char * const fileName) throw(std::string) {
		std::ifstream inFile(fileName);
		if (!inFile.good()) {
			throw std::string("Unable to open file");
		}
		
		Krystal::Scanner scanner(&inFile);
		root = NULL;
		Krystal::Parser parser(scanner, root);
		parser.parse();
	}

	/**
	 * Parse an already open stream
	 */
	KstFile::KstFile(std::istream &kstStream) throw(std::string) {
		Krystal::Scanner scanner(&kstStream);
		root = NULL;
		Krystal::Parser parser(scanner, root);
		parser.parse();
	}
	

	/**
	 * Retrieve a value
	 */
	std::string* KstFile::getParsed() {
		return root->getParsed();
	}
}

