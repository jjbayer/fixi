#include "state.hpp"


void runStateMachine(const State &initial, const std::string &input)
{
    const State * state = &initial;
    std::string::const_iterator it = input.begin();
    while(it != input.end()) {

        state = state->enter(it);

    }
}

void State::addTransition(const std::string &characters, const State &toState, bool eat)
{
    if(&toState == this && eat == false) {
        throw std::runtime_error("Cannot remain in same state without consuming a character");
    }

    for(char c : characters) {
        transitions_.emplace(c, Transition {&toState, eat});
    }
}

void State::addDefaultTransition(const State &toState)
{
    fallbackState_ = &toState;
}

const State * State::enter(std::string::const_iterator &it) const
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

std::string range(char begin, char end)
{
    std::string ret;

    for(char c = begin; c < end; c++) {
        ret += c;
    }

    return ret;
}
