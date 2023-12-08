#include "DFA.h"
#include "SetStates.h"
#include "State.h"
#include "NFA.h"
#include "utility.h"

string inputs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";
DFA::DFA() = default;

void DFA::generateDFA(NFA *finalNFA) {
    // get the starting state of the NFA to be the starting state of the DFA
    auto* start = new SetStates(set<State*>{finalNFA->startState});
    start->epsilonClosure();
    this->startingState = start;

    queue<SetStates*> statesQueue;
    statesQueue.push(this->startingState);

    while (!statesQueue.empty()) {
        SetStates* currentSetStates = statesQueue.front(); statesQueue.pop();
        string ids = currentSetStates->getStatesIds();
        this->stateIdToSetStatesMap.insert(
                pair<string,SetStates*>(ids, currentSetStates));

        // for each input get the next state of the current state
        for (char input  : inputs) {
            auto* nextStates = new SetStates();
            nextStates = currentSetStates->moveTo(input);

            // if the next state is empty, don't add it to the queue
            if (nextStates->states.empty()) {
                continue;
            }
            // if the next state is already in the map, don't add it to the queue
            if (this->stateIdToSetStatesMap.find(nextStates->getStatesIds()) != this->stateIdToSetStatesMap.end()) {
                currentSetStates->nextStates.insert(pair<char,SetStates*>(input, nextStates));
                continue ;
            }
            // if the next state is not in the map, add it to the queue
            statesQueue.push(nextStates);
            currentSetStates->nextStates.insert(pair<char,SetStates*>(input, nextStates));
        }
    }
}

vector<SetStates*> DFA::getStates(bool isAccepted) {
    vector<SetStates*> states;
    for (pair<string,SetStates*> pair : this->stateIdToSetStatesMap){
        if (pair.second->isAccepted == isAccepted) {
            states.push_back(pair.second);
        }
    }
    return states;
}

vector<SetStates*> DFA::remainingStates() {
    return getStates(false);
}

vector<SetStates*> DFA::acceptingStates() {
    return getStates(true);
}

void DFA::writeToFile(const string& fileName) {
    ofstream file;
    file.open(fileName);
    // write the stateIdToSetStatesMap to the file
    for (pair<string,SetStates*> a : this->stateIdToSetStatesMap){
        for (pair<char,SetStates*> b : a.second->nextStates){
            // {a.first is the id of the state, b.first is the input, b.second is the output}
            file << a.first << " --> " << b.first << " --> " << b.second->getStatesIds() << "\n";
        }
    }
    file.close();
}