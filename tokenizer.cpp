#include "tokenizer.hpp"
#include "token.hpp"



void Tokenizer::tokenize(const std::string &input, Tokenizer::Consumer consumer)
{
    std::string buffer;
    const State * prevState = nullptr;

    std::unordered_map<const State *, Token::Type> tokenTypes;

    auto stateCallback = [&](const State & state, char c, State::Step step) {

//        std::cout << "Go from " << (prevState ? prevState->name() : "0") << " to "
//                  << state.name() << " with c = '" << c << "' and step = " << step << std::endl;

        if( (step & 3) == State::RECORD ) {
//            std::cout << "Recording '" << c << "' because step = " << step << std::endl;
            buffer += c;
        }

        //std::cout << "State " << state.name() << " with '" << c << "'" << std::endl;
        if( step & State::FLUSH ) {
//            std::cout << "trans " << prevState->name() << " " << state.name() << std::endl;

//            std::cout << "Flushing '" << buffer << "'" << std::endl;
            if(tokenTypes.count(prevState)) {
//                std::cout << "Consume " << prevState->name() << std::endl;
                consumer(std::make_shared<Token>(tokenTypes.at(prevState), buffer));
            }
//            std::cout << "Clear buffer '" << buffer << "'" << std::endl;
            buffer.clear();
        }


        prevState = &state;
    };

    State expectUnary("EXPECT_UNARY", stateCallback);
    State expectBinary("EXPECT_BINARY", stateCallback);
    State parseName("PARSE_NAME", stateCallback);
    State parseInteger("PARSE_INTEGER", stateCallback);
    State parseFloat("PARSE_FLOAT", stateCallback);
    State parseOperator("PARSE_OPERATOR", stateCallback);
    State parseString("PARSE_STRING", stateCallback);
    State escape("ESCAPE", stateCallback);

    expectUnary.addTransition(" ", expectUnary, State::FORWARD);
    expectUnary.addTransition(range('A', 'z'+1), parseName, State::WAIT);
    expectUnary.addTransition("-", parseInteger, State::RECORD);
    expectUnary.addTransition(range('0', '9'+1), parseInteger, State::WAIT);
    expectUnary.addTransition(".", parseFloat, State::RECORD);
    expectUnary.addTransition("()", parseOperator, State::WAIT);
    expectUnary.addTransition("\"", parseString, State::FORWARD | State::FLUSH);


    expectBinary.addTransition(" ", expectBinary, State::FORWARD);
    expectBinary.addTransition(range('A', 'z'+1), parseName, State::WAIT);
    expectBinary.addTransition("=()+-*/,\n", parseOperator, State::WAIT);
    expectBinary.addTransition(range('0', '9'+1), parseInteger, State::WAIT);
    expectBinary.addTransition("\"", parseString, State::FORWARD | State::FLUSH);

    parseName.addTransition(range('A', 'z'+1) + "" + range('0', '9'+1), parseName, State::RECORD);

    parseName.addDefaultTransition(expectBinary);

    parseInteger.addTransition(range('0', '9'+1), parseInteger, State::RECORD);
    parseInteger.addTransition(".", parseFloat, State::RECORD);
    parseInteger.addDefaultTransition(expectBinary);

    parseFloat.addTransition(range('0', '9'+1), parseFloat, State::RECORD);
    parseFloat.addDefaultTransition(expectBinary);

    static const std::string PrintableChars = range(' ', '~'+1);
    parseString.addTransition(PrintableChars, parseString, State::RECORD);
    parseString.addTransition("\"", expectBinary, State::FORWARD | State::FLUSH);
    // TODO: Cannot end with open string, open list, etc.
    parseString.addTransition("\\", escape, State::FORWARD);

    escape.addTransition("\"", parseString, State::RECORD);

    parseOperator.addTransition("=()+-*/,\n", expectBinary, State::RECORD | State::FLUSH);

    // Set up states
    tokenTypes[&parseName] = Token::Type::NAME;
    tokenTypes[&parseInteger] = Token::Type::INTEGER;
    tokenTypes[&parseFloat] = Token::Type::FLOAT;
    tokenTypes[&parseString] = Token::Type::STRING;
    tokenTypes[&parseOperator] = Token::Type::OPERATOR;

    runStateMachine(expectUnary, input);

    State finalState("FINAL", stateCallback);
    stateCallback(finalState, 0, State::FLUSH);
}
