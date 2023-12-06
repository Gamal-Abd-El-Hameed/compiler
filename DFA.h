#ifndef DFA_DFA_H
#define DFA_DFA_H

#include <bits/stdc++.h>
#include "SetStates.h"
#include "NFA.h"
#include "State.h"

using namespace std;

class DFA {
public:
    SetStates *startingState;
    map<string ,SetStates*> transitions;

    DFA();
    void generateDFA(NFA* NFAfinal);
    vector<SetStates*> remainingStates();
    vector<SetStates*> acceptingStates();
    void writeToFile(const string& filename);
};



#endif //DFA_DFA_H
