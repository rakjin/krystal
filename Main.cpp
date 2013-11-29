#include <string>
#include <iostream>
#include <cstring>

#include <boost/format.hpp>

#include "KstFile.h"
#include "Context.h"

#include "template.cs.h"

using namespace std;

int main(int argc, char * argv[]) {
	// make sure we received a filename
	if (argc != 2) {
		cerr << "Usage: ./krystal [FILENAME]" << endl;
		return 255;
	}

	Krystal::Context context = Krystal::Context();
	string* fileNameFromArg = new string(argv[1]);
	context.insertIncludedFile(fileNameFromArg);

	Krystal::KstFile* kstFile = NULL;
	string* fileNameToProcess = NULL;

	try
	{
		// process file from argument
		kstFile = new Krystal::KstFile(fileNameFromArg, context);
		context.markIncludedFileAsProcessed(fileNameFromArg);

		// process including files if any
		for(fileNameToProcess = context.getUnprocessedFileName();
			fileNameToProcess != NULL;
			fileNameToProcess = context.getUnprocessedFileName())
		{
			new Krystal::KstFile(fileNameToProcess, context);
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

