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

        if(step >= State::RECORD) {
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

    State initial("EXPECT_BINARY", stateCallback);
    State stateName("PARSE_NAME", stateCallback);
    State parseInteger("PARSE_INTEGER", stateCallback);
    State parseOperator("PARSE_OPERATOR", stateCallback);

    initial.addTransition(" ", initial, State::FORWARD);
    initial.addTransition(range('A', 'z'+1), stateName, State::WAIT);
    initial.addTransition("=()+-*/", parseOperator, State::WAIT);
    initial.addTransition(range('0', '9'+1), parseInteger, State::WAIT);

    stateName.addTransition(range('A', 'z'+1), stateName, State::RECORD);
    stateName.addDefaultTransition(initial);

    parseInteger.addTransition(range('0', '9'+1), parseInteger, State::RECORD);
    parseInteger.addDefaultTransition(initial);

    parseOperator.addTransition("=()+-*/", initial, State::RECORD | State::FLUSH);
    parseInteger.addDefaultTransition(initial);

    // Set up states
    tokenTypes[&stateName] = Token::Type::NAME;
    tokenTypes[&parseInteger] = Token::Type::INTEGER;
    tokenTypes[&parseOperator] = Token::Type::OPERATOR;

    runStateMachine(initial, input);

    State finalState("FINAL", stateCallback);
    stateCallback(finalState, 0, State::WAIT);
}
