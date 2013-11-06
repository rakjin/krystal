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
	Rakjin::KstFile* kstFile = NULL;
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

	std::cout << *(kstFile->getParsed());

    delete kstFile;
	return 0;
}

