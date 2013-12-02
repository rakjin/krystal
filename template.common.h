#ifndef TEMPLATE_COMMON_H
#define TEMPLATE_COMMON_H

#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace boost;

// must correspond with the rules in scanner.l, plus a LENGTH_OF_ item (used for LENGTH)
namespace KstDataType
{
	typedef enum Enum
	{
		boolType = 0,
		byteType,
		sbyteType,
		shortType,
		ushortType,
		intType,
		uintType,
		longType,
		ulongType,
		floatType,
		doubleType,
		stringType,

		LENGTH_OF_KST_DATA_TYPES
	} Enum;
}

// must correspond with KstDataType enum
string KstDataTypeLookup[KstDataType::LENGTH_OF_KST_DATA_TYPES] =
{
	"bool",
	"byte",
	"sbyte",
	"short",
	"ushort",
	"int",
	"uint",
	"long",
	"ulong",
	"float",
	"double",
	"string"
};

KstDataType::Enum lookupKstDataType(string* typeString)
{
	for (int i = 0; i < KstDataType::LENGTH_OF_KST_DATA_TYPES; i++)
	{
		if (KstDataTypeLookup[i].compare(*typeString) == 0) //same
		{
			return (KstDataType::Enum)i;
		}
	}
	return ((KstDataType::Enum)(-1));
}


string* indent(string* src)
{
    algorithm::replace_all(*src, "\n", "\n\t");
    algorithm::trim_right_if(*src, algorithm::is_any_of("\t"));
    string result("\t");
    result = result + *src;
    delete src;
    return new string(result);
}

string* indent(string* src, unsigned int depth)
{
	if (depth < 1)
	{
		throw string("wrong indent depth");
	}

	for (unsigned int i = 0; i < depth; i++)
	{
		src = indent(src);
	}
	return src;
}

boost::hash<string> str_hash;
size_t getHashCode(string* str)
{	
	return str_hash(*str);
}

boost::hash<int> int_hash;
size_t getHashCode(int intValue)
{	
	return int_hash(intValue);
}

void combineHashCode(size_t &targetHash, size_t additionalHash)
{
	hash_combine(targetHash, (unsigned int)additionalHash);
}

#endif //TEMPLATE_COMMON_H
