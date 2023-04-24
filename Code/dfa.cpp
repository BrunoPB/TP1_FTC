/**
 * @author Bruno Pena Baêta (696997)
 * @author Felipe Nepomuceno Coelho (689661)
 */

#include <set>
#include <map>
#include <iostream>

typedef std::string state;
typedef std::pair<state, std::string> transition;

class DFA {
private:
    std::set<state> states;
    std::set<std::string> alphabet;
    std::map<transition, state> transitions;
    state initial_state;
    std::set<state> final_states;

public:

    // Constructors
    DFA() {
        this->states = std::set<state>();
        this->alphabet = std::set<std::string>();
        this->transitions = std::map<transition, state>();
        this->initial_state = "";
        this->final_states = std::set<state>();
    }

    DFA(std::set<state> states,
        std::set<std::string> alphabet,
        std::map<transition, state> transitions,
        state initial_state,
        std::set<state> final_states) {
        this->states = states;
        this->alphabet = alphabet;
        this->transitions = transitions;
        this->initial_state = initial_state;
        this->final_states = final_states;
    }

    // DFA Creation
    void addState(state s) {
        this->states.insert(s);
    }

    void addSymbol(std::string symbol) {
        this->alphabet.insert(symbol);
    }

    void addTransition(state from, std::string read, state to) {
        this->transitions[std::make_pair(from, read)] = to;
    }

    void setInitialState(state s) {
        this->initial_state = s;
    }

    void addFinalState(state s) {
        this->final_states.insert(s);
    }

    // DFA Information
    bool isFinalState(state s) {
        return this->final_states.find(s) != this->final_states.end();
    }

    state getInitialState() {
        return this->initial_state;
    }

    state transite(state from, std::string read) {
        return this->transitions[std::make_pair(from, read)];
    }

    std::set<state> getStates() {
        return this->states;
    }

    std::map<transition, state> getTransitions() {
        return this->transitions;
    }
};