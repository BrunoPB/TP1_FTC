/**
 * @author Bruno Pena Baêta (696997)
 * @author Felipe Nepomuceno Coelho (689661)
 */

#include "superDfa.cpp"

bool blumWhileCondition(DFA dfa, int t, std::set<std::string> alphabet, superState Q[1024]) {
    for (int i = 1; i <= t; i++) {
        for (std::string a : alphabet) {
            for (state q : Q[i]) {
                state transitionState = dfa.transite(q,a);
                bool containElement = false;
                for (int j = i+1; j <= t; j++) {
                    if (has(Q[j],transitionState)) { // δ(Q[i],a) ∩ Q[j] ≠ ∅
                        containElement = true;
                    }
                }
                if (containElement) {
                    return true;
                }
            }
        }
    }
    return false;
}

DFA blumOn2Algorithm(DFA dfa) {
    std::cout << "Preparing to run the O(n^2) algorithm...\n";    

    std::cout << "Removing unreachable states...\n";
    dfa.removeUnreachableStates();
    std::cout << "Unreachable states successfully removed.\n";

    // Initialization
    int t = 2;
    superState Q[1024];
    Q[0] = superState();
    Q[1] = dfa.getFinalStates();
    Q[2] = dfa.getNonFinalStates();
    int i = 1;
    int j = 1;

    std::cout << "Running the O(n^2) algorithm...\n";
    
    while(blumWhileCondition(dfa,t,dfa.getAlphabet(),Q)) {
        bool breakOn = false;
        for (i = 1; i <= t; i++) {
            for (std::string a : dfa.getAlphabet()) {
                for (j = 1; j <= t; j++) {
                    for (state q : Q[i]) {
                        state transitionState = dfa.transite(q,a);
                        if (has(Q[j],transitionState)) { // δ(Q[i],a) ∩ Q[j] ≠ ∅
                            Q[t+1].insert(q); // Q[t+1] = {q ϵ Q[i] | δ(q,a) ϵ Q[j]}
                            Q[i] = getSetDifference(Q[i],Q[t+1]); // Q[i] = Q[i] - Q[t+1]
                            t++; // t = t + 1
                            breakOn = true;
                        }
                    }
                    if (breakOn) {
                        break;
                    }
                }
                if (breakOn) {
                    break;
                }
            }
            if (breakOn) {
                break;
            }
        }
    }

    // Creating the new DFA from a SuperDFA
    SuperDFA superDfa = SuperDFA();

    // Setting up alphabet (Sigma')
    for (std::string symbol : dfa.getAlphabet()) {
        superDfa.addSymbol(symbol);
    }

    // Setting up states (Q')
    for (i = 1; i <= t; i++) {
        if (Q[i].size() > 0) {
            superDfa.addState(Q[i]);
        }
    }

    // Setting up initial state (q0)
    for (auto ss : superDfa.getStates()) {
        if (has(ss,dfa.getInitialState())) {
            superDfa.setInitialState(ss);
            break;
        }
    }

    // Setting up final states (F')
    for (auto ss : superDfa.getStates()) {
        for (state s : dfa.getFinalStates()) {
            if (has(ss,s)) {
                superDfa.addFinalState(ss);
                break;
            }
        }
    }

    // Setting up transitions (delta')
    for(state q : dfa.getStates()) { // For all q ϵ Q
        for (std::string a : dfa.getAlphabet()) { // For all a ϵ 𝛴
            state transitionState = dfa.transite(q,a);
            for (superState ss : superDfa.getStates()) { // For all ss ϵ Q'
                if (has(ss,q)) { // If q ϵ ss
                    for (superState sss : superDfa.getStates()) { // For all sss ϵ Q'
                        if (has(sss,transitionState)) { // If transitionState ϵ sss
                            superDfa.addTransition(ss,a,sss);
                        }
                    }
                }
            }
        }
    }

    std::cout << "O(n^2) algorithm successfully executed.\n";
    std::cout << "Minimizing the DFA...\n";

    // Converting the SuperDFA to a DFA
    DFA newDfa = superDfa.convertToDfa();

    std::cout << "DFA successfully minimized!\n\n";

    return newDfa;
}

DFA newtonOn2Algorithm(DFA dfa) {
    // Initialization
    std::cout << "Preparing to run the O(n^2) algorithm...\n";

    std::cout << "Removing unreachable states...\n";
    dfa.removeUnreachableStates();
    std::cout << "Unreachable states successfully removed.\n";

    if (dfa.getFinalStates().size() <= 0) { // Se F = ∅
        return dfa; 
    } else if (dfa.getNonFinalStates().size() <= 0) { // Se E - F = ∅
        return dfa;
    }

    std::cout << "Running the O(n^2) algorithm...\n";
    // Algorithm
    int n = 0; // n <- 0
    std::set<superState> S[1024];
    S[0].insert(dfa.getNonFinalStates()); // S[0] <- {E - F}
    S[0].insert(dfa.getFinalStates()); // S[0] <- {E - F, F}
    do { // Repita
        n++; // n <- n + 1
        S[n].clear(); // S[n] <- ∅
        for (superState X : S[n-1]) { // Para cada X ϵ S[n-1] faça
            do { // Repita
                state e = *(X.begin()); // Selecione um estado e ϵ X
                superState Y = superState();
                for (std::string a : dfa.getAlphabet()) { // Para cada a ϵ 𝛴 faça
                    superState t = superState();
                    state transitionState = dfa.transite(e,a);
                    for (superState Z : S[n-1]) { // Para cada conjunto de estado em S[n-1]
                        if (has(Z,transitionState)) { // que contém δ(e,a)
                            t = Z;
                            break;
                        }
                    }
                    for (std::string b : dfa.getAlphabet()) { // Para cada b ϵ 𝛴 faça
                        for (state el : X) { // Para cada elemento e' ϵ X faça
                            state tsl = dfa.transite(el,b);
                            if (has(t,tsl)) { // Se δ(e',b) ϵ [δ(e,b)]
                                Y.insert(el); // Y <- e'
                            }
                        }
                    }
                }
                X = getSetDifference(X,Y); // X <- X - Y
                S[n].insert(Y); // S[n] <- S[n] U {Y}
            } while(X.size() > 0); // Até X = ∅
        }
    } while (S[n] != S[n-1]); // Até S[n] = S[n-1]

    // Creating the new DFA from a SuperDFA
    SuperDFA superDfa = SuperDFA();

    // Setting up alphabet (Sigma')
    for (std::string symbol : dfa.getAlphabet()) {
        superDfa.addSymbol(symbol);
    }

    // Setting up states (E)
    for (superState ss : S[n]) {
        superDfa.addState(ss);
    }

    // Setting up initial state (i')
    for (auto ss : superDfa.getStates()) {
        if (has(ss,dfa.getInitialState())) {
            superDfa.setInitialState(ss);
            break;
        }
    }

    // Setting up final states (F')
    for (auto ss : superDfa.getStates()) {
        for (state s : dfa.getFinalStates()) {
            if (has(ss,s)) {
                superDfa.addFinalState(ss);
            }
        }
    }

    // Setting up transitions (delta')
    for (superState X : S[n]) { // Para cada X ϵ S[n]
        for (std::string a : superDfa.getAlphabet()) { // e a ϵ 𝛴:
            for (state e : X) { // Para qualquer e ϵ X
                state transitionState = dfa.transite(e,a);
                for (superState Y : S[n]) { // Para qualquer conjunto Y em S[n]
                    if (has(Y,transitionState)) { // que contém δ(e,a)
                        superDfa.addTransition(X,a,Y);
                    }
                }
            }
        }
    }

    std::cout << "O(n^2) algorithm successfully executed.\n";

    std::cout << "Minimizing the DFA...\n";

    // Converting the SuperDFA to a DFA
    DFA newDfa = superDfa.convertToDfa();

    std::cout << "DFA successfully minimized!\n\n";

    return newDfa;
}

/**
 * @brief Checks if two states are equivalent in a set Q
 * 
 * @param s1 The first state
 * @param s2 The second state
 * @param dfa The DFA the states are in
 * @param Q The set of super states
 * @return true if the states are equivalent. false otherwise
 */
bool areEquivalent(state s1, state s2, DFA dfa, std::set<superState> Q) {
    if (dfa.isFinalState(s1) && !dfa.isFinalState(s2)) {
        return false;
    } else if (!dfa.isFinalState(s1) && dfa.isFinalState(s2)) {
        return false;
    } else {
        for (std::string symbol : dfa.getAlphabet()) {
            state transitionState1 = dfa.transite(s1,symbol);
            state transitionState2 = dfa.transite(s2,symbol);
            // Checking if the transition states are in the same super state of Q
            for (superState ss : Q) {
                if (has(ss,transitionState1) && !has(ss,transitionState2)) {
                    return false;
                } else if (!has(ss,transitionState1) && has(ss,transitionState2)) {
                    return false;
                } else if (has(ss,transitionState1) && has(ss,transitionState2)) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * @brief Runs an O(n^2) algorithm that minimizes a DFA.
 * 
 * @param dfa The DFA to be minimized
 * @return The minimized DFA
 */
DFA myOn2Algorithm(DFA dfa) {
    // Initialization
    std::cout << "Preparing to run the O(n^2) algorithm...\n";

    std::cout << "Removing unreachable states...\n";
    dfa.removeUnreachableStates();
    std::cout << "Unreachable states successfully removed.\n";

    std::cout << "Running the O(n^2) algorithm...\n";

    // Algorithm
    int equivalence = 0;
    std::set<superState> Q[1024];
    Q[equivalence] = std::set<superState>();
    Q[equivalence].insert(dfa.getNonFinalStates());
    Q[equivalence].insert(dfa.getFinalStates());
    do {
        equivalence++;
        Q[equivalence] = std::set<superState>();
        Q[equivalence].clear();
        for(superState ss : Q[equivalence-1]) {
            // Ignore empty sets
            if (ss.size() == 0) {
                continue;
            }
            // Sets with 1 element
            if (ss.size() == 1) {
                Q[equivalence].insert(ss);
            }
            // Sets with 2 elements or more
            for (int i = 0; i < ss.size()-1; i++) {
                state compare1 = *std::next(ss.begin(), i);
                state compare2 = *std::next(ss.begin(), i+1);
                if (areEquivalent(compare1,compare2,dfa,Q[equivalence-1])) {
                    // Put in same set
                    bool had = false; // Indicates if there was already a set with that element
                    for (superState sss : Q[equivalence]) {
                        if (has(sss,compare1)) {
                            superState newSuperState = sss;
                            Q[equivalence].erase(sss);
                            newSuperState.insert(compare2);
                            Q[equivalence].insert(newSuperState);
                            had = true;
                            break;
                        }
                    }
                    if (!had) {
                        superState newSuperState = superState();
                        newSuperState.insert(compare1);
                        newSuperState.insert(compare2);
                        Q[equivalence].insert(newSuperState);
                    }
                } else {
                    // Put in different sets
                    bool had = false; // Indicates if there was already a set with that element
                    for (superState sss : Q[equivalence]) {
                        if (has(sss,compare1)) {
                            had = true;
                            break;
                        }
                    }
                    if (had) {
                        superState newSuperState = superState();
                        newSuperState.insert(compare2);
                        Q[equivalence].insert(newSuperState);
                    } else {
                        superState newSuperState = superState();
                        superState newSuperState2 = superState();
                        newSuperState.insert(compare1);
                        newSuperState2.insert(compare2);
                        Q[equivalence].insert(newSuperState);
                        Q[equivalence].insert(newSuperState2);
                    }
                }
            }
        }
    } while (Q[equivalence].size() != Q[equivalence-1].size());
    

    // Creating the new DFA from a SuperDFA
    SuperDFA superDfa = SuperDFA();

    // Setting up alphabet (Sigma')
    for (std::string symbol : dfa.getAlphabet()) {
        superDfa.addSymbol(symbol);
    }

    // Setting up states (Q')
    for (superState ss : Q[equivalence]) {
        superDfa.addState(ss);
    }

    // Setting up initial state (i')
    for (auto ss : superDfa.getStates()) {
        if (has(ss,dfa.getInitialState())) {
            superDfa.setInitialState(ss);
            break;
        }
    }

    // Setting up final states (F')
    for (auto ss : superDfa.getStates()) {
        for (state s : dfa.getFinalStates()) {
            if (has(ss,s)) {
                superDfa.addFinalState(ss);
            }
        }
    }

    // Setting up transitions (delta')
    for (superState X : Q[equivalence]) {
        for (std::string a : superDfa.getAlphabet()) {
            for (state e : X) {
                state transitionState = dfa.transite(e,a);
                for (superState Y : Q[equivalence]) {
                    if (has(Y,transitionState)) {
                        superDfa.addTransition(X,a,Y);
                    }
                }
            }
        }
    }

    std::cout << "O(n^2) algorithm successfully executed.\n";

    std::cout << "Minimizing the DFA...\n";

    // Converting the SuperDFA to a DFA
    DFA newDfa = superDfa.convertToDfa();

    std::cout << "DFA successfully minimized!\n\n";

    return newDfa;
}

/**
 * @brief Runs an O(n log n) algorithm that minimizes a DFA. This algorithm was created by Blum (1996), it is a modification of the O(n^2) algorithm.
 * 
 * @param dfa The DFA to be minimized
 * @return The minimized DFA
 */
DFA blumOnLognAlgorithm(DFA dfa) {
    // Initialization
    std::cout << "Preparing to run the O(n log n) algorithm...\n";

    std::cout << "Removing unreachable states...\n";
    dfa.removeUnreachableStates();
    std::cout << "Unreachable states successfully removed.\n";

    std::cout << "Running the O(n log n) algorithm...\n";

    // Algorithm
    int equivalence = 0;
    std::set<superState> Q[1024];
    Q[equivalence] = std::set<superState>();
    Q[equivalence].insert(dfa.getNonFinalStates());
    Q[equivalence].insert(dfa.getFinalStates());
    do {
        equivalence++;
        Q[equivalence] = std::set<superState>();
        Q[equivalence].clear();
        for(superState ss : Q[equivalence-1]) {
            // Ignore empty sets
            if (ss.size() == 0) {
                continue;
            }
            // Sets with 1 element
            if (ss.size() == 1) {
                Q[equivalence].insert(ss);
            }
            // Sets with 2 elements or more
            for (int i = 0; i < ss.size()-1; i++) {
                state compare1 = *std::next(ss.begin(), i);
                state compare2 = *std::next(ss.begin(), i+1);
                if (areEquivalent(compare1,compare2,dfa,Q[equivalence-1])) {
                    // Put in same set
                    bool had = false; // Indicates if there was already a set with that element
                    for (superState sss : Q[equivalence]) {
                        if (has(sss,compare1)) {
                            superState newSuperState = sss;
                            Q[equivalence].erase(sss);
                            newSuperState.insert(compare2);
                            Q[equivalence].insert(newSuperState);
                            had = true;
                            break;
                        }
                    }
                    if (!had) {
                        superState newSuperState = superState();
                        newSuperState.insert(compare1);
                        newSuperState.insert(compare2);
                        Q[equivalence].insert(newSuperState);
                    }
                } else {
                    // Put in different sets
                    bool had = false; // Indicates if there was already a set with that element
                    for (superState sss : Q[equivalence]) {
                        if (has(sss,compare1)) {
                            had = true;
                            break;
                        }
                    }
                    if (had) {
                        superState newSuperState = superState();
                        newSuperState.insert(compare2);
                        Q[equivalence].insert(newSuperState);
                    } else {
                        superState newSuperState = superState();
                        superState newSuperState2 = superState();
                        newSuperState.insert(compare1);
                        newSuperState2.insert(compare2);
                        Q[equivalence].insert(newSuperState);
                        Q[equivalence].insert(newSuperState2);
                    }
                }
            }
        }
    } while (Q[equivalence].size() != Q[equivalence-1].size());
    

    // Creating the new DFA from a SuperDFA
    SuperDFA superDfa = SuperDFA();

    // Setting up alphabet (Sigma')
    for (std::string symbol : dfa.getAlphabet()) {
        superDfa.addSymbol(symbol);
    }

    // Setting up states (Q')
    for (superState ss : Q[equivalence]) {
        superDfa.addState(ss);
    }

    // Setting up initial state (i')
    for (auto ss : superDfa.getStates()) {
        if (has(ss,dfa.getInitialState())) {
            superDfa.setInitialState(ss);
            break;
        }
    }

    // Setting up final states (F')
    for (auto ss : superDfa.getStates()) {
        for (state s : dfa.getFinalStates()) {
            if (has(ss,s)) {
                superDfa.addFinalState(ss);
            }
        }
    }

    // Setting up transitions (delta')
    for (superState X : Q[equivalence]) {
        for (std::string a : superDfa.getAlphabet()) {
            for (state e : X) {
                state transitionState = dfa.transite(e,a);
                for (superState Y : Q[equivalence]) {
                    if (has(Y,transitionState)) {
                        superDfa.addTransition(X,a,Y);
                    }
                }
            }
        }
    }

    std::cout << "O(n log n) algorithm successfully executed.\n";

    std::cout << "Minimizing the DFA...\n";

    // Converting the SuperDFA to a DFA
    DFA newDfa = superDfa.convertToDfa();

    std::cout << "DFA successfully minimized!\n\n";

    return newDfa;
}