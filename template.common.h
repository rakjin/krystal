#ifndef TEMPLATE_COMMON_H
#define TEMPLATE_COMMON_H

#include <boost/functional/hash.hpp>
#include <boost/algorithm/string/replace.hpp>

std::string* indent(std::string* src)
{
    boost::algorithm::replace_all(*src, "\n", "\n\t");
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
