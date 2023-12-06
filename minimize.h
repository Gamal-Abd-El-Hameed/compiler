//
// Created by ahmed on 12/10/2022.
//
#include <iostream>
#include <utility>
#include "Parser.h"
#include "State.h"
#include "helper.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "Identifier.h"
#include "SetStates.h"
#include "DFA.h"


#ifndef DFA_DFAMINIMIZED_H
#define DFA_DFAMINIMIZED_H


map<string, int> statesChecker ;

void fillTheMap(vector<vector<SetStates*>> groups){
    statesChecker.clear();
    for (int i = 0; i < groups.size(); ++i) {
        for(int j = 0; j < groups[i].size(); j++)
            statesChecker.insert({groups[i][j]->getStatesIds(), i});
    }
}


vector<vector<SetStates*>> minimizeDFA (vector<SetStates*> remStates, vector<SetStates*> finalStates){

    string inputs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";


    int sizeOfGroups = 0 ;
    vector<vector<SetStates*>> groups {std::move(remStates),std::move(finalStates)};

    fillTheMap(groups);

    while(sizeOfGroups != groups.size()){
        //get the number of partitions in this iteration
        sizeOfGroups = groups.size();
        vector<vector<SetStates*>> newGroups ;
        for(vector<SetStates*> group : groups){
            // base case if this group is only one element
            if (group.size() < 2){
                newGroups.push_back(group);
                continue;
            }
            vector<SetStates*> identical , remaining ;
            identical.push_back(group[0]);
            //for loop to fill identical and remaining vectors
            for (int i = 1 ; i < group.size() ; i++){
                bool added = false;
                for(char input : inputs){
                    bool check1 = false, check2 = false;
                    if(group[0]->nextStates.find(input) != group[0]->nextStates.end())
                        check1 = true;
                    if(group[i]->nextStates.find(input) != group[i]->nextStates.end())
                        check2 = true;
                    // if one of them are not taking the input and the other is
                    // or if both of them lead to different group in the output
                    if (check1 != check2 || ((check1 && check2) && (statesChecker[group[0]->nextStates[input]->getStatesIds()] != statesChecker[group[i]->nextStates[input]->getStatesIds()] ))) {
                        remaining.push_back(group[i]);
                        added = true;
                        break;
                    }
                }
                //if the 2 states are identical
                if(!added)
                    identical.push_back(group[i]);
            }
            //create the new partition
            newGroups.push_back(identical);
            if(!remaining.empty())
                newGroups.push_back(remaining);
        }
        groups = newGroups;
        fillTheMap(groups);
    }
    return groups;
}


DFA* generateMinDFA(vector<vector<SetStates*>> finalGroups){

    string inputs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";

    vector<SetStates*> newStates ;

    //Concatenate the groups together.
    for (vector<SetStates*> group : finalGroups){

        SetStates* newState = new SetStates();

        for (SetStates* setState : group){
            newState->insertSet(setState->states);
        }

        //cout << newState->getStatesIds() << "\n\n" ;

        newStates.push_back(newState) ;
    }

    //get the next state for each state
    for (int i = 0 ; i < finalGroups.size() ; i++){
        SetStates* firstOne  = finalGroups[i][0];

        for(char input : inputs){
            if (firstOne->nextStates.find(input) != firstOne->nextStates.end()) {
                string output = firstOne->nextStates[input]->getStatesIds();
                newStates[i]->nextStates.insert({input, newStates[statesChecker[output]]});
            }
        }
    }

    //create the new minimized DFA
    DFA* minimizedDFA = new DFA();

    minimizedDFA->startingState = newStates[0];
    for (SetStates* states : newStates){
        minimizedDFA->transitions.insert({states->getStatesIds(),states});
    }

    return minimizedDFA ;

}


void patternMatching(const string& sourceFile,DFA* minimizedDFA){

    //initialize temp variables
    SetStates* s = new SetStates();
    s = minimizedDFA->startingState ;
    string currLexeme , finLexeme , acceptingType ;
    vector<pair<string,string>> answer ;

    for (int i = 0 ; i < sourceFile.size() ; i++){
        char c = sourceFile[i] ;

        currLexeme += c ;

        //s has no output with that input
        if (s->nextStates.find(c) == s->nextStates.end()){
            if (!finLexeme.empty()){
                //push in the answer
                answer.emplace_back(finLexeme,acceptingType);
                //cout << finLexeme << " : " << acceptingType << "\n" ;
                i--;
            }

            //reset
            currLexeme.clear();
            finLexeme.clear();
            acceptingType.clear();
            s = minimizedDFA->startingState ;

            continue;
        }

        SetStates* output = new SetStates();
        output = minimizedDFA->transitions[s->nextStates[c]->getStatesIds()];

        //if this output is accepting state
        if (output->accepted){
            //append and update finalLexeme
            //currLexeme += c ;
            finLexeme = currLexeme ;
            acceptingType = output->tokenType ;
        }

        //else this output is normal state
        //append

    }

}


#endif //DFA_DFAMINIMIZED_H
