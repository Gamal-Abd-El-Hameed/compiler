//
// Created by mahmoud on 12/18/23.
//

#ifndef LEXICALANALYZERGENERATOR_LL1_H
#define LEXICALANALYZERGENERATOR_LL1_H
#include <bits/stdc++.h>
using namespace std;
class LL1 {
public:
    vector<pair<string,string>>grammer_rules;
    map<string,vector<string>> rules_map;
    map<string,set<string>> first_sets;
    map<string,map<string,string>> table;
    map<string,set<string>> follow_sets;
    void read_file(string filepath);
    void removeLR();
    void left_factor();
    void get_parsing_table();
    void substitute(int i,int j);
    void eliminate_immediate_LR(int rule_index);
    string get_match_substr(string group, string p);
    vector <string> remove_substr(vector <string> vec, string str);
    void get_first_sets();
    void get_follow_sets();
    void first_for_one_key(const string& basicString,stack <string>& stack1);
    bool is_terminal(string str);
    bool has_epsilon(vector<string> prods);
    map<string,vector<string>> get_graph();
    void get_follow_for_one_key(string str,map<string,vector<string>> right_most);
    void create_table();

    vector<string> LL1_parse(string input, stack<string>& s);
};

#endif //LEXICALANALYZERGENERATOR_LL1_H
