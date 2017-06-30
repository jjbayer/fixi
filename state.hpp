#pragma once
#include <functional>
#include <unordered_map>

class State
{
public:

    using Callback = std::function<void(const State &, char)>;

    State(const std::string & name, Callback cb): name_(name), callback_(cb), fallbackState_(nullptr) {}

    const std::string & name() const { return name_; }

    /// For every character in string, add a transition to state toState
    void addTransition(const std::string & characters, const State & toState, bool eat);

    void addDefaultTransition(const State & toState);

    const State * enter(std::string::const_iterator & it) const;

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

std::string range(char begin, char end);

void runStateMachine(const State & initial, const std::string & input);

