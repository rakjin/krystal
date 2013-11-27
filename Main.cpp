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
	string* fileNameFromArg = new string(argv[1]);
	context.insertIncludedFile(fileNameFromArg);

	// try and open the KST file
	Rakjin::KstFile* kstFile = NULL;
	string* fileName = NULL; //context.getAnUnprocessedFromIncludedFilesMap();

	for(fileName = context.getUnprocessedFileName();
		fileName != NULL;
		fileName = context.getUnprocessedFileName())
	{
		try
		{
			kstFile = new Rakjin::KstFile(fileName->c_str(), context);
		}
		catch (string error)
		{
			cerr << "ERROR: " << error << endl;
			return 255;
		}

		context.markIncludedFileAsProcessed(fileName);
	}

	cout << *(kstFile->getParsed()); // the last parsed

    delete kstFile;
	return 0;
}

