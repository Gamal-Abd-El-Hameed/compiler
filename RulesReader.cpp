#include "RulesReader.h"
#include <bits/stdc++.h>
#include "utility.h"
using namespace std;

map<string, vector<char>>RulesReader::rawRegularExpressions = {};
map<string, string>RulesReader::regularExpressions = {};
map<string, pair<int,NFA*>>RulesReader::tokens = {};
vector<pair<string, string>>RulesReader::regularDefinitions = {};

/**
 * @brief parse the file and fill the regularExpressions and regularDefinitions vectors
 * @param filepath
 */
void RulesReader::readFile(const string& filepath) {
    ifstream inFile;
    inFile.open(filepath);
    string str;
    int priority = 0;
    regex expressionMatcher(R"([\s]*[a-zA-z]*[\s]*=[\s|a-zA-z0-9|\-|\+|\*|\(|\)|\|]*)");
    regex definitionMatcher(R"([\s]*[a-zA-z]*[\s]*:[\s|a-zA-z0-9|\-|\+|\*|\(|\)|\||\=|\<|>|\!\.|\/]*)");
    regex keywordsMatcher(R"(\{[\s]*([a-z]*[\s]*)*\})");
    regex punctuationMatcher(R"(\[[\s]*([\W]*[\s]*)*\])");
    while(getline(inFile, str)) {
        if(regex_match(str, expressionMatcher)) {
            parseExpression(str);
        }
        else if(regex_match(str, definitionMatcher)) {
            parseDefinition(str, priority++);
        }
        else if(regex_match(str, keywordsMatcher) ||
                regex_match(str, punctuationMatcher)) {
            keywordsPunctuationParsing(str);
        }
        else {
            cout << "RulesReader says: " + str + ": Invalid Rule (or empty line)\n";
        }
    }
    inFile.close();
}


/**
 * @brief parse a definition and add it to the regularExpressions map
 */
void RulesReader::parseExpression(const string& regularExpression) {
    // Example:
    // letter = a-z | A-Z
    int pos = static_cast<int>(regularExpression.find('='));
    string LHS = remove_spaces(regularExpression.substr(0, pos));
    string RHS = regularExpression.substr(pos + 1, regularExpression.size());
    if(regularExpression.find('-') != string::npos) {
        vector<char> alphabet = get_ranges(remove_spaces(RHS));
        rawRegularExpressions.insert({LHS, alphabet});
    }
    else {
        vector<string> RHSTokens = separateOperatorsAndOperands(RHS);
        for (auto & RHSToken : RHSTokens) {
            if (rawRegularExpressions.count(RHSToken) == 0 && !is_tokenizing_symbol(RHSToken)) {
                RHSToken = surround_parentheses(RHSToken);
            }
        }
        string concatenatedExpression = accumulate(RHSTokens.begin(), RHSTokens.end(), string(""));
        regularExpressions.insert({LHS, concatenatedExpression});
    }
}

void RulesReader::parseDefinition(const string& regularDefinition, int priority) {
    // Example:
    // id : letter (letter | digit)*
    int pos = static_cast<int>(regularDefinition.find(':'));
    string LHS = remove_spaces(regularDefinition.substr(0, pos));
    string RHS = regularDefinition.substr(pos + 1, regularDefinition.size());
    vector<string> RHSTokens = separateOperatorsAndOperands(RHS);
    for (int i = 0; i < RHSTokens.size(); i++) {
        string RHSToken = RHSTokens.at(i);
        if (RHSToken == "\\") {
            RHSToken.insert(0, "(");
            RHSTokens.at(i) = RHSToken;
            i++;
            RHSTokens.at(i) += ")";
        }
        else if (regularExpressions.count(RHSToken) != 0) {
            RHSTokens.at(i) = regularExpressions.at(RHSToken);
        }
        else if (rawRegularExpressions.count(RHSToken) == 0) {
            RHSTokens.at(i) = surround_parentheses(RHSToken);
        }
    }
    string concatenatedExpression = accumulate(RHSTokens.begin(), RHSTokens.end(), string(""));
    regularDefinitions.emplace_back(LHS, concatenatedExpression);
    tokens.insert({LHS,make_pair(priority,new NFA)});
}


void RulesReader::keywordsPunctuationParsing(string keyword) {
    keyword = keyword.substr(1, keyword.length() - 2);
    vector<string> keywordTokens = separateOperatorsAndOperands(keyword);
    for(int i = 0; i < keywordTokens.size(); i++) {
        string s = keywordTokens.at(i);
        if(s=="\\") {
            string keywordExpression= keywordTokens.at(i + 1) + ":" + s + keywordTokens.at(i + 1);
            parseDefinition(keywordExpression, -1);
            i++;
        }
        else {
            string keyword_expression = s;
            keyword_expression += ":";
            keyword_expression += s;
            parseDefinition(keyword_expression, -1);
        }
    }
}
