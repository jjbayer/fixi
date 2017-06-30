#include "tokenizer.hpp"
#include "token.hpp"

#include <iostream>


int main()
{
    auto program = "  x =23*  16+foo() / bar ( )    ";
//    auto program = "  a bc  def";

    Tokenizer tokenizer;

    std::cout << program << std::endl;
    tokenizer.tokenize(program, [](std::shared_ptr<Token> token) {
        std::cout << token->toString() << " " << std::endl;
    });
}
