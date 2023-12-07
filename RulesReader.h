#include <bits/stdc++.h>
using namespace std;
#ifndef PHASE_1__PARSER_H
#define PHASE_1__PARSER_H
#include "NFA.h"

class RulesReader {
public:
    static map<string,vector<char>>rawRegularExpressions;
    static map<string,string>regularExpressions;
    static map<string,pair<int,NFA*>>tokens;
    static vector<pair<string,string>>regularDefinitions;

    static void readFile(const string& filepath);
    static void parseExpression(const string& regularExpression);
    static void parseDefinition(const string& regularDefinition, int priority);
    static void keywordsPunctuationParsing(string keyword);
};


#endif //PHASE_1__PARSER_H
