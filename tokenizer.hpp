#pragma once

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>
#include <iostream>


class State
{
public:

    using Callback = std::function<void(const State &, char)>;

    State(const std::string & name, Callback cb): name_(name), callback_(cb), fallbackState_(nullptr) {}

    const std::string & name() const { return name_; }

    void addTransition(char begin, char end, const State & toState, bool eat)
    {
        if(&toState == this && eat == false) {
            throw std::runtime_error("Cannot remain in same state without consuming a character");
        }

        for(char c = begin; c <= end; c++) {
            std::cout << "Emplace for " << c <<std::endl;
            transitions_.emplace(c, Transition {&toState, eat});
        }
    }

    void addDefaultTransition(const State & toState)
    {
        fallbackState_ = &toState;
    }

    const State * enter(std::string::const_iterator & it) const
    {
        char c = *it;


        if( transitions_.count(c) ) {
            const auto & transition = transitions_.at(c);

            if(transition.eat) {
                it++;
            } else {
                c = 0;
            }

            callback_(*transition.next, c);
            
            return transition.next;

        } else if(fallbackState_) {

            callback_(*fallbackState_, 0);

            return fallbackState_;

        } else {
            std::string msg = "Unexpected character ";
            msg += c;
            throw std::runtime_error(msg);
        }
    }

private:

    struct Transition {
        const State * next;
        bool eat;
    };

    std::string name_;
    Callback callback_;
    std::unordered_map<char, Transition> transitions_;
    const State * fallbackState_;
};


void runStateMachine(const State & initial, const std::string & input)
{
    const State * state = &initial;
    std::string::const_iterator it = input.begin();
    while(it != input.end()) {

        state = state->enter(it);

    }
}


class Token
{
public:
    enum class Type {
        STRING
    };

    const char * typeName() { return "STRING"; }
    std::string toString() { return string; }

private:

    Type type;
    std::string string;
};


class Tokenizer
{
public:
    using Consumer = std::function<void(std::shared_ptr<Token>)>;

    void tokenize(const std::string & input, Consumer consumer)
    {
        std::string buffer;
        const State * prevState = nullptr;
        auto stateCallback = [&](const State & state, char c) {

            //std::cout << "State " << state.name() << " with '" << c << "'" << std::endl;
            if(prevState && &state != prevState && ! buffer.empty()) {
                std::cout << "Clear buffer '" << buffer << "'" << std::endl;
                buffer.clear();
            } else if(c != ' ' && c != 0) {

                buffer += c;
            }


            prevState = &state;
        };

        State initial("EXPECT_BINARY", stateCallback);
        State stateName("PARSE_NAME", stateCallback);
        initial.addTransition(' ', ' ', initial, true);
        initial.addTransition('A', 'z', stateName, false);

        stateName.addTransition('A', 'z', stateName, true);
        stateName.addDefaultTransition(initial);

        runStateMachine(initial, input);

        State finalState("FINAL", stateCallback);
        stateCallback(finalState, 0);
    }
};


//class StateMachine
//{
//public:

//    StateMachine(State * initial);

//    void setInitialState(State * state)
//    {

//    }

//    void run(const std::string & input);
//};
