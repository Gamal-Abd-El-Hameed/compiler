#include "Identifier.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "Parser.h"
void Identifier::parse_string(string filepath){
    ifstream inFile;
    inFile.open(filepath);
    string input_line;
    while(getline(inFile, input_line)) {
        while(!input_line.empty()){
            int index=parsing_single_token(input_line);
            input_line=index==-1?input_line.substr(1,input_line.size()):input_line.substr(index+1,input_line.size());
        }
    }

}
int Identifier::parsing_single_token(string input) {
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
    } else if(input[0]!=' ' && input[0]!='\n' && input[0]!='\t'){
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
