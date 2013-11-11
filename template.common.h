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

int getHashCode(std::string* str)
{
	boost::hash<std::string> str_hash;
	return str_hash(*str);
}

#endif //TEMPLATE_COMMON_H
