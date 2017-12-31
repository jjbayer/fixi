#pragma once
#include <stdexcept>


struct Error: public std::runtime_error
{
    Error();
    Error(const std::string & what);
};
