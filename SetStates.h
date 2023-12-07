#ifndef DFA_SETSTATES_H
#define DFA_SETSTATES_H

#include <bits/stdc++.h>
#include "State.h"
using namespace std;

class SetStates {
public:
    bool isAccepted;
    set<State*> states ;
    map<char, SetStates*> nextStates;
    string tokenType;

    explicit SetStates(set<State*> states);
    SetStates();
    void epsilonClosure();
    SetStates* moveTo(char input);
    string getStatesIds();
    void insertStates(const vector<State*>& vector);
    void insertSet(const set<State*>& statesSet);
    void insertState(State* state);
};

#endif //DFA_SETSTATES_H
