#include "NFA_Generator.h"
#include "helper.h"
#include "State.h"
NFA* NFA_Generator::total_NFA=new NFA(new State(),vector<State*>{});
void NFA_Generator::generate_all_NFAs(vector<pair<string,string>>RE_expression_pairs,map<string,vector<char>>raw_RE_definitions){
    for(pair<string,string> p:RE_expression_pairs){
        NFA* result= generate_NFA(p,raw_RE_definitions);
        this->NFAs.push_back(result);
    }
    combine_NFAs();
    cout<<"Done"<<endl;
}
NFA* NFA_Generator::generate_NFA(pair<string,string> RE_expression_pair,map<string,vector<char>>raw_RE_definitions) {
    string LHS=RE_expression_pair.first;
    string RHS=RE_expression_pair.second;
    vector<string>tokens=split_on_spacial_chars(RHS);
    vector<string>v=generate_infix(tokens);
    v= infixToPostfix(v);
    NFA* result= postfix_eval(v,raw_RE_definitions, LHS);
    return result;
}
NFA* NFA_Generator::postfix_eval(vector<string>postfix,map<string,vector<char>>RE_definitions,string accepted_type){
    stack<NFA*>st;
     for(int i=0;i<postfix.size();i++){
         string s=postfix.at(i);
         if((!is_spacial_character(s)&&s!="`" )||(s=="\\")){
            State* state = new State();
            State* end_state = new State();
            if(s=="\\") {
                if (postfix.at(++i) == "L") { // L is lambda
                    state->addNextState(end_state, vector<char>{'\0'});
                } else {
                    s = postfix.at(i);
                }
            }
             if(RE_definitions.find(s) != RE_definitions.end()) {
                 state->addNextState(end_state, RE_definitions.at(s));
             }else if (s != "\\") {
                 state->addNextState(end_state,vector<char>{s[0]});
             }
            NFA* currentNFA=new NFA(state,vector<State*>{end_state});
            st.push(currentNFA);
         }
         else{
             if(s=="*"){
                st.top()->closureNFA();
             }
             else if(s=="+"){
                 st.top()->positive_closureNFA();
             }
             else if(s=="|"){
                 NFA* NFA1=st.top();
                 st.pop();
                 NFA* NFA2=st.top();
                 st.pop();
                 NFA1->ORNFA(NFA2);
                 st.push(NFA1);
             }
             else if(s=="`"){
                 NFA* NFA1=st.top();
                 st.pop();
                 NFA* NFA2=st.top();
                 st.pop();
                 NFA2->concatenateNFA(NFA1);
                 st.push(NFA2);
             }
         }
     }
     NFA* accepted_NFA=st.top();
     State* accpeting_state=new State();
     accpeting_state->accepted = true;
     accpeting_state->tokenType= accepted_type;
     accepted_NFA->combine_end_states(accpeting_state);
     return accepted_NFA;
}
void NFA_Generator::combine_NFAs(){
    for(NFA* nfa:NFAs){
        total_NFA->start_state->addNextState(nfa->start_state,vector<char>{'\0'});
        total_NFA->transitions.insert(nfa->transitions.begin(), nfa->transitions.end());
        total_NFA->end_states.push_back(nfa->end_states[0]);
    }
}
