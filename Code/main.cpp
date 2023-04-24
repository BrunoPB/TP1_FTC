/**
 * @author Bruno Pena Baêta (696997)
 * @author Felipe Nepomuceno Coelho (689661)
 */

#include "dfa.cpp"
#include <iostream>
#include "pugixml/pugixml.hpp"
#include "utils.cpp"

// #define BASE_PATH "./../" // Debug path
#define BASE_PATH "./../../" // Execution path

DFA loadDfaFromFile(bool* dfaNullFlag);
void exportDfaToFile(DFA dfa);
void runOn2Algorithm();
void runOnLogNAlgorithm();

int main()
{
    DFA dfa = DFA();
    bool dfaNullFlag = true;
    bool quit = false;

    while (!quit)
    {
        std::cout << "MENU:\n1. Load DFA file\n2. Export DFA\n3. Run O(n^2) Algorithm\n4. Run O(n log n) Algorithm\n0. Quit\nChoose option: ";
        int option;
        std::cin >> option;
        switch (option)
        {
        case 1:
            dfa = loadDfaFromFile(&dfaNullFlag);
            break;
        case 2:
            if (dfaNullFlag) {
                std::cout << "\nNo DFA loaded yet.\n\n";
                break;
            }
            exportDfaToFile(dfa);
            break;
        case 3:
            if (dfaNullFlag) {
                std::cout << "\nNo DFA loaded yet.\n\n";
                break;
            }
            runOn2Algorithm();
            break;
        case 4:
            if (dfaNullFlag) {
                std::cout << "\nNo DFA loaded yet.\n\n";
                break;
            }
            runOnLogNAlgorithm();
            break;
        default:
            quit = true;
            break;
        }
    }

    return 0;
}

DFA loadDfaFromFile(bool* dfaNullFlag) {
    std::cout << "File name to load: ";
    std::string file_name;
    std::cin >> file_name;

    std::cout << "Loading file...\n";

    std::string s_base_path = BASE_PATH;
    std::string file_path = s_base_path + "Data/" + file_name;

    if (!existsFile(file_path)) {
        std::cout << "\nFile not found.\n\n";
        *dfaNullFlag = true;
        return DFA();
    }

    pugi::xml_document file;
    pugi::xml_parse_result result = file.load_file(file_path.c_str());

    if (!result) {
        std::cout << "\nError loading file.\n\n";
        *dfaNullFlag = true;
        return DFA();
    }
    
    std::cout << "File loaded successfully.\n";

    std::cout << "Setting up DFA...\n";

    DFA dfa = DFA();
    pugi::xml_node automaton = file.child("structure").child("automaton");

    // Setting up states
    for (pugi::xml_node node = automaton.child("state"); node; node = node.next_sibling("state")) {
        std::string id = node.attribute("id").value();
        dfa.addState(id);
        if (node.child("initial")) {
            dfa.setInitialState(id);
        }
        if (node.child("final")) {
            dfa.addFinalState(id);
        }
    }

    // Setting up transitions
    for (pugi::xml_node node = automaton.child("transition"); node; node = node.next_sibling("transition")) {
        std::string symbol = node.child_value("read");
        dfa.addSymbol(symbol);
        dfa.addTransition(node.child_value("from"), symbol, node.child_value("to"));
    }

    std::cout << "DFA successfully setted up.\n\n";

    *dfaNullFlag = false;
    return dfa;
}

void exportDfaToFile(DFA dfa) {
    std::cout << "File name to export: ";
    std::string file_name;
    std::cin >> file_name;

    std::string s_base_path = BASE_PATH;
    std::string file_path = s_base_path + "Output/" + file_name;

    if (existsFile(file_path)) {
        std::cout << "\nFile already exists.\n\n";
        return;
    }

    std::cout << "Loading skeleton file...\n";

    std::string skeleton_path = s_base_path + "Data/skeleton.jff";
    if (!existsFile(skeleton_path)) {
        std::cout << "\nSkeleton file not found. Please recreate it.\n\n";
        return;
    }
    pugi::xml_document skeleton;
    pugi::xml_parse_result result = skeleton.load_file(skeleton_path.c_str());
    if (!result) {
        std::cout << "\nError loading skeleton file. Please recreate it.\n\n";
        return;
    }

    std::cout << "Exporting DFA...\n";
    
    pugi::xml_node automaton = skeleton.child("structure").child("automaton");

    // Setting up states
    for (state s : dfa.getStates()) {
        pugi::xml_node state = automaton.append_child("state");
        state.append_attribute("id") = s.c_str();
        state.append_attribute("name") = ("q" + s).c_str();
        state.append_child("x").append_child(pugi::node_pcdata).set_value("0");
        state.append_child("y").append_child(pugi::node_pcdata).set_value("0");
        if (s == dfa.getInitialState()) {
            state.append_child("initial");
        }
        if (dfa.isFinalState(s)) {
            state.append_child("final");
        }
    }

    // Setting up transitions
    for (std::pair<const transition, state> const& it : dfa.getTransitions()) {
        pugi::xml_node transition = automaton.append_child("transition");
        const pugi::char_t* state1 = it.first.first.c_str();
        const pugi::char_t* state2 = it.second.c_str();
        const pugi::char_t* symbol = it.first.second.c_str();
        transition.append_child("from").append_child(pugi::node_pcdata).set_value(state1);
        transition.append_child("to").append_child(pugi::node_pcdata).set_value(state2);
        transition.append_child("read").append_child(pugi::node_pcdata).set_value(symbol);
    }

    skeleton.save_file(file_path.c_str());

    std::cout << "\nDFA successfully exported to " + file_path + ".\n\n";
}

void runOn2Algorithm() {
    std::cout << "Running O(n^2) Algorithm\n";
}

void runOnLogNAlgorithm() {
    std::cout << "Running O(n log n) Algorithm\n";
}