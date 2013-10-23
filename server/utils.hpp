#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cctype>
#include <string>

bool parse_unsigned(std::string const &str, uint64_t &value);
void urldecode(char *dst, const char *src);

#endif
