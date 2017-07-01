#include "state.hpp"
#include <sstream>


void runStateMachine(const State &initial, const std::string &input)
{
    const State * state = &initial;
    std::string::const_iterator it = input.begin();
    while(it != input.end()) {

        state = state->enter(it);

    }
}

State::State(const std::__cxx11::string &name, State::Callback cb): name_(name), callback_(cb), fallbackState_(nullptr) {}

void State::addTransition(const std::string &characters, const State &toState, Step step)
{
    if(&toState == this && step == WAIT) {
        throw std::runtime_error("Cannot remain in same state without consuming a character");
    }

    for(char c : characters) {
        transitions_.emplace(c, Transition {&toState, step});
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

        if(transition.step & FORWARD) {
            it++;
        }

        callback_(*transition.next, c, transition.step);

        return transition.next;

    } else if(fallbackState_) {

        callback_(*fallbackState_, 0, WAIT | FLUSH);

        return fallbackState_;

    } else {
        std::stringstream msg;
        msg << "Unexpected character '" << c << "'";
        throw std::runtime_error(msg.str());
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
