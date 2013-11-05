#pragma once

#include <fstream>
#include "Scanner.h"

namespace Rakjin {
	class KstFile {
		public:
			// can instantiate with either a file name or an already open stream
			inline explicit KstFile(const char * const fileName) throw(std::string);
			inline explicit KstFile(std::istream &kstStream) throw(std::string);

			// Get a value from section and key
			const char * getValue(const char * const section, const char * const key) const;
		private:
			// supress default constructor
			KstFile();
			// supress default copy constructor
			KstFile(KstFile const &rhs);
			// supress default assignment operator
			KstFile &operator=(KstFile const &rhs);
			
			// the kst data
			Krystal::mapData kstData;
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
		Krystal::Parser parser(scanner, kstData);
		parser.parse();
	}

	/**
	 * Parse an already open stream
	 */
	KstFile::KstFile(std::istream &kstStream) throw(std::string) {
		Krystal::Scanner scanner(&kstStream);
		Krystal::Parser parser(scanner, kstData);
		parser.parse();
	}
	

	/**
	 * Retrieve a value
	 */
	char const * KstFile::getValue(const char * const section, const char * const key) const {
	    // find the section
	    Krystal::mapData::const_iterator iSection = kstData.find(section);
	    if (iSection == kstData.end()) {
	        return 0;
	    }
	    
	    // find the value
	    std::map<std::string, std::string>::const_iterator iValue = iSection->second.find(key);
	    if (iValue == iSection->second.end()) {
	        return 0;
	    }
	    
	    // return the result
		return iValue->second.c_str();
	}
}

