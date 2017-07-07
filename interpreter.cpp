#include "interpreter.hpp"
#include "token.hpp"

//struct Unde

void Interpreter::push(Token token)
{
    switch(token.type()) {
        case Token::Type::NAME: {
            try {
                token = lookup_.at(token.toString());
            } catch(std::out_of_range&e) {
                throw UndefinedVariable(token);
            }

            if(token.type() == Token::Type::FUNCTION) {
                token.function(stack_, lookup_);
            } else {
                stack_.push_back(token);
            }

            break;
        }
    default:

        stack_.push_back(token);
    }
}
