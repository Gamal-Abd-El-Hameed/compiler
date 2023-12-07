#ifndef DFA_DFA_H
#define DFA_DFA_H

#include <bits/stdc++.h>
#include "SetStates.h"
#include "NFA.h"
#include "State.h"

using namespace std;

class DFA {
public:
    SetStates *startingState{};
    map<string, SetStates*> stateIdToSetStatesMap;

    DFA();
    void generateDFA(NFA* finalNFA);
    vector<SetStates*> remainingStates();
    vector<SetStates*> acceptingStates();
    vector<SetStates*> getStates(bool isAccepted);
    void writeToFile(const string& filename);
};



#endif //DFA_DFA_H
