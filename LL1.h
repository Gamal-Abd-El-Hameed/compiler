//
// Created by mahmoud on 12/18/23.
//

#ifndef LEXICALANALYZERGENERATOR_LL1_H
#define LEXICALANALYZERGENERATOR_LL1_H
#include <bits/stdc++.h>
using namespace std;
class LL1 {
public:
    vector<pair<string,string>>Grammar_rules;
    map<string,vector<string>> rules_map;
    map<string,set<string>> first_sets;
    map<string,map<string,string>> table;
    map<string,set<string>> follow_sets;
    void readFile(const string& filepath);
    void eliminateLeftRecursion();
    void leftFactor();
    void getParsingTable();
    void substituteProductions(int i,int j);
    void eliminateImmediateLeftRecursion(int rule_index);
    string getMatchSubstr(const string& group, const string& p);
    vector <string> removeSubstr(const vector<string>& vec, const string& str);
    void getFirstSets();
    void getFollowSets();
    void firstForOneKey(const string& basicString,stack <string>& stack1);
    bool isTerminal(string str);
    bool hasEpsilon(vector<string> prods);
    map<string,vector<string>> getGraph();
    void getFollowForOneKey(string str,map<string,vector<string>> right_most);
    void createTable();
    void writeGrammarRules(const string& title);
    void printGrammarRules();
    void writeTableResults();

    vector<string> LL1_parse(string input, stack<string>& s);
};

#endif //LEXICALANALYZERGENERATOR_LL1_H
