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
	string* fileNameToProcess = NULL;

	try
	{
		// process file from argument
		kstFile = new Rakjin::KstFile(fileNameFromArg, context);
		context.markIncludedFileAsProcessed(fileNameFromArg);

		// process including files if any
		for(fileNameToProcess = context.getUnprocessedFileName();
			fileNameToProcess != NULL;
			fileNameToProcess = context.getUnprocessedFileName())
		{
			new Rakjin::KstFile(fileNameToProcess, context);
			context.markIncludedFileAsProcessed(fileNameToProcess);
		}
	}
	catch (string error)
	{
		cerr << "ERROR: " << error << endl;
		return 255;
	}

	cout << *(kstFile->getParsed()); // the first parsed

    delete kstFile;
	return 0;
}

