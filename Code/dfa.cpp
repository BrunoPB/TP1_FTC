/**
 * @author Bruno Pena Baêta (696997)
 * @author Felipe Nepomuceno Coelho (689661)
 */

#include <set>
#include <map>
#include <utility>
#include <iostream>
#include "utils.cpp"

typedef std::string state;
typedef std::pair<state, std::string> transition;

/**
 * @brief A class representing a Deterministic Finite Automaton
 */
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
     * @brief Gets a set of the DFA's final states
     * 
     * @return The DFA's final states
     */
    std::set<state> getFinalStates() {
        return this->final_states;
    }

    /**
     * @brief Gets a set of the DFA's non-final states
     * 
     * @return The DFA's non-final states
     */
    std::set<state> getNonFinalStates() {
        std::set<state> non_final_states;
        for (state s : this->states) {
            if (!this->isFinalState(s)) {
                non_final_states.insert(s);
            }
        }
        return non_final_states;
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
     * @return A set of states that contains all the states of the DFA
     */
    std::set<state> getStates() {
        return this->states;
    }

    /**
     * @brief Gets all the transitions of the DFA
     * 
     * @return A map that contains all the transitions of the DFA
     */
    std::map<transition, state> getTransitions() {
        return this->transitions;
    }

    /**
     * @brief Gets all the symbols of the DFA's alphabet
     * 
     * @return A set of strings that contains all the symbols of the DFA's alphabet
     */
    std::set<std::string> getAlphabet() {
        return this->alphabet;
    }

    // DFA Operations
    /**
     * @brief Removes all the unreachable states from the DFA
     */
    void removeUnreachableStates() {
        std::set<state> reachable_states;
        std::set<state> new_reachable_states;
        reachable_states.insert(this->initial_state);
        new_reachable_states.insert(this->initial_state);
        while (new_reachable_states.size() > 0) {
            std::set<state> aux;
            for (state s : new_reachable_states) {
                for (std::string symbol : this->alphabet) {
                    state next_state = this->transite(s, symbol);
                    if (!has(reachable_states,next_state)) {
                        aux.insert(next_state);
                    }
                }
            }
            new_reachable_states = aux;
            reachable_states.insert(new_reachable_states.begin(), new_reachable_states.end());
        }
        std::set<state> unreachable_states;
        for (state s : this->states) {
            if (!has(reachable_states, s)) {
                unreachable_states.insert(s);
            }
        }
        for (state s : unreachable_states) {
            this->states.erase(s);
            for (std::string symbol : this->alphabet) {
                this->transitions.erase(std::make_pair(s, symbol));
            }
        }
    }

    /**
     * @brief Complete the DFA
     */
    void completeAutomaton() {
        int error_state_value = 0;
        for (state s : this->getStates()) {
            try {
                if (std::stoi(s) > error_state_value) {
                    error_state_value = std::stoi(s);
                }
            } catch (std::exception& e) {
                continue;
            }
        }
        error_state_value++;
        state error_state = std::to_string(error_state_value);
        this->addState(error_state);
        for (state s : this->getStates()) {
            for (std::string symbol : this->getAlphabet()) {
                if (this->transitions.find(std::make_pair(s, symbol)) == this->transitions.end()) {
                    this->addTransition(s, symbol, error_state);
                }
            }
        }
    }
};