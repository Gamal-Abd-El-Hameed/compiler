#include "NFA_Generator.h"

#include <utility>
#include "helper.h"
#include "State.h"


NFA* NFA_Generator::combinedNFA=new NFA(new State(), vector<State*>{});


void NFA_Generator::generateNfAs(const vector<pair<string,string>>&regularDefinitions,
                                 const map<string,vector<char>>&rawRegularExpressions) {
    for(const pair<string,string>& regularDefinition:regularDefinitions) {
        NFA* result = generateNfa(regularDefinition, rawRegularExpressions);
        this->NFAs.push_back(result);
    }
    combineNfAs();
}


NFA* NFA_Generator::generateNfa(const pair<string,string>& regularDefinition,
                                map<string,vector<char>> rawRegularExpressions) {
    string LHS = regularDefinition.first;
    string RHS = regularDefinition.second;
    vector<string> tokens = split_on_spacial_chars(RHS);
    vector<string> v = generate_infix(tokens);
    v = infixToPostfix(v);
    return postfixEval(v, std::move(rawRegularExpressions), LHS);
}


NFA* NFA_Generator::postfixEval(vector<string> tokens,
                    map<string,vector<char>> rawRegularExpressions, string acceptedType) {
    stack<NFA*> NFAStack;
     for (int i = 0; i < tokens.size(); i++) {
         string token = tokens.at(i);
         if ((!is_spacial_character(token) && token != "`" ) || (token == "\\")) {
            auto *state = new State(), *end_state = new State();
            if (token == "\\") {
                if (tokens.at(++i) == "L") {
                    state->addNextState(end_state, vector<char>{'\0'});
                }
                else {
                    token = tokens.at(i);
                }
            }
            if(rawRegularExpressions.find(token) != rawRegularExpressions.end()) {
                state->addNextState(end_state, rawRegularExpressions.at(token));
            }
            else if (token != "\\") {
                state->addNextState(end_state,vector<char>{token[0]});
            }
            NFA* currentNFA=new NFA(state,vector<State*>{end_state});
            NFAStack.push(currentNFA);
         }
         else {
             if (token == "*") {
                NFAStack.top()->closureNFA();
             }
             else if (token == "+") {
                 NFAStack.top()->positiveClosureNfa();
             }
             else if (token == "|") {
                 NFA* NFA1=NFAStack.top(); NFAStack.pop();
                 NFA* NFA2=NFAStack.top(); NFAStack.pop();
                 NFA1->ORNFA(NFA2);
                 NFAStack.push(NFA1);
             }
             else if (token == "`") {
                 NFA* NFA1=NFAStack.top(); NFAStack.pop();
                 NFA* NFA2=NFAStack.top(); NFAStack.pop();
                 NFA2->concatenateNFA(NFA1);
                 NFAStack.push(NFA2);
             }
         }
     }
     NFA* accepted_NFA=NFAStack.top();
     auto* acceptingState=new State();
    acceptingState->isAccepted = true;
    acceptingState->tokenType= std::move(acceptedType);
    accepted_NFA->combineEndStates(acceptingState);
     return accepted_NFA;
}


void NFA_Generator::combineNfAs() {
    for(NFA* nfa:NFAs) {
        combinedNFA->startState->addNextState(nfa->startState, vector<char>{'\0'});
        combinedNFA->stateIdToStateMap.insert(nfa->stateIdToStateMap.begin(), nfa->stateIdToStateMap.end());
        combinedNFA->endStates.push_back(nfa->endStates[0]);
    }
}
