#include "SetStates.h"

#include <utility>
#include "State.h"
#include "RulesReader.h"

SetStates::SetStates(set<State*> states) {
    this->states = std::move(states);
    this->isAccepted = false ;
    this->tokenType = ""  ;
}

SetStates::SetStates() {
    this->isAccepted = false;
    this->tokenType = "" ;
}

void SetStates::epsilonClosure() {
    stack<State*> stateStack ;

    for (State* state : this->states) {
        stateStack.push(state);
        if (state->isAccepted) {
            this->isAccepted = true;
            // if the token type is empty or the current token type has higher priority
            if (this->tokenType.empty() ||
            (RulesReader::tokens[state->acceptedToken].first <
             RulesReader::tokens[this->tokenType].first))
                this->tokenType = state->acceptedToken ;
        }
    }

    while (!stateStack.empty()) {
        State* state = stateStack.top(); stateStack.pop();
        this->states.insert(state);

        // if the state has epsilon transition, push it to the stack
        if(state->nextStates.find('\0') != state->nextStates.end()) {
            vector<State*> epsilonStates = state->nextStates['\0'];
            for (State* epsilonState : epsilonStates) {
                if (epsilonState->isAccepted) {
                    this->isAccepted = true;
                    if (this->tokenType.empty() ||
                    (RulesReader::tokens[epsilonState->acceptedToken].first <
                     RulesReader::tokens[this->tokenType].first))
                        this->tokenType = epsilonState->acceptedToken ;
                }
                stateStack.push(epsilonState);
            }
        }
    }
}


SetStates* SetStates::moveTo(char input) {
    auto* res = new SetStates();

    for (State* state : states) {
        if(state->nextStates.find(input) != state->nextStates.end()){
            res->insertStates(state->nextStates[input]);
        }
    }
    res->epsilonClosure();
    return res;
}


string SetStates::getStatesIds() {
    vector<int> ids;
    string stateIds;
    ids.reserve(states.size());
    for (State* s : states) {
        ids.push_back(s->id);
    }

    sort(ids.begin(), ids.end());
    for (int Id : ids) {
        stateIds += to_string(Id) + "," ;
    }
    return stateIds;
}


void SetStates::insertStates(const vector<State*>& vector) {
    for (State* state : vector) {
        insertState(state);
    }
}


void SetStates::insertState(State* state) {
    this->states.insert(state);
    if (state->isAccepted) {
        this->isAccepted = true;
        if (this->tokenType.empty() ||
        (RulesReader::tokens[state->acceptedToken].first <
         RulesReader::tokens[this->tokenType].first))
            this->tokenType = state->acceptedToken ;
    }
}


void SetStates::insertSet(const set<State *>& statesSet) {
    for (State* state : statesSet) {
        insertState(state);
    }
}
