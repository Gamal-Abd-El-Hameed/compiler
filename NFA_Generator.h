#ifndef PHASE_1__NFA_GENERATOR_H
#define PHASE_1__NFA_GENERATOR_H
#include "NFA.h"


class NFA_Generator {
public:
    static NFA* total_NFA;
    vector<NFA*>NFAs;
    NFA* generate_NFA(pair<string,string> RE_expression_pair,map<string,vector<char>>RE_definitions);
    NFA* postfix_eval(vector<string>postfix,map<string,vector<char>>RE_definitions,string accepted_type);
    void combine_NFAs();
    void generate_all_NFAs(vector<pair<string,string>>RE_expression_pairs,map<string,vector<char>>RE_definition);
};


#endif //PHASE_1__NFA_GENERATOR_H
