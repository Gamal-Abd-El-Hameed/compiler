#include "parser_generator.h"
#include "helper.h"
#include "stack"

void parser_generator::read_file(string filepath) {
    ifstream inFile;
    inFile.open(filepath);
    string str;
    regex new_rule(R"([\s]*[a-zA-z]*[\s]*=.*)");
    string LHS;
    while(getline(inFile, str)){
        if(regex_match(str,new_rule)){
            int pos=str.find('=');
            LHS = remove_spaces(str.substr(0,pos));
            string RHS=  str.substr(pos+1,str.size());
            grammer_rules.emplace_back(make_pair(LHS,RHS));
        }
        else{
            grammer_rules.back().second.append(str);
        }
    }
}

void parser_generator::removeLR() {
    for(int i = 0; i<grammer_rules.size(); i++){
        for(int j=0; j<i; j++){
            substitute(i,j);
        }
        eliminate_immediate_LR(i);
    }
//    cout<<"\nAfter eliminate Left Recursion:"<<endl;
//    cout<<"_______________________________\n"<<endl;
//    print_grammer_rules();
//    write_grammer_rules("After eliminate Left Recursion:");
}
void  parser_generator::eliminate_immediate_LR(int rule_index){
    pair<string,string> rule=grammer_rules[rule_index];
    vector<string> productions = split_on_spacial_chars(rule.second, regex(R"([\|])"));
    pair<string,string>dash= make_pair(rule.first+"_dashLR","");
    string temp;
     for(int i=0;i<productions.size();i++){
         if(productions[i].find("|") != string::npos)continue;
         vector<string> prod_parts = split_on_spacial_chars(productions[i],regex(R"([\s]+)"));
         if(prod_parts[0]==rule.first){
             string accu=accumlator(subvector(prod_parts,1,prod_parts.size()), " ");
             dash.second.append(accu.append(" "+dash.first+" |"));
         }else{
             string accu=accumlator(prod_parts, " ");
             temp.append(accu+" "+dash.first+" |");
         }
     }
     if(!dash.second.empty()){
        grammer_rules.at(rule_index).second=temp.substr(0,temp.size()-1);
        dash.second.append(" Epsilon");
        grammer_rules.emplace_back(dash);
     }
}
void parser_generator::substitute(int i,int j){
    pair<string,string> rule_i=grammer_rules[i];
    pair<string,string> rule_j=grammer_rules[j];
    vector<string>rule_i_productions= split_on_spacial_chars(rule_i.second, regex(R"([\|])"));
    for(int i=0; i < rule_i_productions.size(); i++){
        string rule_i_prod=rule_i_productions[i];
        vector<string> rule_i_prod_part= split_on_spacial_chars(rule_i_prod,regex(R"([\s]+)"));
        if(rule_i_prod_part[0]==rule_j.first){
            string accu=accumlator(subvector(rule_i_prod_part,1,rule_i_prod_part.size()), " ");
            vector<string>rule_j_productions=split_on_spacial_chars(rule_j.second, regex(R"([\|])"));
            for(int j=0;j<rule_j_productions.size();j++){
                if(rule_j_productions[j]!="|")
                    rule_j_productions[j].append(" "+accu);
            }
            string rule_j_substitution=accumlator(rule_j_productions, " ");
            rule_i_productions[i]=rule_j_substitution;
        }
    }
    string rule_i_prod_final=accumlator(rule_i_productions, " ");
    grammer_rules.at(i).second=rule_i_prod_final;
}


void parser_generator::left_factor() {
    for (int index = 0; index < grammer_rules.size(); index++) {
        string RHS = grammer_rules[index].second;
        vector <string> productions = split_on_spacial_chars(RHS, regex(R"([\|])"));
        sort(productions.begin(), productions.end());
        map <string, vector<string>> groups;
        int group_index = -1;
        string group = remove_extra_spaces(productions[0]);
        int current = -1;
        string refactored;
        for (string p: productions) {
            if (p.find("|") != string::npos)continue;
            string substr = get_match_substr(group, p);
            if (substr.size() == 0) {
                if (group_index < current - 1) {
                    vector <string> group_str = remove_substr(subvector(productions, group_index + 1, current + 1),group);
                    groups.insert({group, group_str});
                } else {
                    refactored.append(group + " |");
                }
                group = p;
                group_index = current;
            } else {
                group = substr;
            }
            current++;
        }
        if (group_index < current - 1) {
            vector <string> group_str = remove_substr(subvector(productions, group_index + 1, current + 1), group);

            groups.insert({group, group_str});
        } else {
            refactored.append(group + " |");
        }
        for (auto const&[key, val]: groups) {
            string clean_key=key;
            if(is_terminal(clean_key)){
                clean_key=remove_spaces(clean_key);
                clean_key.erase(remove(clean_key.begin(), clean_key.end(), '\''),clean_key.end());
            }
            clean_key= group_naming(clean_key);
            refactored.append(key + " " + clean_key + "_dashLF " + "|");
            grammer_rules.push_back({clean_key + "_dashLF", accumlator(val, "|")});
        }
        grammer_rules[index].second = refactored.substr(0, refactored.size() - 1);
    }
//    cout<<"\nAfter Left Factoring:"<<endl;
//    cout<<"_____________________\n"<<endl;
//    print_grammer_rules();
//    write_grammer_rules("After Left Factoring:");
}

string parser_generator::get_match_substr(string group,string p){
    vector<string>space_split_group= split_on_spacial_chars(group,regex(R"([\s]+)"));
    vector<string>space_split_p= split_on_spacial_chars(p,regex(R"([\s]+)"));
    for(int i=0;i<min(space_split_p.size(),space_split_group.size());i++){
        if(space_split_group[i]!=space_split_p[i]){
           return accumlator(subvector(space_split_p,0,i)," ");
        }
    }
    if(p.size()<group.size()) return p;
    return group;
}
vector<string> parser_generator::remove_substr(vector<string>vec,string str){
    vector<string>result;
    for(int i=0;i<vec.size();i++){
        result.emplace_back(vec[i].substr(str.size(),vec[i].size()));
        if(remove_extra_spaces(result[i])==""){
            result[i]="Epsilon";
        }
    }
    return result;
}

void parser_generator::get_parsing_table() {
    for(pair<string,string> p:grammer_rules){
        rules_map.insert({p.first, split_on_spacial_chars(p.second,regex(R"([\|])"))});
    }
    get_first_sets();
    get_follow_sets();
    create_table();
    write_table_results();
}

void parser_generator::get_first_sets(){
    stack<string> s;
    for (auto const&[key, val]: rules_map) {
        first_sets.insert({key,set<string>{}});
    }
    for (auto const&[key, val]: rules_map) {
        s.push(key);
        while(!s.empty()){
            first_for_one_key(key,s);
        }
    }

}
void parser_generator::get_follow_sets(){
    map<string,vector<string>> graph = get_graph();
    follow_sets[grammer_rules[0].first].insert("$");
    vector<string> order = topological_sort(graph);
    //follow_sets[order[0]].insert("$");
    for(string str:order){
        get_follow_for_one_key(str,graph);
    }
}

void parser_generator::create_table(){
//    cout<<"\nCreate Parsing Table:"<<endl;
//    cout<<"_____________________\n"<<endl;
    for(auto const&[key, val]: rules_map) {
        for(string production:val){
            if (production.find("|") != string::npos)continue;
            if(production.find("Epsilon") != string::npos) continue;
            vector<string> prod_parts = split_on_spacial_chars(production,regex(R"([\s]+)"));
            if(is_terminal(prod_parts[0])){
                if(table[key].count(prod_parts[0])!=0)cout<<"Not LL(1)"<<endl;
                table[key][prod_parts[0]]=remove_extra_spaces(production);
            } else{
                for(string terminal:first_sets[prod_parts[0]]){
                    if(table[key].count(terminal)!=0)cout<<"Not LL(1)"<<endl;
                    table[key][terminal]=remove_extra_spaces(production);
                }
            }
        }
        bool eps=has_epsilon(rules_map[key]);
            for(string follow:follow_sets[key]){
                if (table[key].count(follow)==0 || table[key][follow]=="Sync") {
                    table[key][follow] = eps ? "Epsilon" : "Sync";
                }
                else if(eps && table[key][follow]!="Epsilon"){
                    cout << "Not LL(1) : Non-terminal("+key+") under input ("+follow+") has production ("+ table[key][follow] +") Then neglect the coming production (Epsilon)"<< endl;
                }
            }
    }
}
void parser_generator::first_for_one_key(const string& key,stack <string>& s) {
    string current_key=s.top();
    s.pop();
    vector<string> productions = rules_map[current_key];
     for(string p:productions){
         if (p.find("|") != string::npos)continue;
         vector<string> prod_parts = split_on_spacial_chars(p,regex(R"([\s]+)"));
         if(is_terminal(prod_parts[0])){
             if(prod_parts[0].find("Epsilon") != string::npos) {
                 if(has_epsilon(rules_map[key])) {
                     first_sets.at(key).insert(prod_parts[0]);
                 }
             }
             else{
                 first_sets.at(key).insert(prod_parts[0]);
             }
         }else{
             s.push(prod_parts[0]);
             int i;
             for(i=0;i<prod_parts.size();i++){
                 if(has_epsilon(rules_map[prod_parts[i]]) && (i+1)<prod_parts.size()){
                     s.push(prod_parts[i+1]);
                 }else{
                     break;
                 }
             }
             if(i==prod_parts.size()-1 && has_epsilon(rules_map[prod_parts[i]])){
                 first_sets.at(key).insert("Epsilon");
             }
         }
     }
}

bool parser_generator::is_terminal(string str) {
    regex terminal(R"('.*')");
    return regex_match(str,terminal)||str.find("Epsilon") != string::npos;
}

bool parser_generator::has_epsilon(vector<string> prods) {
    for (string s:prods) {
        s=remove_spaces(s);
        if(s.find("Epsilon") != string::npos)return true;
    }
    return false;
}

/**
 * @brief get the right most non-terminal for each non-terminal
 * @return
 */
map<string,vector<string>> parser_generator::get_graph(){
    map<string,vector<string>> right_most;
    for(auto const&[key, val]:rules_map){
        right_most.insert({key,vector<string>{}});
        for(string prod:val){
            vector<string> prod_parts = split_on_spacial_chars(prod,regex(R"([\s]+)"));
            for(int j=prod_parts.size()-1;j>=0;j--){
                if(prod_parts[j].find("Epsilon") != string::npos||prod_parts[j].find("|") != string::npos||is_terminal(prod_parts[j])) {
                    break;
                }
                if(prod_parts[j]!=key) {
                    right_most[key].push_back(prod_parts[j]);
                }
                if (!has_epsilon(rules_map[prod_parts[j]]))
                    break;
            }
        }
    }
    return right_most;
}

void parser_generator::get_follow_for_one_key(string str,map<string,vector<string>> right_most) {
    for (auto const &[key, val]: rules_map) {
        for (string prod: val) {
            if (prod.find("|") != string::npos)continue;
            vector <string> prod_parts = split_on_spacial_chars(prod, regex(R"([\s]+)"));
            for (int i = 1; i < prod_parts.size(); i++) {
                if (prod_parts[i - 1] == str) {
                    if (is_terminal(prod_parts[i])) {
                        follow_sets[str].insert(prod_parts[i]);
                    } else {
                        follow_sets[str].insert(first_sets[prod_parts[i]].begin(), first_sets[prod_parts[i]].end());//
                    }
                    if(follow_sets[str].find("Epsilon")!=follow_sets[str].end())
                        follow_sets[str].erase("Epsilon");
                    }
            }

            if(search_in_vector(right_most[key],str)){//
                follow_sets[str].insert(follow_sets[key].begin(),follow_sets[key].end());//
            }
        }
    }
}
void parser_generator::print_grammer_rules(){
    for(pair<string,string>p:grammer_rules){
//        cout<<p.first+" = "+p.second<<endl;
    }
}
void parser_generator::write_grammer_rules(string title){
    fstream fout;
    fout.open(output_path,ios::out | ios::app);
    fout<<"\n"+title<<"\n";
    for(pair<string,string>p:grammer_rules){
        fout<<p.first+","+p.second<<"\n";
    }
    fout<<"\n";
    fout.flush();
    fout.close();
}
void parser_generator::write_table_results(){
    fstream fout;
    fout.open(output_path,ios::out | ios::app);
    fout<<"\nFirst_sets"<<"\n";

    for(pair<string,set<string>>p:first_sets){
        fout<<p.first+",";
        for(string s:p.second){
            fout<<s+" ";
        }
        fout<<"\n";
    }
    fout<<"\nFollow_sets:"<<"\n";

    for(pair<string,set<string>>p:follow_sets){
        fout<<p.first+",";
        for(string s:p.second){
            fout<<s+" ";
        }
        fout<<"\n";
    }
    fout<<"\nParsing table:"<<"\n";


    for(auto const&[key, val]:table){
        fout<<key+"\n";
        for(auto const&[key_2, val_2]:val){
            fout<<" ,"+key_2+","+val_2+"\n";
        }
    }
    fout<<"\n";

    fout.flush();
    fout.close();

}

vector<string> parser_generator::LL1_parse(string input, stack<string>& s) {
    vector<string> result;
    bool flag = true;
    while(flag){
        string top=s.top();
        if(top=="$"){
            if(input == "$"){
                result.push_back("Token Accepted");
                return result;
            }else{
                result.push_back("Token Rejected");
                return result;
            }
        }
        if(is_terminal(top)){
            if(top[0]=='\''){
                if(top.substr(1, input.size())==input){
                    s.pop();
//                    result.push_back("Matched "+top);
                    flag = false;
                }else{
                    result.push_back("Error: missing "+ top + ", inserted");
                    s.pop();
                }
            }else{
                result.push_back("Rejected");
                return result;
            }
        }else{
            string terminal = "\'" + input + "\'";
            if(terminal == "\'$\'") terminal="$";
            if(table[top].count(terminal)==0){
                result.push_back("Error:(illegal " + top + ")" +  " - " + "discarded " + terminal);
                flag = false;
                continue;
            }
            string production=table[top][terminal];
            if(production=="Sync"){
                result.push_back("Sync");
                s.pop();
            }
            else if(production=="Epsilon"){
                result.push_back(top+"->"+production);
                s.pop();
            }else{
                s.pop();
                vector<string> prod_parts = split_on_spacial_chars(production,regex(R"([\s]+)"));
                result.push_back(top+"->"+production);
                for(int j=prod_parts.size()-1;j>=0;j--){
                    s.push(prod_parts[j]);
                }
            }
        }
    }
    return result;
}
