/**
 * @author Bruno Pena Baêta (696997)
 * @author Felipe Nepomuceno Coelho (689661)
 */

#include <set>
#include <map>
#include <utility>
#include <iostream>
#include "dfa.cpp"

typedef std::set<state> superState;
typedef std::pair<std::set<state>, std::string> superTransition;

/**
 * @brief A class representing what we called a Super Deterministic Finite Automaton. Basically, it is a SuperDFA that can have a set of states as states, what we called super states.
 */
class SuperDFA {
private:
    std::set<superState> states;
    std::set<std::string> alphabet;
    std::map<superTransition, superState> transitions;
    superState initial_state;
    std::set<superState> final_states;

public:
    // Constructors
    SuperDFA() {
        this->states = std::set<superState>();
        this->alphabet = std::set<std::string>();
        this->transitions = std::map<superTransition, superState>();
        this->initial_state = superState();
        this->final_states = std::set<superState>();
    }

    SuperDFA(std::set<superState> states,
        std::set<std::string> alphabet,
        std::map<superTransition, superState> transitions,
        superState initial_state,
        std::set<superState> final_states) {
        this->states = states;
        this->alphabet = alphabet;
        this->transitions = transitions;
        this->initial_state = initial_state;
        this->final_states = final_states;
    }

    // SuperDFA Creation
    /**
     * @brief Adds a super state to the SuperDFA
     * 
     * @param s The super state to be added
     */
    void addState(superState s) {
        this->states.insert(s);
    }

    /**
     * @brief Adds a symbol to the SuperDFA's alphabet
     * 
     * @param symbol The symbol to be added
     */
    void addSymbol(std::string symbol) {
        this->alphabet.insert(symbol);
    }

    /**
     * @brief Adds a super transition to the SuperDFA
     * 
     * @param from The super state from which the super transition starts
     * @param read The symbol that triggers the super transition
     * @param to The super state to which the transition goes
     */
    void addTransition(superState from, std::string read, superState to) {
        this->transitions[std::make_pair(from, read)] = to;
    }

    /**
     * @brief Sets the SuperDFA's initial super state
     * 
     * @param s The super state to be set as initial
     */
    void setInitialState(superState s) {
        this->initial_state = s;
    }

    /**
     * @brief Adds a final super state to the SuperDFA
     * 
     * @param s The super state to be added as final
     */
    void addFinalState(superState s) {
        this->final_states.insert(s);
    }

    // SuperDFA Information
    /**
     * @brief Checks if a super state is a final super state
     * 
     * @param s The super state to be checked
     * @return true if the super state is a final super state. false otherwise
     */
    bool isFinalState(superState s) {
        return this->final_states.find(s) != this->final_states.end();
    }

    /**
     * @brief Gets the SuperDFA's initial super state
     * 
     * @return The SuperDFA's initial super state
     */
    superState getInitialState() {
        return this->initial_state;
    }

    /**
     * @brief Gets a set of the SuperDFA's final super states
     * 
     * @return The SuperDFA's final states
     */
    std::set<superState> getFinalStates() {
        return this->final_states;
    }

    /**
     * @brief Gets a set of the SuperDFA's non-final super states
     * 
     * @return The SuperDFA's non-final super states
     */
    std::set<superState> getNonFinalStates() {
        std::set<superState> non_final_states;
        for (superState s : this->states) {
            if (!this->isFinalState(s)) {
                non_final_states.insert(s);
            }
        }
        return non_final_states;
    }

    /**
     * @brief Gets the super state to which a super transition goes when a symbol is read by a super state
     * 
     * @param from The super state from which the super transition starts
     * @param read The symbol that triggers the super transition
     * @return The super state to which the super transition goes
     */
    superState transite(superState from, std::string read) {
        return this->transitions[std::make_pair(from, read)];
    }

    /**
     * @brief Gets all the super states of the SuperDFA
     * 
     * @return A set of super states that contains all the super states of the SuperDFA
     */
    std::set<superState> getStates() {
        return this->states;
    }

    /**
     * @brief Gets all the super transitions of the SuperDFA
     * 
     * @return A map that contains all the transitions of the SuperDFA
     */
    std::map<superTransition, superState> getTransitions() {
        return this->transitions;
    }

    /**
     * @brief Gets all the symbols of the SuperDFA's alphabet
     * 
     * @return A set of strings that contains all the symbols of the SuperDFA's alphabet
     */
    std::set<std::string> getAlphabet() {
        return this->alphabet;
    }

    /**
     * @brief Prints the SuperDFA's states to the console
     * 
     */
    void printStates() {
        std::string text = "States: {";
        for (superState s : this->states) {
            text += "{";
            for (state st : s) {
                text += (st + ",");
            }
            text.pop_back();
            text += "},";
        }
        text.pop_back();
        text += "}";
        std::cout << text << std::endl;
    }

    /**
     * @brief Convert the SuperDFA to a DFA
     * 
     * @return The DFA that is equivalent to the SuperDFA
     */
    DFA convertToDfa() {
        DFA dfa = DFA();

        // Setting up alphabet
        for (std::string symbol : this->alphabet) {
            dfa.addSymbol(symbol);
        }

        // Setting up states
        for (superState super_state : this->states) {
            std::string state_name = "";
            for (state s : super_state) {
                state_name += (s + ",");
            }
            state_name.pop_back();
            dfa.addState(state_name);
        }

        // Setting up initial state
        std::string initial_state_name = "";
        for (state s : this->initial_state) {
            initial_state_name += (s + ",");
        }
        initial_state_name.pop_back();
        dfa.setInitialState(initial_state_name);

        // Setting up final states
        for (superState super_state : this->final_states) {
            std::string state_name = "";
            for (state s : super_state) {
                state_name += (s + ",");
            }
            state_name.pop_back();
            dfa.addFinalState(state_name);
        }

        // Setting up transitions
        for (std::pair<superTransition, superState> p : this->transitions) {
            // Getting from state name
            std::string from_state_name = "";
            for (state s : p.first.first) {
                from_state_name += (s + ",");
            }
            from_state_name.pop_back();

            // Getting symbol
            std::string symbol = p.first.second;

            // Getting to state name
            std::string to_state_name = "";
            superState to_super_state = p.second;
            for (state s : to_super_state) {
                to_state_name += (s + ",");
            }
            to_state_name.pop_back();

            dfa.addTransition(from_state_name, symbol, to_state_name);
        }

        return dfa;
    }
};