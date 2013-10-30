/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details. */
 
#include "IniLoader.h"
#include <string>
#include <iostream>
#include <cstring>

int main(int argc, char * argv[]) {
	// make sure we received a filename
	if (argc != 2) {
		std::cerr << "Usage: ./ini-parser [FILENAME]" << std::endl;
		return 255;
	}

	// try and open the INI file
	Waffleshop::IniFile * iniFile;
	try {
		// - means stdin, not a file named '-'
		if (strcmp(argv[1], "-") == 0) {
			iniFile = new Waffleshop::IniFile(std::cin);
		} else {
			iniFile = new Waffleshop::IniFile(argv[1]);
		}
	} catch (std::string error) {
		std::cerr << "ERROR: " << error << std::endl;
		return 255;
	}

	// print a value
	std::cout << iniFile->getValue("OMG", "WTF") << std::endl;
	std::cout << iniFile->getValue("ABC", "KEY1") << std::endl;
	std::cout << iniFile->getValue("ABC", "KEY2") << std::endl;
	std::cout << iniFile->getValue("ABC", "KEY3") << std::endl;

    delete iniFile;
	return 0;
}

