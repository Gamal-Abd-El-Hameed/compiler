//
// Created by mahmoud on 12/23/23.
//

#ifndef LEXICALANALYZERGENERATOR_PARSER_H
#define LEXICALANALYZERGENERATOR_PARSER_H
#include <bits/stdc++.h>
#include "LL1.h"

using namespace std;

class Parser{
public:
    vector<string> input;
    stack<string> stack_;
    map<string, map<string, string>> parsing_table;
    void Parse(vector<string> in, LL1* ll1);

};
#endif //LEXICALANALYZERGENERATOR_PARSER_H