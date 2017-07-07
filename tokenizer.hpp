#pragma once

#include "state.hpp"

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <iostream>

class Token;

class Tokenizer
{
public:
    using Consumer = std::function<void(Token)>;

    void tokenize(const std::string & input, Consumer consumer);
};
