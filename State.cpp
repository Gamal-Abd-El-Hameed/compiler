#include "State.h"
#include "helper.h"
State::State(){
    this->id=id_generator++;
    this->accepted=false;
}
void State::addNextState(State* next, vector<char> input) {
    for(char i : input) {
        if (this->nextStates.count(i) != 0) {
            this->nextStates.at(i).push_back(next);
        }
        else {
            this->nextStates.insert({i, vector<State*>{next}});
        }
    }
}