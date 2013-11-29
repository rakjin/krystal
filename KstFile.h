#ifndef KST_FILE_H
#define KST_FILE_H

#include "SyntaxTree.h"
#include "Context.h"

using namespace std;

namespace Krystal {

	class KstFile {
		public:
			// can instantiate with either a file name or an already open stream
			KstFile(string* fileName, Context &_context) throw(string);

			// Get a value from section and key
			string* getParsed();
		private:
			// supress default constructor
			KstFile();
			// supress default copy constructor
			KstFile(KstFile const &rhs);
			// supress default assignment operator
			KstFile &operator=(KstFile const &rhs);
			
			// the kst data
			Node* root;

			Context* context;
	};
}

#endif // KST_FILE_H
