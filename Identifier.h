#ifndef PHASE_1__IDENTIFIER_H
#define PHASE_1__IDENTIFIER_H
#include <bits/stdc++.h>
#include "State.h"
using namespace std;
class Identifier {
public:
    vector<pair<string,string>>acceptedTokens;
    int parsing_single_token(string input);
    vector<State*> epsilonClosure (State* s);
    vector<State*> nextAndEpsilonClosures(vector<State*>next_states,char input);
    void parse_string(string input_line);
};
#endif //PHASE_1__IDENTIFIER_H
