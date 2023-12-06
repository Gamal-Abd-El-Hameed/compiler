#ifndef PHASE_1__NFA_H
#define PHASE_1__NFA_H


#include "State.h"

class NFA {
public:
    State* start_state;
    vector<State*>end_states;
    map<int,State*>transitions;
    NFA(State* s,vector<State*> v);
    NFA();
    void closureNFA();
    void concatenateNFA(NFA* concatenatedNFA);
    void ORNFA(NFA* paralleledNFA);
    void positive_closureNFA();
    void combine_end_states(State* combined_state);
};


#endif //PHASE_1__NFA_H
