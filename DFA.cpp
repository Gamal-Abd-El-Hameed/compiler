#include "DFA.h"
#include "SetStates.h"
#include "State.h"
#include "NFA.h"
#include "helper.h"

string inputs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";
DFA::DFA() {

}

void DFA::generateDFA(NFA *NFAfinal){

    //first get the starting state of the NFA to be the starting state of the DFA
    SetStates* start = new SetStates();
    *start = SetStates(set<State*>{NFAfinal->start_state});

    start->Eclosure();
    this->startingState = start;

    queue<SetStates*> Dstates ;
    Dstates.push(this->startingState);

    int lvl= 0 ;

    while (!Dstates.empty()){
//        cout << lvl++ << "\n";

        SetStates* T = Dstates.front();
        Dstates.pop();

        string Ids = T->getStatesIds();
//        cout << Ids << "\n" ;
        this->transitions.insert(pair<string,SetStates*>(Ids,T));


        //for each input get the output of T
        for (char input  : inputs){
            SetStates* output = new SetStates();
            output = T->moveTo(input);

            //check if we have this state before in the transition table or U is empty
            if (output->states.empty()){
                continue;
            }
            if (this->transitions.find(output->getStatesIds())!=this->transitions.end()){
                T->nextStates.insert(pair<char,SetStates*>(input,output));
                continue ;
            }

//            cout << "input : " << input << "\n" ;
//            cout << output->getStatesIds() << "\n";
//            cout << output->accepted << "\n" ;
            Dstates.push(output);
            T->nextStates.insert(pair<char,SetStates*>(input,output));
        }

    }


}

vector<SetStates*> DFA::remainingStates() {
    vector<SetStates*> answer ;

    for (pair<string,SetStates*> a : this->transitions){
        if (!a.second->accepted) {
            answer.push_back(a.second);
            //cout << a.first << "\n";
        }
    }
    return answer ;
}

vector<SetStates*> DFA::acceptingStates() {
    vector<SetStates*> answer ;

    for (pair<string,SetStates*> a  : this->transitions){
        if (a.second->accepted) {
            answer.push_back(a.second);
            //cout << a.first << "\n";
        }
    }
    return answer ;
}

void DFA::writeToFile(const string& fileName) {
    ofstream file;
    file.open(fileName);
    // write the transitions to the file
    for (pair<string,SetStates*> a : this->transitions){
        for (pair<char,SetStates*> b : a.second->nextStates){
            // {a.first is the id of the state, b.first is the input, b.second is the output}
            file << a.first << " --> " << b.first << " --> " << b.second->getStatesIds() << "\n";
        }
    }
    file.close();
}