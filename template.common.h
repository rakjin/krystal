#ifndef TEMPLATE_COMMON_H
#define TEMPLATE_COMMON_H

#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>

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


std::string* indent(std::string* src)
{
    boost::algorithm::replace_all(*src, "\n", "\n\t");
    boost::algorithm::trim_right_if(*src, boost::algorithm::is_any_of("\t"));
    std::string result("\t");
    result = result + *src;
    delete src;
    return new std::string(result);
}

boost::hash<std::string> str_hash;
std::size_t getHashCode(std::string* str)
{	
	return str_hash(*str);
}

boost::hash<int> int_hash;
std::size_t getHashCode(int intValue)
{	
	return int_hash(intValue);
}

void combineHashCode(std::size_t &targetHash, std::size_t additionalHash)
{
	boost::hash_combine(targetHash, (unsigned int)additionalHash);
}

#endif //TEMPLATE_COMMON_H
