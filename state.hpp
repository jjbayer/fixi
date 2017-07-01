#pragma once
#include <functional>
#include <unordered_map>

class State
{
public:

    /// flush bit - record bit - increment bit
    static const int WAIT = 0;
    static const int FORWARD = 1;
    static const int RECORD = 3;
    static const int FLUSH = 4;
    using Step = int;


    using Callback = std::function<void(const State &, char, Step)>;

    State(const std::string & name, Callback cb);

    const std::string & name() const { return name_; }

    /// For every character in string, add a transition to state toState
    void addTransition(const std::string & characters,
                       const State & toState,
                       Step step);

    void addDefaultTransition(const State & toState);

    const State * enter(std::string::const_iterator & it) const;

private:

    struct Transition {
        const State * next;
        State::Step step;
    };

    std::string name_;
    Callback callback_;
    std::unordered_map<char, Transition> transitions_;
    const State * fallbackState_;
};

std::string range(char begin, char end);

void runStateMachine(const State & initial, const std::string & input);

