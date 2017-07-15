#include "interpreter.hpp"
#include "token.hpp"
#include "binop.hpp"
#include "assign.hpp"

#include <functional>


using namespace std::placeholders;


Interpreter::Interpreter()
{
    Token::Function plus(std::bind(binop, add<int>, add<float>, _1, _2));
    Token::Function min(std::bind(binop, sub<int>, sub<float>, _1, _2));
    Token::Function star(std::bind(binop, mul<int>, mul<float>, _1, _2));
    Token::Function slash(std::bind(binop, div<int>, div<float>, _1, _2));

    lookup_.set("+", plus);
    lookup_.set("-", min);
    lookup_.set("*", star);
    lookup_.set("/", slash);
    lookup_.set("=", Token::Function(assign));

    lookup_.set("{", Token::Function([&](Stack&, Lookup&) {
        record_.reset(new std::vector<Token>);
    }));
    lookup_.set("}", Token::Function([&](Stack&, Lookup&) {

    }));
}

void Interpreter::push(Token token)
{

    if( record_ != nullptr ) {

//        std::cout << "Record token " << token.toString() << std::endl;
        if( token.type() == Token::Type::NAME && token.toString() == "}") {
            auto copy = record_;
            record_ = nullptr;
            push(Token(copy));
        } else {
            record_->push_back(token);
        }

    } else {
//        std::cout << "Interpret token " << token.toString() << std::endl;

        switch(token.type()) {
        case Token::Type::NAME: {
            try {
                token = lookup_.get(token.toString());
            } catch(std::out_of_range&e) {
                throw UndefinedVariable(token);
            }

            if(token.type() == Token::Type::FUNCTION) {
                if( token.children() ) {
                    for(const auto & t : *token.children() ) {
                        push(t);
                    }
                } else {
                    token.function(stack_, lookup_);
                }
            } else {
                stack_.push(token);
            }

            break;
        }
        default:

            stack_.push(token);
        }
    }
}
