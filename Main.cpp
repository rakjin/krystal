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

	Rakjin::KstFile* kstFile = NULL;
	string* fileNameToProcess = context.getUnprocessedFileName();

	do
	{
		try
		{
			// parse file and fill context
			kstFile = new Rakjin::KstFile(fileNameToProcess->c_str(), context);
		}
		catch (string error)
		{
			cerr << "ERROR: " << error << endl;
			return 255;
		}

		context.markIncludedFileAsProcessed(fileNameToProcess);
		fileNameToProcess = context.getUnprocessedFileName();
	}
	while(fileNameToProcess != NULL);

	cout << *(kstFile->getParsed()); // the last parsed

    delete kstFile;
	return 0;
}

