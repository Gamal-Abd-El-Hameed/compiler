#ifndef PHASE_1__NFA_GENERATOR_H
#define PHASE_1__NFA_GENERATOR_H
#include "NFA.h"


class NFA_Generator {
public:
    static NFA* combinedNFA;
    vector<NFA*> NFAs;
    static NFA* generateNfa(const pair<string,string>& regularDefinition, map<string,vector<char>>rawRegularExpressions);
    static NFA* postfixEval(vector<string>tokens, map<string,vector<char>>rawRegularExpressions, string acceptedType);
    void combineNfAs();
    void generateNfAs(const vector<pair<string,string>>&regularDefinitions, const map<string,vector<char>>&rawRegularExpressions);
};


#endif //PHASE_1__NFA_GENERATOR_H
