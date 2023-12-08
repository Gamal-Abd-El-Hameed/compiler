#include <bits/stdc++.h>
#ifndef PHASE_1_HELPER_H
#define PHASE_1_HELPER_H


static int id_generator=-1;


static string remove_spaces(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}


static vector<string> separateOperatorsAndOperands(const string& inputString) {
    vector<string> outputVector;
    regex regexPattern(R"([+()*\|\-,:?\s\\]+)"); // {+, *, (, ), |, \, ?}
    string currentToken;

    for (char currentChar : inputString) {
        if (regex_match(string(1, currentChar), regexPattern)) {
            if (!currentToken.empty()) {
                outputVector.push_back(currentToken);
            }
            if (currentChar != ' ') {
                outputVector.emplace_back(1, currentChar);
            }
            currentToken = "";
        } else {
            currentToken.append(string(1, currentChar));
        }
    }

    if (!currentToken.empty()) {
        outputVector.push_back(currentToken);
    }

    return outputVector;
}


static bool is_tokenizing_symbol(string c) {
    regex rgx(R"([+()*\|\?\s\\]+)"); // {+, *, (, ), |, \, ?}
    return regex_match(c,rgx);
}


static vector<string> AddConcatenations(vector<string> regexExpressionTokens) {
    for (int index = 1; index < regexExpressionTokens.size(); index++) {
        if (regexExpressionTokens.at(index) == "\\") {
            index++;
        } else if (regexExpressionTokens.at(index) == "(" &&
                   regexExpressionTokens.at(index - 1) != "|" &&
                   regexExpressionTokens.at(index - 1) != "(") {
            regexExpressionTokens.insert(regexExpressionTokens.begin() + index, string(1, '`'));
            index += 1;
        } else if (!is_tokenizing_symbol(regexExpressionTokens.at(index)) &&
                   (regexExpressionTokens.at(index - 1) == "*" ||
                    regexExpressionTokens.at(index - 1) == "+" ||
                    !is_tokenizing_symbol(regexExpressionTokens.at(index - 1)) ||
                    regexExpressionTokens.at(index - 1) == ")")) {
            regexExpressionTokens.insert(regexExpressionTokens.begin() + index, string(1, '`'));
            index += 1;
        }
    }
    return regexExpressionTokens;
}


static string surround_parentheses(string input){
    if(is_tokenizing_symbol(input)) return input;
    for(int i = 0; i < input.size(); i++) {
        input.insert(i, "(");
        input.insert(i + 2, ")");
        i += 2;
    }
    return input;
}

//return precedence of operators
static int prec(string c) {
    if (c == "*" || c == "+")
        return 3;
    else if (c == "`" )
        return 2;
    else if (c == "|" )
        return 1;
    else
        return -1;
}

//convert infix expression to postfix expression
static vector<string> infixToPostfix(vector<string>RE_expression_tokens) {
    stack<string> st;
    vector<string>postfix;

    for (int i = 0; i < RE_expression_tokens.size(); i++) {
        string c = RE_expression_tokens.at(i);
        if(c=="\\"){
            postfix.push_back(c);
            i++;
            postfix.push_back(RE_expression_tokens.at(i));
        }
        // If the scanned character is
        // an operand, add it to output string.
       else if (!is_tokenizing_symbol(c) && c != "`")
            postfix.push_back(c);
            // If the scanned character is an
            // ‘(‘, push it to the stack.
        else if (c == "(")
            st.push("(");
            // If the scanned character is an ‘)’,
            // pop and to output string from the stack
            // until an ‘(‘ is encountered.
        else if (c == ")") {
            while (st.top() != "(") {
                postfix.push_back(st.top());
                st.pop();
            }
            st.pop();
        }
            // If an operator is scanned
        else {
            while (!st.empty()
                   && prec(RE_expression_tokens[i]) <= prec(st.top())) {
                postfix.push_back(st.top());
                st.pop();
            }
            st.push(c);
        }
    }
    // Pop all the remaining elements from the stack
    while (!st.empty()) {
        postfix.push_back(st.top());
        st.pop();
    }
    return postfix;
}

static vector<char> get_ranges(string range) {
    range=remove_spaces(range);
    vector<char>result;
    while(range.find('-') != string::npos){
        int pos=range.find('-');
        int before=min(range[pos-1],range[pos+1]);
        int after=max(range[pos-1],range[pos+1]);
        range=range.substr(pos+1);
        for(int i=before;i<=after;i++){
            result.push_back(i);
        }
    }
    return result;
}
#endif //PHASE_1_HELPER_H
