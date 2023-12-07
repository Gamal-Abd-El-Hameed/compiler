#include "InputReader.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "RulesReader.h"


void InputReader::readFile(const string& filepath) {
    ifstream inFile;
    inFile.open(filepath);
    string inputLine;
    while(getline(inFile, inputLine)) {
        while(!inputLine.empty()) {
            int index = readSingleToken(inputLine);
            inputLine = index == -1 ? inputLine.substr(1, inputLine.size())
                            : inputLine.substr(index + 1, inputLine.size());
        }
    }
}


int InputReader::readSingleToken(string input) {
    int accepted_index = -1;
    string accepted_token, acceptedString, res;
    NFA *totalNFA = NFA_Generator::combinedNFA;
    vector<State *> currentStates = epsilonClosure(totalNFA->startState);
    for (int i = 0; i < input.size(); i++) {
        char currentChar = input[i];
        res += currentChar;
        if (currentStates.empty())
            break;
        vector<State *> comingStates = nextAndEpsilonClosures(currentStates, currentChar);
        int accepted_priority = INT_MAX;
        for (State *comingState: comingStates) {
            if (comingState->isAccepted) {
                if (accepted_priority > RulesReader::tokens.at(comingState->tokenType).first) {
                    accepted_priority = RulesReader::tokens.at(comingState->tokenType).first;
                    accepted_index = i;
                    accepted_token = comingState->tokenType;
                    acceptedString = res;
                }
            }
        }
        currentStates = comingStates;
    }
    if (accepted_index > -1) {
        acceptedTokens.emplace_back(acceptedString, accepted_token);
    } else if(input[0] != ' ' && input[0] != '\n' && input[0] != '\t') {
        acceptedTokens.emplace_back(to_string(input[0]), "Undefined");
    }
    return accepted_index;
}


vector<State*> InputReader::nextAndEpsilonClosures(const vector<State*>&currentStates, char input){
   set<State*> res;
   for(State* currentState:currentStates) {
       if(currentState->nextStates.count(input) != 0) {
           vector<State *> nextStates = currentState->nextStates.at(input);
           for (State *s: nextStates) {
               vector<State *> epsilonClosureVector = epsilonClosure(s);
               for (State *epsilonClosureState: epsilonClosureVector)
                   res.insert(epsilonClosureState);
           }
       }
   }
   vector<State*> v(res.begin(), res.end());
   return v;
}


vector<State*> InputReader::epsilonClosure(State* state) {
    set<int> visited = {state->id};
    vector<State*> closure = {state};
    for(int i = 0; i < closure.size(); i++) {
        state = closure.at(i);
        map<char,vector<State*>> nextStates = state->nextStates;
        if (nextStates.count('\0') != 0) {
            vector<State*> epsilonClosureVector = state->nextStates.at('\0');
            for (State *epsilonClosureState: epsilonClosureVector) {
                if (visited.count(epsilonClosureState->id) == 0) {
                    closure.push_back(epsilonClosureState);
                    visited.insert(epsilonClosureState->id);
                }
            }
        }
    }
    return closure;
}
