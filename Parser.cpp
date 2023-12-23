//
// Created by gamal on 23/12/23.
//

#include "Parser.h"
#include "utility.h"
#include <bits/stdc++.h>

using namespace std;

/**
 * there are 5 cases for the parsing table:
 * 1- if the top of the stack is a terminal and equal to the input, then pop from stack and get next input (match)
 * 2- if the top of the stack is a terminal and not equal to the input: error
 * and then insert the top of the stack to the input (missing)
 * 3- if the top of the stack is a non-terminal, and the cell in the parsing table contains a rule:
 * then pop from stack and push the rule to the stack in reverse order (expand)
 * 4- if the cell contains "sync": error
 * and then pop from stack
 * 5- if the cell is empty: error and then get next input
 * @param in
 * @param ll1
 */
void Parser::Parse(vector<string> in, LL1 *ll1) {
    stack_.emplace("$");
    stack_.emplace(ll1->start_symbol);
    parsing_table = ll1->parsing_table;
    input = std::move(in);
    input.emplace_back("$");
    int input_index = 0;
    while (!stack_.empty()) {
        string top = stack_.top();
        stack_.pop();
        // if the top of the stack is a terminal
        if (ll1->terminals.find(top) != ll1->terminals.end()) {
            // case 1: match
            if (top == input[input_index]) {
                cout << "match " << top << endl;
                input_index++;
            }
            // case 2: missing
            else {
                cout << "error: missing " << top << endl;
            }
        }
        // case 4: sync
        else if (parsing_table[top][input[input_index]] == "sync") {
            cout << "error: unexpected " << input[input_index] << endl;
            stack_.pop();
        }
        // case 5: empty
        else if (parsing_table[top][input[input_index]].empty()) {
            cout << "error: unexpected " << input[input_index] << endl;
            input_index++;
        }
        // case 3: expand
        else {
            vector<string> production = splitmyDelimeter(parsing_table[top][input[input_index]], ' ');
            cout << top << " -> ";
            for (const string &productionPart : production) {
                cout << productionPart << " ";
            }
            reverse(production.begin(), production.end());
            for (const string &productionPart : production) {
                stack_.emplace(productionPart);
            }
        }
    }
}