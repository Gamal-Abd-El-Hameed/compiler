//
// Created by mahmoud on 12/18/23.
//

#ifndef LEXICALANALYZERGENERATOR_LL1_H
#define LEXICALANALYZERGENERATOR_LL1_H
#include <bits/stdc++.h>
using namespace std;
class LL1{
public:
    vector<string> terminals;
    vector<string> grammar;
    map<string, vector<string>> firstsets, followsets;
    string left_factoring(string LHS, string RHS);
    vector<string> remove_left_recursion(string LHS, string RHS);
    void calc_first_set(vector<string> regular_definitions);
    void calc_follow_set(vector<string> regular_definitions);
    void read_grammar(const string& filepath);
};

#endif //LEXICALANALYZERGENERATOR_LL1_H
