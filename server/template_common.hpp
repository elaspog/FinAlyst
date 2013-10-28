#ifndef TEMPLATE_COMMON_HPP
#define TEMPLATE_COMMON_HPP

#include <iostream>
#include "request.hpp"

void html_content(std::ostream &fcout);
void header(std::ostream &fcout,
        OptsMap const &assetpath, std::string const &title);
void footer(std::ostream &fcout);
void print_environment(std::ostream &fcout, Request const &request);
void static_page(std::string const &filename,
        OptsMap const &config, std::ostream &fcout);

#endif
