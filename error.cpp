#include "error.hpp"

Error::Error(): std::runtime_error("Error") {}

Error::Error(const std::string & what)
    : std::runtime_error(what)
{

}
