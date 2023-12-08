#ifndef PHASE_1_STATE_H
#define PHASE_1_STATE_H
#include <bits/stdc++.h>
using namespace std;

class State {
public:
    int id;
    bool isAccepted;
    map<char,vector<State*>> nextStates;
    string acceptedToken;
    State();
    void addNextState(State* next, const vector<char>& input);
};


#endif //PHASE_1_STATE_H
