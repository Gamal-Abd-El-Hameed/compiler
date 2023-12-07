#ifndef PHASE_1__IDENTIFIER_H
#define PHASE_1__IDENTIFIER_H
#include <bits/stdc++.h>
#include "State.h"
using namespace std;
class InputReader {
public:
    vector<pair<string,string>>acceptedTokens;
    int readSingleToken(string input);
    static vector<State*> epsilonClosure (State* state);
    static vector<State*> nextAndEpsilonClosures(const vector<State*>&currentStates, char input);
    void readFile(const string& filepath);
};
#endif //PHASE_1__IDENTIFIER_H
