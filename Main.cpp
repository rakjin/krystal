#include <string>
#include <iostream>
#include <cstring>

#include <boost/format.hpp>

#include "Krystal.h"
#include "Context.h"

#include "template.cs.h"

using namespace std;

int main(int argc, char * argv[]) {
	// make sure we received a filename
	if (argc != 2) {
		cerr << "Usage: ./krystal [FILENAME]" << endl;
		return 255;
	}

	Rakjin::Context context = Rakjin::Context();

	// try and open the KST file
	Rakjin::KstFile* kstFile = NULL;
	try
	{
		kstFile = new Rakjin::KstFile(argv[1], context);
	}
	catch (string error)
	{
		cerr << "ERROR: " << error << endl;
		return 255;
	}

	cout << *(kstFile->getParsed());

    delete kstFile;
	return 0;
}

