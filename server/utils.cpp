#include "utils.hpp"
#include <algorithm>

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

