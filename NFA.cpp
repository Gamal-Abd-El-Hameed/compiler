#include "NFA.h"
NFA::NFA(State* s, vector<State*> v) {
    this->start_state = s;
    this->end_states = v;
    this->transitions.insert({s->id,s});
    for(State* s :v){
        this->transitions.insert({s->id,s});
    }
}
void NFA::closureNFA(){
    // Combine all end states to only one state
   State* combined = new State();
   this->transitions.insert({combined->id,combined});
   for(State* s:this->end_states){
       s->addNextState(combined, vector<char>{'\0'});
   }
   this->end_states.clear();
   this->end_states.push_back(combined);
   // put new start state at the beginning
   State* new_start_state = new State();
   this->transitions.insert({new_start_state->id,new_start_state});
   new_start_state->addNextState(this->start_state,vector<char>{'\0'});
   // do closure
   combined->addNextState(this->start_state,vector<char>{'\0'});
   this->start_state=new_start_state;
   this->start_state->addNextState(combined,vector<char>{'\0'});
}
void NFA::positive_closureNFA(){
    // Combine all end states to only one state
    State* combined = new State();
    this->transitions.insert({combined->id,combined});
    for(State* s:this->end_states){
        s->addNextState(combined, vector<char>{'\0'});
    }
    this->end_states.clear();
    this->end_states.push_back(combined);
    // put new start state at the beginning
    State* new_start_state = new State();
    this->transitions.insert({new_start_state->id,new_start_state});
    new_start_state->addNextState(this->start_state,vector<char>{'\0'});
    // do closure
    combined->addNextState(this->start_state,vector<char>{'\0'});
    this->start_state=new_start_state;
}
void NFA::concatenateNFA(NFA* concatenatedNFA){
    for(State* s:this->end_states){
        s->addNextState(concatenatedNFA->start_state,vector<char>{'\0'});
    }
    this->end_states.clear();
    for(State* s:concatenatedNFA->end_states){
        this->end_states.push_back(s);
    }
    this->transitions.insert(concatenatedNFA->transitions.begin(), concatenatedNFA->transitions.end());
}
void NFA::ORNFA(NFA* paralleledNFA){
    State* new_start_state = new State();
    this->transitions.insert({new_start_state->id,new_start_state});
    new_start_state->addNextState(this->start_state, vector<char>{'\0'});
    new_start_state->addNextState(paralleledNFA->start_state, vector<char>{'\0'});
    vector<State*> newVector;
    newVector.reserve( this->end_states.size() + paralleledNFA->end_states.size() );
    newVector.insert( newVector.end(), this->end_states.begin(), this->end_states.end() );
    newVector.insert( newVector.end(), paralleledNFA->end_states.begin(), paralleledNFA->end_states.end());
    this->end_states=newVector;
    this->start_state=new_start_state;
    this->transitions.insert(paralleledNFA->transitions.begin(), paralleledNFA->transitions.end());
}
/**
 * Combine all end states to only one state
 * @param combined_end_state
 */
void NFA::combine_end_states(State* combined_end_state){
    this->transitions.insert({combined_end_state->id,combined_end_state});
    for(State* s:this->end_states){
        s->addNextState(combined_end_state, vector<char>{'\0'});
    }
    this->end_states.clear();
    this->end_states.push_back(combined_end_state);
}
NFA::NFA() {
}

