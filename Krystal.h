#pragma once

#include <fstream>
#include <set>
#include "Scanner.h"
#include "SyntaxTree.h"

using namespace std;

namespace Rakjin {

	class Domain {
		public:
			Domain();
			bool insertIncludedFile(string* fileName);
			bool insertDeclaration(string* declarationName);
		private:
			bool insertStringIntoSet(set<string> &targetSet, string* str);
			set<string>* includedFiles; //table for included files' names during parsing
			set<string>* declarations; //table for declarations(enums, packets) names during parsing
	};

	Domain::Domain() {
		includedFiles = new set<string>();
		declarations = new set<string>();
	}

	bool Domain::insertStringIntoSet(set<string> &targetSet, string* str)
	{
		// http://msdn.microsoft.com/ko-kr/library/547ckb56.aspx
		pair< set<string>::iterator, bool > pr;
		pr = targetSet.insert(*str);
		return pr.second;
	}

	bool Domain::insertIncludedFile(string* fileName)
	{
		bool success = Domain::insertStringIntoSet(*includedFiles, fileName);
		return success;
	}

	bool Domain::insertDeclaration(string* declarationName)
	{
		bool success = Domain::insertStringIntoSet(*declarations, declarationName);
		return success;
	}


	class KstFile {
		public:
			// can instantiate with either a file name or an already open stream
			inline explicit KstFile(const char * const fileName, Domain &_domain) throw(string);
			inline explicit KstFile(istream &kstStream, Domain &_domain) throw(string);

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

			Domain* domain;
	};
	
	/**
	 * Open and parse a file
	 */
	KstFile::KstFile(const char * const fileName, Domain &_domain) throw(string) {
		ifstream inFile(fileName);
		if (!inFile.good()) {
			throw string("Unable to open file");
		}
		
		domain = &_domain;

		Krystal::Scanner scanner(&inFile);
		root = NULL;
		Krystal::Parser parser(scanner, root, new string(fileName));
		parser.parse();
	}

	/**
	 * Parse an already open stream
	 */
	KstFile::KstFile(istream &kstStream, Domain &_domain) throw(string) {

		domain = &_domain;

		Krystal::Scanner scanner(&kstStream);
		root = NULL;
		Krystal::Parser parser(scanner, root, new string("stream"));
		parser.parse();
	}
	

	/**
	 * Retrieve a value
	 */
	string* KstFile::getParsed() {
		return root->getParsed(0);
	}
}

