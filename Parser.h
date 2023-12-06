#include <bits/stdc++.h>
using namespace std;
#ifndef PHASE_1__PARSER_H
#define PHASE_1__PARSER_H
#include "NFA.h"

class Parser {
public:
    static map<string,vector<char>>raw_RE_definitions;
    static map<string,string>RE_definitions;
    static map<string,pair<int,NFA*>>tokens;
    static vector<pair<string,string>>RE_expressions;

    void parseFile(string filepath);
    void parse_definition(string re_df);
    void parse_expression(string re_ex,int priority);
    void keywords_punctuation_parsing(string keyword_punctuation);
    regex generateRegex(string str);

};


#endif //PHASE_1__PARSER_H
