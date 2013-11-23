#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cctype>
#include <string>
#include <ostream>
#include <functional>
#include <map>

typedef std::map<std::string, std::string> OptsMap;
typedef std::function<void (std::string const&, std::string const&)> OptsCb;

void save_pairs(std::ostream &output, char delimiter, OptsMap const &pairs);
void parse_pairs(std::istream &urldata, char pair_delimiter, OptsMap &pairs);
bool parse_unsigned(std::string const &str, uint64_t &value);
void urldecode(char *dst, const char *src);
void jsonescape(std::string const &str, std::ostream &out);
void htmlspecialchars(std::string const &str, std::ostream &out);

#endif
