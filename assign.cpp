#include "assign.hpp"
#include "interpreter.hpp"
#include "token.hpp"


void assign(Stack &stack, Lookup &lookup)
{
    if( stack.size() < 2) throw InterpreterError("= needs two operands");

    auto symbol = stack.pop();

    if( symbol.type() != Token::Type::SYMBOL ) {
        throw InterpreterError("Second operand to '=' must be symbol");
    }

    auto value = stack.pop();
    lookup.set(symbol.toString(), value);
}
