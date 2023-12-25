#include "Parser.h"
#include <bits/stdc++.h>
#include "helper.h"
using namespace std;

map<string,vector<char>>Parser::raw_RE_definitions=map<string ,vector<char>>{};
map<string,string>Parser::RE_definitions=map<string,string>{};
map<string,pair<int,NFA*>>Parser::tokens=map<string,pair<int,NFA*>>{};
vector<pair<string,string>>Parser::RE_expressions=vector<pair<string,string>>{};
void Parser::parseFile(string filepath){
    ifstream inFile;
    inFile.open(filepath);
    string str;
    int priority=0;
    regex definition_matcher(R"([\s]*[a-zA-z]*[\s]*=[\s|a-zA-z0-9|\-|\+|\*|\(|\)|\|]*)");
    regex expression_matcher(R"([\s]*[a-zA-z]*[\s]*:[\s|a-zA-z0-9|\-|\+|\*|\(|\)|\||\=|\<|>|\!\.|\/]*)");
    regex keywords_matcher(R"(\{[\s]*([a-z]*[\s]*)*\})");
    regex punctuation_matcher(R"(\[[\s]*([\W]*[\s]*)*\])");
    while(getline(inFile, str)){
        if(regex_match(str, definition_matcher)){
            parse_definition(str);
        }else if(regex_match(str, expression_matcher)){
            parse_expression(str,priority++);
        }else if(regex_match(str, keywords_matcher)){
            keywords_punctuation_parsing(str);
        }else if(regex_match(str, punctuation_matcher)){
            keywords_punctuation_parsing(str);
        }else{
//            cout << "Parser says: "+ str + ": Invalid Rule (or empty line)" << endl;
        }
    }
    inFile.close();
    }
void Parser::parse_definition(string re_df) {
    int pos=re_df.find('=');
    string LHS = remove_spaces(re_df.substr(0,pos));
    string RHS=  re_df.substr(pos+1,re_df.size());
    if(re_df.find('-')!=string::npos){
        vector<char>alphabet= get_ranges(remove_spaces(RHS));
        raw_RE_definitions.insert({LHS,alphabet});
    }
    else{
        vector<string>RHS_tokens= split_on_spacial_chars(RHS);
        for(int i=0;i<RHS_tokens.size();i++){
            string s=RHS_tokens.at(i);
            if(raw_RE_definitions.count(s)==0&& !is_spacial_character(s)){
                while(RE_definitions.count(s)!=0){
                    s=RE_definitions.at(s);
                    RHS_tokens.at(i)=s;
                }
                if(raw_RE_definitions.count(s)==0){
                    RHS_tokens.at(i)= surround_parentheses(s);
                }
            }
        }
        string a = accumulate(RHS_tokens.begin(),RHS_tokens.end(),string(""));
        RE_definitions.insert({LHS,a});
    }
}

void Parser::parse_expression(string re_ex,int priority){
    int pos=re_ex.find(':');
    string LHS = remove_spaces(re_ex.substr(0,pos));
    string RHS= re_ex.substr(pos+1,re_ex.size());
    vector<string>RHS_tokens= split_on_spacial_chars(RHS);
    for(int i=0;i<RHS_tokens.size();i++) {
        string s = RHS_tokens.at(i);
        if(s=="\\"){
            s = "("+s;
            RHS_tokens.at(i)=s;
            i++;
            RHS_tokens.at(i)+=")";
        }
        else if(RE_definitions.count(s)!=0){
            RHS_tokens.at(i)=RE_definitions.at(s);
        }
        else if(raw_RE_definitions.count(s)==0){
            RHS_tokens.at(i)=surround_parentheses(s);
        }
    }
    string a = accumulate(RHS_tokens.begin(),RHS_tokens.end(),string(""));
    RE_expressions.emplace_back(LHS,a);
    tokens.insert({LHS,make_pair(priority,new NFA)});
}
void Parser::keywords_punctuation_parsing(string keyword) {
    keyword = keyword.substr(1, keyword.length() - 2);
    vector<string>keyword_tokens= split_on_spacial_chars(keyword);
    for(int i=0;i<keyword_tokens.size();i++) {
        string s = keyword_tokens.at(i);
        if(s=="\\"){
            string keyword_expression=keyword_tokens.at(i+1)+":"+s+keyword_tokens.at(i+1);
            parse_expression(keyword_expression,-1);
            i++;
        }
        else{
        string keyword_expression=s+":"+s;
        parse_expression(keyword_expression,-1);
        }
    }
}

