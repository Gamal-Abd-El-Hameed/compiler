#ifndef PHASE_1__STATE_H
#define PHASE_1__STATE_H
#include <bits/stdc++.h>
using namespace std;

class State {
public:
    int id;
    bool accepted;
    map<char,vector<State*>> nextStates;
    string tokenType;
    State();
    void addNextState(State* next, vector<char>input);


};


#endif //PHASE_1__STATE_H
