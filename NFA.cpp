#include "NFA.h"

NFA::NFA(State* state, const vector<State*>& endStates) {
    this->startState = state;
    this->endStates = endStates;
    this->stateIdToStateMap.insert({state->id, state});
    for(State* s :endStates) {
        this->stateIdToStateMap.insert({s->id, s});
    }
}


void NFA::closureNFA() {
    // Combine all end states to only one state
   auto* combined = new State();
   this->stateIdToStateMap.insert({combined->id, combined});
   for (State* endState:this->endStates) {
       endState->addNextState(combined, vector<char>{'\0'});
   }
   this->endStates.clear();
   this->endStates.push_back(combined);
   // put new start state at the beginning
   auto* newStartState = new State();
   this->stateIdToStateMap.insert({newStartState->id, newStartState});
   newStartState->addNextState(this->startState, vector<char>{'\0'});
   // do closure
   combined->addNextState(this->startState, vector<char>{'\0'});
   this->startState = newStartState;
   this->startState->addNextState(combined, vector<char>{'\0'});
}


void NFA::positiveClosureNfa() {
    // Combine all end states to only one state
    auto* combined = new State();
    this->stateIdToStateMap.insert({combined->id, combined});
    for (State* endState:this->endStates) {
        endState->addNextState(combined, vector<char>{'\0'});
    }
    this->endStates.clear();
    this->endStates.push_back(combined);
    // put new start state at the beginning
    auto* newStartState = new State();
    this->stateIdToStateMap.insert({newStartState->id, newStartState});
    newStartState->addNextState(this->startState, vector<char>{'\0'});
    // do closure
    combined->addNextState(this->startState, vector<char>{'\0'});
    this->startState = newStartState;
}


void NFA::concatenateNFA(NFA* concatenatedNFA) {
    for (State* endState:this->endStates) {
        endState->addNextState(concatenatedNFA->startState, vector<char>{'\0'});
    }
    this->endStates.clear();
    for (State* s:concatenatedNFA->endStates) {
        this->endStates.push_back(s);
    }
    this->stateIdToStateMap.insert(concatenatedNFA->stateIdToStateMap.begin(), concatenatedNFA->stateIdToStateMap.end());
}


void NFA::ORNFA(NFA* paralleledNFA){
    auto* newStartState = new State();
    this->stateIdToStateMap.insert({newStartState->id, newStartState});
    newStartState->addNextState(this->startState, vector<char>{'\0'});
    newStartState->addNextState(paralleledNFA->startState, vector<char>{'\0'});
    vector<State*> newVector;
    newVector.reserve(this->endStates.size() + paralleledNFA->endStates.size());
    newVector.insert(newVector.end(), this->endStates.begin(), this->endStates.end() );
    newVector.insert(newVector.end(), paralleledNFA->endStates.begin(), paralleledNFA->endStates.end());
    this->endStates = newVector;
    this->startState = newStartState;
    this->stateIdToStateMap.insert(paralleledNFA->stateIdToStateMap.begin(), paralleledNFA->stateIdToStateMap.end());
}


/**
 * Combine all end states to only one state
 */
void NFA::combineEndStates(State* combined_end_state) {
    this->stateIdToStateMap.insert({combined_end_state->id, combined_end_state});
    for(State* s:this->endStates){
        s->addNextState(combined_end_state, vector<char>{'\0'});
    }
    this->endStates.clear();
    this->endStates.push_back(combined_end_state);
}
NFA::NFA() = default;
