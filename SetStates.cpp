#include "SetStates.h"
#include "State.h"
#include "RulesReader.h"

SetStates::SetStates(set<State*> states) {
    this->states = states;
    this->accepted = false ;
    this->tokenType = ""  ;
}

SetStates::SetStates() {
    this->accepted = false;
    this->tokenType = "" ;
}

void SetStates::Eclosure(){
    //get the epslon of this Setstates
    stack<State*> stateStack ;

    for (State* a : this->states){
        stateStack.push(a);
        if (a->accepted){
            this->accepted = true ;
            if (this->tokenType.empty() || (RulesReader::tokens[a->tokenType].first < RulesReader::tokens[this->tokenType].first))
                this->tokenType = a->tokenType ;
        }
    }

    while (!stateStack.empty()){
        State* s = stateStack.top();
        stateStack.pop();
        this->states.insert(s);

        //check if that state has epslon closure add them to the stack
        if(s->nextStates.find('\0')!=s->nextStates.end()){
            vector<State*> epslonStates = s->nextStates['\0'];
            for (State* s : epslonStates){
                if (s->accepted) {
                    this->accepted = true ;
                    if (this->tokenType.empty() || (RulesReader::tokens[s->tokenType].first < RulesReader::tokens[this->tokenType].first))
                        this->tokenType = s->tokenType ;
                }
                stateStack.push(s);
            }
        }
    }
}

SetStates* SetStates::moveTo(char input) {
    //get the output of the input for each state
    //and fill this output into the nextstates attribute
    SetStates* output = new SetStates();

    for (State* s : states){
        if(s->nextStates.find(input)!=s->nextStates.end()){
            output->insertStates(s->nextStates[input]);
        }
    }

    output->Eclosure();

    return output;
}

string SetStates::getStatesIds() {
    vector<int> Ids ;
    string answer ;
    for (State* s : states){
        Ids.push_back(s->id);
    }

    sort(Ids.begin(),Ids.end());
    for (int Id : Ids){
        answer += to_string(Id) + "," ;
    }
    //sort states ids then convert them to string
    return answer;
}


void SetStates::insertStates(vector<State*> vector) {
    for (State* s : vector){
        if (s->accepted){
            this->accepted = true ;
            if (this->tokenType.empty() || (RulesReader::tokens[s->tokenType].first < RulesReader::tokens[this->tokenType].first))
                this->tokenType = s->tokenType ;
        }
        this->states.insert(s);
    }
}

void SetStates::insertState(State* s) {
    this->states.insert(s);
    if (s->accepted){
        this->accepted = true ;
        if (this->tokenType.empty() || (RulesReader::tokens[s->tokenType].first < RulesReader::tokens[this->tokenType].first))
            this->tokenType = s->tokenType ;
    }
}

void SetStates::insertSet(set<State *> states) {
    for (State* s : states){
        if (s->accepted){
            this->accepted = true ;
            if (this->tokenType.empty() || (RulesReader::tokens[s->tokenType].first < RulesReader::tokens[this->tokenType].first))
                this->tokenType = s->tokenType ;
        }
        this->states.insert(s);
    }
}
