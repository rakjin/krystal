#include "Krystal.h"
#include <string>
#include <iostream>
#include <cstring>

int main(int argc, char * argv[]) {
	// make sure we received a filename
	if (argc != 2) {
		std::cerr << "Usage: ./krystal [FILENAME]" << std::endl;
		return 255;
	}

	// try and open the KST file
	Rakjin::KstFile * kstFile;
	try {
		// - means stdin, not a file named '-'
		if (strcmp(argv[1], "-") == 0) {
			kstFile = new Rakjin::KstFile(std::cin);
		} else {
			kstFile = new Rakjin::KstFile(argv[1]);
		}
	} catch (std::string error) {
		std::cerr << "ERROR: " << error << std::endl;
		return 255;
	}

	// print a value
	std::cout << kstFile->getValue("OMG", "WTF") << std::endl;
	std::cout << kstFile->getValue("ABC", "KEY1") << std::endl;
	std::cout << kstFile->getValue("ABC", "KEY2") << std::endl;
	std::cout << kstFile->getValue("ABC", "KEY3") << std::endl;

    delete kstFile;
	return 0;
}

