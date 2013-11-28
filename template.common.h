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

void combineHashCode(std::size_t &targetHash, int additionalIntHash)
{
	boost::hash_combine(targetHash, additionalIntHash);
}

#endif //TEMPLATE_COMMON_H
