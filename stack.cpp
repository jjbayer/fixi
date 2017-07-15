#include "stack.hpp"

#include "error.hpp"


void Stack::push(Token t)
{
    data_.push_back(t);
}

Token Stack::pop()
{
    if( data_.empty() ) {
        throw EmptyStack();
    }

    Token top = *data_.rbegin();
    data_.pop_back();

    return top;
}
