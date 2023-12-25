#ifndef PHASE_1__IDENTIFIER_H
#define PHASE_1__IDENTIFIER_H
#include <bits/stdc++.h>
#include "State.h"
#include "LL1.h"

using namespace std;
class InputReader {
public:
    vector<pair<string,string>>acceptedTokens;
    int readSingleToken(string input);
    int parsing_single_token(string input, LL1* pg, stack<string> &tempStack);
    void parse_string(string input_line, LL1* pg);
    static vector<State*> epsilonClosure (State* state);
    static vector<State*> nextAndEpsilonClosures(const vector<State*>&currentStates, char input);
    void readFile(const string& filepath);
};
#endif //PHASE_1__IDENTIFIER_H
