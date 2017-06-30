#include <tokenizer.hpp>

#include <iostream>


int main()
{
    auto program = "  x =23*  16+foo() / bar ( )    ";
//    auto program = "  a bc  def";

    Tokenizer tokenizer;

    tokenizer.tokenize(program, [](std::shared_ptr<Token> token) {
        std::cout << "[" << token->typeName() << " " << token->toString() << "]" << std::endl;
    });
}
