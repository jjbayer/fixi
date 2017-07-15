#pragma once
#include "error.hpp"
#include "token.hpp"

#include <unordered_map>


struct LookupError: public Error
{
    LookupError(const std::string & what);
};


class Lookup
{
public:

    const Token & get(const std::string & name) const;

    void set(const std::string & name, const Token & token);

private:
    std::unordered_map<std::string, Token> data_;
};
