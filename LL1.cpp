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
                    firstsets[LHS].push_back(T);
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
                    firstsets[LHS].insert(firstsets[LHS].end(), firstsets[internal_NTs[internal_NT_id]].begin(), firstsets[internal_NTs[internal_NT_id]].end());
                }else{
                    firstsets[LHS].push_back("Epsilon");
                }
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
