#include "utils.hpp"
#include <algorithm>
#include <cstring>
#include <istream>
#include <stdexcept>

void save_pairs(std::ostream &output, char delimiter, OptsMap const &pairs)
{
    for (auto p : pairs)
        output << p.first << "=" << p.second << delimiter;
}

void parse_pairs(std::istream &urldata, char pair_delimiter, OptsMap &pairs)
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

bool parse_unsigned(std::string const &str, uint64_t &value)
{
    size_t pos;
    try
    {
        value = std::stoll(str, &pos, 10);
        if (pos != str.length()) return false;
    } catch (...)
    {
        return false;
    }
    return true;
}

void urldecode(char *dst, const char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b)))
        {
            if (a >= 'a')
                a -= 'A'-'a';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'A'-'a';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst = 16*a+b;
            ++dst;
            src+=3;
        } else {
            if (*src == '+')
                *dst = ' ';
            else
                *dst = *src;
            ++dst;
            ++src;
        }
    }
    // Be sure to close string
    *dst++ = '\0';
}

void jsonescape(std::string const &str, std::ostream &out)
{
    unsigned length = 0;
    char escaped[str.length()*2]; // Escaped string can't be longer
    for (char c : str)
    {
        if (c == '"')
        {
            escaped[length] = '\\';
            ++length;
        }
        escaped[length] = c;
        ++length;
    }
    out.write(escaped, length);
}

void htmlspecialchars(std::string const &str, std::ostream &out)
{
    static const char quot[] = "&quot;";
    static const char apos[] = "&apos;";
    static const char amp[] = "&amp;";
    static const char lt[] = "&lt;";
    static const char gt[] = "&gt;";
    unsigned length = 0;
    char escaped[str.length()*5]; // Escaped string can't be longer
    for (char c : str)
    {
        switch (c)
        {
        case '"':
            memcpy(escaped+length, quot, sizeof(quot)-1);
            length += sizeof(quot)-1;
            break;
        case '\'':
            memcpy(escaped+length, apos, sizeof(apos)-1);
            length += sizeof(apos)-1;
            break;
        case '&':
            memcpy(escaped+length, amp, sizeof(amp)-1);
            length += sizeof(amp)-1;
            break;
        case '<':
            memcpy(escaped+length, lt, sizeof(lt)-1);
            length += sizeof(lt)-1;
            break;
        case '>':
            memcpy(escaped+length, gt, sizeof(gt)-1);
            length += sizeof(gt)-1;
            break;
        default:
            escaped[length] = c;
            ++length;
        }
    }
    out.write(escaped, length);
}
