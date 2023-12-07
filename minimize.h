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


map<string, int> stateToGroupMap;

/**
 * loop on the groups and fill the map
*/
void updateStateToGroupMap(vector<vector<SetStates*>> groups) {
    stateToGroupMap.clear();
    for (int groupNumber = 0; groupNumber < groups.size(); ++groupNumber) {
        for (const auto &state : groups[groupNumber]) {
            stateToGroupMap[state->getStatesIds()] = groupNumber;
        }
    }
}


vector<vector<SetStates*>> minimizeDFA (vector<SetStates*> remStates, vector<SetStates*> finalStates){
    const string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";
    int previousGroupCount = 0 ;
    vector<vector<SetStates*>> groups {std::move(remStates),std::move(finalStates)};
    updateStateToGroupMap(groups);

    while(previousGroupCount != groups.size()) {
        //get the number of partitions in this iteration
        previousGroupCount = (int)groups.size();
        vector<vector<SetStates*>> newGroups;
        for (const auto& group : groups) {
            // base case if this group is only one element
            if (group.size() == 1) {
                newGroups.push_back(group);
                continue;
            }
            vector<SetStates*> identicalStates, remainingStates;
            auto firstState = group[0];
            identicalStates.push_back(firstState);
            // loop to fill identicalStates and remainingStates vectors
            for (int i = 1 ; i < group.size() ; i++) {
                // currentState is the state that we are comparing with firstState
                auto currentState = group[i];
                bool isDifferent = false;
                for (char input : alphabet) {
                    bool hasTransition1 = (firstState->nextStates.find(input) != firstState->nextStates.end());
                    bool hasTransition2 = (currentState->nextStates.find(input) != currentState->nextStates.end());
                    // if the 2 states are different
                    // or if the 2 states have transition but to different groups
                    if (hasTransition1 != hasTransition2 ||
                        (hasTransition1 &&
                         (stateToGroupMap[firstState->nextStates[input]->getStatesIds()] !=
                          stateToGroupMap[currentState->nextStates[input]->getStatesIds()]))) {
                        remainingStates.push_back(currentState);
                        isDifferent = true;
                        break;
                    }
                }
                //if the 2 states are identicalStates
                if(!isDifferent)
                    identicalStates.push_back(currentState);
            }
            newGroups.push_back(identicalStates);
            if(!remainingStates.empty())
                newGroups.push_back(remainingStates);
        }
        groups = newGroups;
        updateStateToGroupMap(groups);
    }
    return groups;
}


DFA* generateMinDFA(vector<vector<SetStates*>> finalGroups) {
    const string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";
    vector<SetStates*> minimizedStates;

    // Concatenate the groups together.
    // loop on the groups
    for (const auto& group : finalGroups) {
        auto* setStates = new SetStates();
        // loop on the states in the group
        for (SetStates* setState : group) {
            setStates->insertSet(setState->states);
        }
        minimizedStates.push_back(setStates) ;
    }

    // get the next state for each state
    for (int i = 0; i < finalGroups.size(); i++) {
        SetStates* representativeState = finalGroups[i][0];
        for(char input:alphabet) {
            // if the representativeState has transition with this input
            if (representativeState->nextStates.find(input) != representativeState->nextStates.end()) {
                string outputStateId = representativeState->nextStates[input]->getStatesIds();
                minimizedStates[i]->nextStates.insert({input, minimizedStates[stateToGroupMap[outputStateId]]});
            }
        }
    }

    DFA* minimizedDFA = new DFA();
    minimizedDFA->startingState = minimizedStates[0];
    for (SetStates* states:minimizedStates) {
        minimizedDFA->transitions.insert({states->getStatesIds(),states});
    }
    return minimizedDFA;
}

/**
 * @brief this function takes the source file and the minimized DFA
 * and print the lexemes and their types
 */
void patternMatching(const string& sourceFile, DFA* minimizedDFA) {
    SetStates* currentState = minimizedDFA->startingState;
    string currentLexeme, finalLexeme, acceptingType;
    vector<pair<string,string>> matches;

    for (int i = 0; i < sourceFile.size(); i++){
        char currentChar = sourceFile[i];
        currentLexeme += currentChar;

        // if this char is not in the alphabet
        if (currentState->nextStates.find(currentChar) == currentState->nextStates.end()) {
            if (!finalLexeme.empty()) {
                matches.emplace_back(finalLexeme, acceptingType);
                i--;
            }
            currentLexeme.clear();
            finalLexeme.clear();
            acceptingType.clear();
            currentState = minimizedDFA->startingState ;
            continue;
        }

        SetStates* transitionState = minimizedDFA->transitions[currentState->nextStates[currentChar]->getStatesIds()];

        // if this transitionState is accepting state
        if (transitionState->accepted) {
            finalLexeme = currentLexeme ;
            acceptingType = transitionState->tokenType ;
        }
    }
}


#endif //DFA_DFAMINIMIZED_H
