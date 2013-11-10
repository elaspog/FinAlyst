#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cctype>
#include <string>
#include <ostream>

bool parse_unsigned(std::string const &str, uint64_t &value);
void urldecode(char *dst, const char *src);
void jsonescape(std::string const &str, std::ostream &out);

#endif
