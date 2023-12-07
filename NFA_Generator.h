#ifndef PHASE_1__NFA_GENERATOR_H
#define PHASE_1__NFA_GENERATOR_H
#include "NFA.h"


class NFA_Generator {
public:
    static NFA* combinedNFA;
    vector<NFA*> NFAs;
    NFA* generateNfa(pair<string,string> RE_expression_pair, map<string,vector<char>>RE_definitions);
    NFA* postfixEval(vector<string>postfix, map<string,vector<char>>RE_definitions, string accepted_type);
    void combineNfAs();
    void generateNfAs(vector<pair<string,string>>RE_expression_pairs, map<string,vector<char>>RE_definition);
};


#endif //PHASE_1__NFA_GENERATOR_H
