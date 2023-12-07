#ifndef PHASE_1_NFA_H
#define PHASE_1_NFA_H


#include "State.h"

class NFA {
public:
    State* startState{};
    vector<State*> endStates;
    map<int,State*> stateIdToStateMap;
    NFA(State* state, const vector<State*>& endStates);
    NFA();
    void closureNFA();
    void concatenateNFA(NFA* concatenatedNFA);
    void ORNFA(NFA* paralleledNFA);
    void positiveClosureNfa();
    void combineEndStates(State* combined_end_state);
};


#endif //PHASE_1_NFA_H
