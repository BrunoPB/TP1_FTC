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
    /**
     * @brief Adds a state to the DFA
     * 
     * @param s The state to be added
     */
    void addState(state s) {
        this->states.insert(s);
    }

    /**
     * @brief Adds a symbol to the DFA's alphabet
     * 
     * @param symbol The symbol to be added
     */
    void addSymbol(std::string symbol) {
        this->alphabet.insert(symbol);
    }

    /**
     * @brief Adds a transition to the DFA
     * 
     * @param from The state from which the transition starts
     * @param read The symbol that triggers the transition
     * @param to The state to which the transition goes
     */
    void addTransition(state from, std::string read, state to) {
        this->transitions[std::make_pair(from, read)] = to;
    }

    /**
     * @brief Sets the DFA's initial state
     * 
     * @param s The state to be set as initial
     */
    void setInitialState(state s) {
        this->initial_state = s;
    }

    /**
     * @brief Adds a final state to the DFA
     * 
     * @param s The state to be added as final
     */
    void addFinalState(state s) {
        this->final_states.insert(s);
    }

    // DFA Information
    /**
     * @brief Checks if a state is a final state
     * 
     * @param s The state to be checked
     * @return true if the state is a final state. false otherwise
     */
    bool isFinalState(state s) {
        return this->final_states.find(s) != this->final_states.end();
    }

    /**
     * @brief Gets the DFA's initial state
     * 
     * @return The DFA's initial state
     */
    state getInitialState() {
        return this->initial_state;
    }

    /**
     * @brief Gets the state to which a transition goes when a symbol is read by a state
     * 
     * @param from The state from which the transition starts
     * @param read The symbol that triggers the transition
     * @return The state to which the transition goes
     */
    state transite(state from, std::string read) {
        return this->transitions[std::make_pair(from, read)];
    }

    /**
     * @brief Gets all the states of the DFA
     * 
     * @return std::set<state> that contains all the states of the DFA
     */
    std::set<state> getStates() {
        return this->states;
    }

    /**
     * @brief Gets all the transitions of the DFA
     * 
     * @return std::map<transition, state> that contains all the transitions of the DFA
     */
    std::map<transition, state> getTransitions() {
        return this->transitions;
    }
};