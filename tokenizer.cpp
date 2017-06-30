#include "tokenizer.hpp"
#include "token.hpp"



void Tokenizer::tokenize(const std::string &input, Tokenizer::Consumer consumer)
{
    std::string buffer;
    const State * prevState = nullptr;

    std::unordered_map<const State *, Token::Type> tokenTypes;

    auto stateCallback = [&](const State & state, char c) {

        if(c != ' ' && c != 0) {

            buffer += c;
        }

        //std::cout << "State " << state.name() << " with '" << c << "'" << std::endl;
        if(prevState && &state != prevState ) {
//            std::cout << "trans " << prevState->name() << " " << state.name() << std::endl;

            if(tokenTypes.count(prevState)) {
//                std::cout << "Consume " << prevState->name() << std::endl;
                consumer(std::make_shared<Token>(tokenTypes.at(prevState), buffer));
            }
//            std::cout << "Clear buffer '" << buffer << "'" << std::endl;
            buffer.clear();
        }


        prevState = &state;
    };

    State initial("EXPECT_BINARY", stateCallback);
    State stateName("PARSE_NAME", stateCallback);
    State parseInteger("PARSE_INTEGER", stateCallback);
    State parseOperator("PARSE_OPERATOR", stateCallback);

    initial.addTransition(" ", initial, true);
    initial.addTransition(range('A', 'z'+1), stateName, false);
    initial.addTransition("=()+-*/", parseOperator, false);
    initial.addTransition(range('0', '9'+1), parseInteger, false);

    stateName.addTransition(range('A', 'z'+1), stateName, true);
    stateName.addDefaultTransition(initial);

    parseInteger.addTransition(range('0', '9'+1), parseInteger, true);
    parseInteger.addDefaultTransition(initial);

    parseOperator.addTransition("=()+-*/", initial, true);

    // Set up states
    tokenTypes[&stateName] = Token::Type::NAME;
    tokenTypes[&parseInteger] = Token::Type::INTEGER;
    tokenTypes[&parseOperator] = Token::Type::OPERATOR;

    runStateMachine(initial, input);

    State finalState("FINAL", stateCallback);
    stateCallback(finalState, 0);
}
