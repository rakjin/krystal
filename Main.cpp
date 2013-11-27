#include "Krystal.h"
#include "Context.h"
#include <string>
#include <iostream>
#include <cstring>

#include <boost/format.hpp>

#include "template.cs.h"

int main(int argc, char * argv[]) {
	// make sure we received a filename
	if (argc != 2) {
		std::cerr << "Usage: ./krystal [FILENAME]" << std::endl;
		return 255;
	}

	Rakjin::Context context = Rakjin::Context();

	// try and open the KST file
	Rakjin::KstFile* kstFile = NULL;
	try
	{
		kstFile = new Rakjin::KstFile(argv[1], context);
	}
	catch (std::string error)
	{
		std::cerr << "ERROR: " << error << std::endl;
		return 255;
	}

	std::cout << *(kstFile->getParsed());

    delete kstFile;
	return 0;
}

