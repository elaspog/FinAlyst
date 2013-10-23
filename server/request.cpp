#include "request.hpp"
#include "utils.hpp"
#include "logger.hpp"

void parse_pairs(std::istream &urldata, char pair_delimiter, std::map<std::string, std::string> &pairs)
{
    std::string key;
    std::string value;
    enum { Key, Value } state = Key;
    char c;
    urldata >> c;
    while (!urldata.eof() && c != '\n')
    {
        if (state == Key)
        {
            if (c == '=') state = Value;
            else
            {
                if (c == pair_delimiter)
                {
                    throw std::logic_error("data parser error, unexpected '"
                            +std::string(1, pair_delimiter)+"'" + key);
                }
                key += c;
            }
        } else
        {
            if (c == pair_delimiter)
            {
                state = Key;
                char v[value.length()];
                urldecode(v, value.c_str());
                pairs[key] = std::string(v);
                key.clear();
                value.clear();
            } else
            {
                if (c == '=')
                {
                    throw std::logic_error("data parser error, unexcepted '='"
                            + key + " " + value);
                }
                value += c;
            }
        }
        urldata >> c;
    }
    if (state == Value)
    {
        char v[value.length()];
        urldecode(v, value.c_str());
        pairs[key] = std::string(v);
    }
    else if (!key.empty()) throw std::logic_error("data parser error, unterminated key");
}
