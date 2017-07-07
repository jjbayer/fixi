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

    State neutral("NEUTRAL", stateCallback);
//    State expectUnary("EXPECT_UNARY", stateCallback);
//    State expectBinary("EXPECT_BINARY", stateCallback);
    State parseName("PARSE_NAME", stateCallback);
    State parseSymbol("PARSE_SYMBOL", stateCallback);
    State parseInteger("PARSE_INTEGER", stateCallback);
    State parseMinus("PARSE_MINUS", stateCallback);
    State parseFloat("PARSE_FLOAT", stateCallback);
//    State parseOperator("PARSE_OPERATOR", stateCallback);
    State parseString("PARSE_STRING", stateCallback);
    State escape("ESCAPE", stateCallback);

    neutral.addTransition(" ", neutral, State::FORWARD);
    neutral.addTransition(range('A', 'z'+1), parseName, State::WAIT);
    neutral.addTransition("*+<>=^", parseName, State::RECORD);
    neutral.addTransition(":", parseSymbol, State::FORWARD);
    neutral.addTransition("-", parseMinus, State::RECORD);
    neutral.addTransition(range('0', '9'+1), parseInteger, State::WAIT);
    neutral.addTransition(".", parseFloat, State::RECORD);
//    expectUnary.addTransition("()", parseOperator, State::WAIT);
    neutral.addTransition("\"", parseString, State::FORWARD | State::FLUSH);

    parseName.addTransition(range('A', 'z'+1) + "" + range('0', '9'+1), parseName, State::RECORD);
    parseName.addDefaultTransition(neutral);

    parseSymbol.addTransition(range('A', 'z'+1), parseSymbol, State::RECORD);
    parseSymbol.addDefaultTransition(neutral);

    parseInteger.addTransition(range('0', '9'+1), parseInteger, State::RECORD);
    parseInteger.addTransition(".", parseFloat, State::RECORD);
    parseInteger.addDefaultTransition(neutral);

    parseMinus.addTransition(" ", parseName, State::WAIT);
    parseMinus.addTransition(range('0', '9'+1), parseInteger, State::RECORD);
//    parseMinus.addDefaultTransition(neutral);

    parseFloat.addTransition(range('0', '9'+1), parseFloat, State::RECORD);
    parseFloat.addDefaultTransition(neutral);

    static const std::string PrintableChars = range(' ', '~'+1);
    parseString.addTransition(PrintableChars, parseString, State::RECORD);
    parseString.addTransition("\"", neutral, State::FORWARD | State::FLUSH);
    // TODO: Cannot end with open string, open list, etc.
    parseString.addTransition("\\", escape, State::FORWARD);

    escape.addTransition("\"", parseString, State::RECORD);

//    parseOperator.addTransition("=()+-*/,\n", expectBinary, State::RECORD | State::FLUSH);

    // Set up states
    tokenTypes[&parseName] = Token::Type::NAME;
    tokenTypes[&parseSymbol] = Token::Type::SYMBOL;
    tokenTypes[&parseInteger] = Token::Type::INTEGER;
    tokenTypes[&parseFloat] = Token::Type::FLOAT;
    tokenTypes[&parseString] = Token::Type::STRING;
//    tokenTypes[&parseOperator] = Token::Type::OPERATOR;

    runStateMachine(neutral, input);

    State finalState("FINAL", stateCallback);
    stateCallback(finalState, 0, State::FLUSH);
}
