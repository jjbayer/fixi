#include "tokenizer.hpp"
#include "token.hpp"
#include "parser.hpp"

#include <iostream>


int main()
{
    std::string program;// = "\"Hello world!\" :hello_world = hello_world print";

//    auto program = "  x =23.* 16+foo() / bar41 (\" arg +asdf\\\"23234987 * 13\")\ny=2";
//    auto program = " 1 + .3 - 0.5 + 12.3 + -3 * -.6 / -5.5";
//    auto program = "  a bc  def";

    Tokenizer tokenizer;

    std::cout << ">>> ";
    for (std::string line; std::getline(std::cin, line);) {


//        std::cout << program << std::endl;
        std::vector<std::shared_ptr<Token> > tokens;
        tokenizer.tokenize(line, [&](std::shared_ptr<Token> token) {
            std::cout.flush();
            std::cout << "| " << token->typeName() << " " << token->toString() << " " << std::endl;
            tokens.push_back(token);
        });
        std::cout << ">>> ";

    }

//    std::vector<std::shared_ptr<Token> >::iterator it = tokens.begin();
//    std::vector<std::shared_ptr<Token> >::iterator end = tokens.end();
//    ListBody tree(it, end);

//    std::cout << "*** Print tree: *** " << std::endl;
//    print(tree);

    std::cout << std::endl;

    return 0;
}
