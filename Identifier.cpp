#include "Identifier.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "Parser.h"
#include "parser_generator.h"

void Identifier::parse_string(string filepath,parser_generator* pg){
    ifstream inFile;
    inFile.open(filepath);
    string input_line;
    stack<string> tempStack = stack<string>();
    tempStack.push("$");
    tempStack.push(pg->grammer_rules[0].first);
    while(getline(inFile, input_line)) {
        while(!input_line.empty()){
            int index=parsing_single_token(input_line,pg, tempStack);
            input_line=index==-1?input_line.substr(1,input_line.size()):input_line.substr(index+1,input_line.size());
        }
    }
    vector<string> res = pg->LL1_parse("$", tempStack);
    for (string str:res) {
        cout << str << endl;
    }
}

int Identifier::parsing_single_token(string input,parser_generator * pg, stack<string> &tempStack) {
    int accepted_index = -1;
    string accepted_token;
    string acceptedString;
    string accu;
    NFA *totalNFA = NFA_Generator::total_NFA;
    vector<State *> current_states = epsilonClosure(totalNFA->start_state);
    for (int i = 0; i < input.size(); i++) {
        accu+=input[i];
        if (current_states.empty())break;
        vector<State *> comming_states = nextAndEpsilonClosures(current_states, input[i]);
        int accepted_priority = INT16_MAX;
        for (State *s: comming_states) {
            if (s->accepted) {
                if (accepted_priority > Parser::tokens.at(s->tokenType).first) {
                    accepted_priority = Parser::tokens.at(s->tokenType).first;
                    accepted_index = i;
                    accepted_token = s->tokenType;
                    acceptedString = accu;
                }
            }
        }
        current_states = comming_states;
    }
    if (accepted_index > -1) {
        acceptedTokens.push_back(make_pair(acceptedString, accepted_token));
        //send this token to the LL1 parse
//        cout<<endl;
//        cout<<"## send "+accepted_token+"--->"<<endl;
//        cout<<endl;
        vector<string> res = pg->LL1_parse(accepted_token, tempStack);
        for (string str:res) {
            cout << str << endl;
        }
    } else if(input[0]!=' '){
        acceptedTokens.push_back(make_pair(to_string(input[0]), "Undefined"));
    }
    return accepted_index;
}
vector<State*> Identifier::nextAndEpsilonClosures(vector<State*>current_states,char input){
   set<State*> set;
   for(State* current_state:current_states) {
       if(current_state->nextStates.count(input)!=0) {
           vector<State *> next_state = current_state->nextStates.at(input);
           for (State *s: next_state) {
               vector<State *> vec = epsilonClosure(s);
               for (State *st: vec) set.insert(st);
           }
       }
   }
    vector<State*> v(set.begin(), set.end());
   return v;
}

vector<State*> Identifier::epsilonClosure(State* s){
    set<int> visited = {s->id};
    vector<State*>closure={s};
    for(int i=0;i<closure.size();i++){
        s=closure.at(i);
        map<char,vector<State*>> nextStates=s->nextStates;
        if(nextStates.count('\0')!=0) {
            vector<State *> vec = s->nextStates.at('\0');
            for (State *st: vec) {
                if (visited.count(st->id) == 0) {
                    closure.push_back(st);
                    visited.insert(st->id);
                }
            }
        }
        }
    return closure;
}
