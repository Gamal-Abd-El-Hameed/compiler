#include "LL1.h"
#include "utility.h"

void LL1::read_grammar(const std::string &filepath) {
    ifstream inFile;
    inFile.open(filepath);
    string str;
    int priority = 0;
    while(getline(inFile, str)) {
        grammar.push_back(remove_spaces(str));
    }
    inFile.close();
}

string LL1::left_factoring(string LHS, string RHS) {
    vector<string> other, left_factor;
    vector<string> split_by_or = splitmyDelimeter(RHS, '|');
    for(string NT : split_by_or){
        int i=0;
        if(NT.length() >= LHS.length()) {
            while (i < NT.length() && LHS[i] == NT[i])
                i++;
        }
        if(i == LHS.length()){
            left_factor.push_back(NT.substr(i));
            left_factor.push_back("|");
        }else{
            other.push_back(NT);
            other.push_back("|");
        }
    }
    string newRHS = "";
    if(left_factor.size() > 0){
        newRHS += LHS + "(" + accumulate(left_factor.begin(), left_factor.end()-1, string("")) + ")";
        if(other.size() > 0)
            newRHS += "|";
    }
    if(other.size() > 0){
        newRHS += "(" + accumulate(other.begin(), other.end()-1, string("")) + ")";
    }
    return newRHS;
}

vector<string> LL1::remove_left_recursion(std::string LHS, std::string RHS) {
    //Assumption RHS here is RHS that was processed
    //by LL1::left_factoring
    vector<string> split_by_or = splitmyDelimeter(RHS, '|');
    if(split_by_or[0].length() <= LHS.length())
        return vector<string>{RHS};
    string Match = split_by_or[0].substr(0, LHS.length());
    if(LHS != Match)
        return vector<string>{RHS};
    string LHS2 = LHS + "'";
    string RHS1 = split_by_or[1] + LHS2;
    string RHS2 = split_by_or[0].substr(LHS.length()) + LHS2 + "|" + "Epsilon";
    string definition1 = LHS + ":" + RHS1;
    string defintion2 = LHS2 + ":" + RHS2;
    return vector<string>{definition1, defintion2};
}

void LL1::calc_first_set(vector<std::string> regular_definitions) {
    set<string> contains_epsilon;
    set<string> LHSs;
    for(string regularDefinition : regular_definitions) {
        int pos = static_cast<int>(regularDefinition.find('='));
        string LHS = remove_spaces(regularDefinition.substr(0, pos));
        string RHS = regularDefinition.substr(pos + 1, regularDefinition.size());
        LHSs.insert(LHS);
        if(RHS.find("Epsilon") >= 0 && RHS.find("Epsilon") < RHS.length())
            contains_epsilon.insert(LHS);
    }
    for(int i=regular_definitions.size()-1; i>=0; i--) {
        string regularDefinition = regular_definitions[i];
        int pos = static_cast<int>(regularDefinition.find('='));
        string LHS = remove_spaces(regularDefinition.substr(0, pos));
        string RHS = regularDefinition.substr(pos + 1, regularDefinition.size());
        vector<string> split_by_or = splitmyDelimeter(RHS, '|');
        for(string NT : split_by_or){
            NT = remove_spaces(NT);
            bool starts_with_terminal = false;
            for(string T : terminals){
                T = "\'" + T + "\'";
                if(NT.find(T) == 0) {
                    starts_with_terminal = true;
                    firstsets[LHS].insert(T);
                }
            }
            if(!starts_with_terminal){
                vector<string> internal_NTs;
                string token = "";
                for(int j=0; j<NT.length(); j++){
                    token += NT[j];
                    if(LHSs.count(token) > 0){
                        internal_NTs.push_back(token);
                        token = "";
                    }
                }
                int internal_NT_id = 0;
                while(internal_NT_id < internal_NTs.size() && contains_epsilon.count(internal_NTs[internal_NT_id]) > 0){
                    internal_NT_id++;
                }
                if(internal_NT_id < internal_NTs.size()){
                    firstsets[LHS].insert(firstsets[internal_NTs[internal_NT_id]].begin(), firstsets[internal_NTs[internal_NT_id]].end());
                }else{
                    firstsets[LHS].insert("Epsilon");
                }
            }


        }
    }
}

void LL1::calc_follow_set(vector<std::string> regular_definitions) {
    // insert '$' to the follow of the start symbol
    followsets[regular_definitions[0].substr(0, regular_definitions[0].find('='))].insert("$");
    for (const string &regularDefinition : regular_definitions) {
        int pos = static_cast<int>(regularDefinition.find('='));
        string LHS = remove_spaces(regularDefinition.substr(0, pos));
        string RHS = regularDefinition.substr(pos + 1, regularDefinition.size());
        vector<string> split_by_or = splitmyDelimeter(RHS, '|');

        for (string NT : split_by_or) {
            NT = remove_spaces(NT);
            vector<string> internal_NTs;
            string token;

            // Extract non-terminals from the right-hand side
            for (int j = 0; j < NT.length(); j++) {
                token += NT[j];
                if (terminals.find(token) == terminals.end() && token != "Epsilon") {
                    internal_NTs.push_back(token);
                    token = "";
                }
            }

            // Process internal non-terminals
            for (int j = 0; j < internal_NTs.size() - 1; j++) {
                string NT1 = internal_NTs[j];
                string NT2 = internal_NTs[j + 1];

                // Check if NT2 is a terminal, then add it to the follow set of NT1
                if (terminals.find(NT2) != terminals.end()) {
                    followsets[NT1].insert(NT2);
                }
                else {
                    // NT2 is a non-terminal, add
                    followsets[NT1].insert(firstsets[NT2].begin(), firstsets[NT2].end());

                    // If NT2 can derive epsilon, go to the next non-terminal and add its first set
                    // keep this loop until you reach a non-terminal that cannot derive epsilon
                    if (firstsets[NT2].count("Epsilon")) {
                        int k = j + 2;
                        while (k < internal_NTs.size() && firstsets[internal_NTs[k]].count("Epsilon")) {
                            followsets[NT1].insert(firstsets[internal_NTs[k]].begin(), firstsets[internal_NTs[k]].end());
                            k++;
                        }
                    }
                }
            }

            // Process the last internal non-terminal
            string NT1 = internal_NTs.back();
            followsets[NT1].insert(followsets[LHS].begin(), followsets[LHS].end());
            // backward loop to handle the case of multiple epsilon
            int j = static_cast<int>(internal_NTs.size()) - 2;
            while (j >= 0 && firstsets[internal_NTs[j + 1]].count("Epsilon")) {
                followsets[internal_NTs[j]].insert(followsets[LHS].begin(), followsets[LHS].end());
                j--;
            }
        }
    }
    // remove "Epsilon" from followsets if any
    for (auto &followset : followsets) {
        followset.second.erase("Epsilon");
    }
}

/**
 *  for each production rule A --> alpha of a grammar G
    1– for each terminal a in FIRST(alpha)
        add A --> alpha to M[A,a]
    2– If epsilon in FIRST(alpha)
        for each terminal a in FOLLOW(A) add A --> alpha to M[A,a]
    3– If epsilon in FIRST(alpha) and $ in FOLLOW(A)
        add A --> alpha to M[A,$]

 * 4- synchronization part:
 * for each terminal a in FOLLOW(A)
 *    add "sync" to M[A,a] if M[A,a] is empty
 */
void LL1::create_parsing_table() {
    for (const string &regularDefinition : grammar) {
        int pos = static_cast<int>(regularDefinition.find('='));
        string LHS = remove_spaces(regularDefinition.substr(0, pos));
        string RHS = regularDefinition.substr(pos + 1, regularDefinition.size());
        vector<string> split_by_or = splitmyDelimeter(RHS, '|');

        for (const string &production : split_by_or) {
            // Step 1: For each terminal a in FIRST(production), add the production to M[A, a]
            set<string> first_of_production = firstsets[production];
            for (const string &a : first_of_production) {
                if (a != "Epsilon") {
                    parsing_table[LHS][a] = production;
                }
            }

            // Step 2: If epsilon is in FIRST(production), add the production to M[A, a] for each terminal a in FOLLOW(A)
            if (first_of_production.count("Epsilon")) {
                set<string> follow_A = followsets[LHS];
                for (const string &a : follow_A) {
                    if (a != "Epsilon") {
                        parsing_table[LHS][a] = production;
                    }
                }
            }

            // Step 3: If epsilon is in FIRST(production) and $ is in FOLLOW(A), add the production to M[A, $]
            if (first_of_production.count("Epsilon") && followsets[LHS].count("$")) {
                parsing_table[LHS]["$"] = production;
            }
        }

        // Step 4: Synchronization part
        set<string> follow_A = followsets[LHS];
        for (const string &a : follow_A) {
            if (parsing_table[LHS].count(a) == 0) {
                parsing_table[LHS][a] = "sync";
            }
        }
    }
}

int main(){
    LL1 ll1;
    /*string LHS = "E";
    string RHS = "E+T|T";
    string newRHS = ll1.left_factoring(LHS, RHS);
    auto new_grammar = ll1.remove_left_recursion(LHS, newRHS);*/
    ll1.read_grammar("/home/mahmoud/MyComputer/compiler-lastnight/compiler/cfg_input.txt");
    vector<string> regular_definitions;
    ll1.terminals = {"num", "id", "int", "float", "if", "while", "else", "mulop", "relop", "+", "-", ";", "(", ")"};
    regular_definitions.push_back("METHOD_BODY::=STATEMENT_LIST");
    regular_definitions.push_back("STATEMENT_LIST::=STATEMENT | STATEMENT_LIST STATEMENT");
    regular_definitions.push_back("STATEMENT::=DECLARATION \
    | IF"
                                  "| WHILE"
                                  "| ASSIGNMENT");
    regular_definitions.push_back("DECLARATION::=PRIMITIVE_TYPE \'id\' \';\'");
    regular_definitions.push_back("PRIMITIVE_TYPE::=\'int\' | \'float\'");
    regular_definitions.push_back("IF::=\'if\' \'(\' EXPRESSION \')\' \'{\' STATEMENT \'}\' \'else\' \'{\' STATEMENT \'}\'");
    regular_definitions.push_back("WHILE::=\'while\' \'(\' EXPRESSION \')\' \'{\' STATEMENT \'}\'");
    regular_definitions.push_back("ASSIGNMENT::=\'id\' \'::=\' EXPRESSION \';\'");
    regular_definitions.push_back("EXPRESSION::=SIMPLE_EXPRESSION \
                                  | SIMPLE_EXPRESSION \'relop\' SIMPLE_EXPRESSION");
    regular_definitions.push_back("SIMPLE_EXPRESSION::=TERM | SIGN TERM | SIMPLE_EXPRESSION \'addop\' TERM");
    regular_definitions.push_back("TERM::=FACTOR | TERM \'mulop\' FACTOR");
    regular_definitions.push_back("FACTOR::=\'id\' | \'num\' | \'(\' EXPRESSION \')\'");
    regular_definitions.push_back("SIGN::=\'+\' | \'-\'");

    ll1.calc_first_set(ll1.grammar);
    regular_definitions.push_back("SIGN::=\'+\' | \'-\'");
}
