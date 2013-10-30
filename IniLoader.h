/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */
 
#pragma once

#include <fstream>
#include "Scanner.h"

namespace Waffleshop {
	class IniFile {
		public:
			// can instantiate with either a file name or an already open stream
			inline explicit IniFile(const char * const fileName) throw(std::string);
			inline explicit IniFile(std::istream &iniStream) throw(std::string);

			// Get a value from section and key
			const char * getValue(const char * const section, const char * const key) const;
		private:
			// supress default constructor
			IniFile();
			// supress default copy constructor
			IniFile(IniFile const &rhs);
			// supress default assignment operator
			IniFile &operator=(IniFile const &rhs);
			
			// the ini data
			IniLoader::mapData iniData;
	};
	
	/**
	 * Open and parse a file
	 */
	IniFile::IniFile(const char * const fileName) throw(std::string) {
		std::ifstream inFile(fileName);
		if (!inFile.good()) {
			throw std::string("Unable to open file");
		}
		
		IniLoader::Scanner scanner(&inFile);
		IniLoader::Parser parser(scanner, "", iniData);
		parser.parse();
	}

	/**
	 * Parse an already open stream
	 */
	IniFile::IniFile(std::istream &iniStream) throw(std::string) {
		IniLoader::Scanner scanner(&iniStream);
		IniLoader::Parser parser(scanner, "", iniData);
		parser.parse();
	}
	

	/**
	 * Retrieve a value
	 */
	char const * IniFile::getValue(const char * const section, const char * const key) const {
	    // find the section
	    IniLoader::mapData::const_iterator iSection = iniData.find(section);
	    if (iSection == iniData.end()) {
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

