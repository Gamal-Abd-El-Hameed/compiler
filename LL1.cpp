#include "LL1.h"
#include "utility.h"

bool debug = true;
void LL1::readFile(const string& filepath) {
    ifstream inFile(filepath);
    string str;

    const regex newRule(R"([\s]*[a-zA-z]*[\s]*=.*)");
    while (getline(inFile, str)) {
        if (regex_match(str, newRule)) {
            size_t pos = str.find('=');
            string LHS = remove_spaces(str.substr(0, pos));
            string RHS = str.substr(pos + 1);
            Grammar_rules.emplace_back(LHS, RHS);
        } else {
            Grammar_rules.back().second += str;
        }
    }
}

void LL1::eliminateLeftRecursion() {
    for (int i = 0; i < Grammar_rules.size(); i++) {
        for (int j = 0; j < i; j++) {
            substituteProductions(i, j);
        }
        eliminateImmediateLeftRecursion(i);
    }

    cout << "\nAfter eliminating Left Recursion:" << endl;
    cout << "_________________________________\n" << endl;

    printGrammarRules();
    writeGrammarRules("After eliminating Left Recursion:");
    cout << "_________________________________\n" << endl;
}

void LL1::eliminateImmediateLeftRecursion(int rule_index) {
    const auto& rule = Grammar_rules[rule_index];
    const auto& productions = split_on_spacial_chars(rule.second, regex(R"([\|])"));
    pair<string, string> dash{rule.first + "_dashLR", ""};
    string temp;

    for (const auto& production : productions) {
        if (production.find("|") != string::npos) continue;
        const auto& prod_parts = split_on_spacial_chars(production, regex(R"([\s]+)"));

        if (prod_parts[0] == rule.first) {
            const auto& accumulated = accumlator(subvector(prod_parts, 1, prod_parts.size()), " ");
            dash.second.append(accumulated + " " + dash.first + " |");
        } else {
            const auto& accumulated = accumlator(prod_parts, " ");
            temp.append(accumulated + " " + dash.first + " |");
        }
    }

    if (!dash.second.empty()) {
        Grammar_rules.at(rule_index).second = temp.substr(0, temp.size() - 1);
        dash.second.append(" Epsilon");
        Grammar_rules.emplace_back(dash);
    }
}

void LL1::substituteProductions(int ruleIndex1, int ruleIndex2) {
    pair<string, string> rule1 = Grammar_rules[ruleIndex1];
    pair<string, string> rule2 = Grammar_rules[ruleIndex2];
    vector<string> rule1Productions = split_on_spacial_chars(rule1.second, regex(R"([\|])"));

    for (string& rule1Production : rule1Productions) {
        vector<string> rule1ProductionParts = split_on_spacial_chars(rule1Production, regex(R"([\s]+)"));

        if (rule1ProductionParts[0] == rule2.first) {
            string accumulator = accumlator(subvector(rule1ProductionParts, 1, rule1ProductionParts.size()), " ");
            vector<string> rule2Productions = split_on_spacial_chars(rule2.second, regex(R"([\|])"));

            for (string& rule2Production : rule2Productions) {
                if (rule2Production != "|") {
                    rule2Production.append(" " + accumulator);
                }
            }

            string rule2Substitution = accumlator(rule2Productions, " ");
            rule1Production = rule2Substitution;
        }
    }

    string rule1ProductionsFinal = accumlator(rule1Productions, " ");
    Grammar_rules.at(ruleIndex1).second = rule1ProductionsFinal;
}

void LL1::leftFactor() {
    for (int index = 0; index < Grammar_rules.size(); index++) {
        string RHS = Grammar_rules[index].second;
        vector<string> productions = split_on_spacial_chars(RHS, regex(R"([\|])"));
        sort(productions.begin(), productions.end());

        map<string, vector<string>> groups;
        int groupIndex = -1;
        string currentGroup = remove_extra_spaces(productions[0]);
        int currentIndex = -1;
        string refactored;

        for (string &production : productions) {
            if (production.find("|") != string::npos) continue;

            string substr = getMatchSubstr(currentGroup, production);

            if (substr.empty()) {
                if (groupIndex < currentIndex - 1) {
                    vector<string> groupStrings = removeSubstr(subvector(productions, groupIndex + 1, currentIndex + 1), currentGroup);
                    groups.insert(make_pair(currentGroup, groupStrings));
                } else {
                    refactored.append(currentGroup + " |");
                }

                currentGroup = production;
                groupIndex = currentIndex;
            } else {
                currentGroup = substr;
            }

            currentIndex++;
        }

        if (groupIndex < currentIndex - 1) {
            vector<string> groupStrings = removeSubstr(subvector(productions, groupIndex + 1, currentIndex + 1), currentGroup);
            groups.insert(make_pair(currentGroup, groupStrings));
        } else {
            refactored.append(currentGroup + " |");
        }

        for (auto it = groups.begin(); it != groups.end(); ++it) {
            const auto& key = it->first;
            const auto& val = it->second;

            string cleanKey = key;
            if (isTerminal(cleanKey)) {
                cleanKey = remove_spaces(cleanKey);
                cleanKey.erase(remove(cleanKey.begin(), cleanKey.end(), '\''), cleanKey.end());
            }

            cleanKey = group_naming(cleanKey);
            refactored.append(key + " " + cleanKey + "_dashLF " + "|");
            Grammar_rules.push_back({cleanKey + "_dashLF", accumlator(val, "|")});
        }

        Grammar_rules[index].second = refactored.substr(0, refactored.size() - 1);
    }

    cout << "\nAfter Left Factoring:" << endl;
    cout << "_____________________\n" << endl;
    printGrammarRules();
    writeGrammarRules("After Left Factoring:");
    cout << "_________________________________\n" << endl;
}

string LL1::getMatchSubstr(const string& group, const string& p) {
    vector<string> spaceSplitGroup = split_on_spacial_chars(group, regex(R"([\s]+)"));
    vector<string> spaceSplitP = split_on_spacial_chars(p, regex(R"([\s]+)"));

    int minSize = min(spaceSplitP.size(), spaceSplitGroup.size());

    for (int i = 0; i < minSize; i++) {
        if (spaceSplitGroup[i] != spaceSplitP[i]) {
            return accumlator(subvector(spaceSplitP, 0, i), " ");
        }
    }

    if (p.size() < group.size()) {
        return p;
    }

    return group;
}

vector<string> LL1::removeSubstr(const vector<string>& vec, const string& str) {
    vector<string> result;

    for (const auto& s : vec) {
        string substrResult = s.substr(str.size());
        result.emplace_back(remove_extra_spaces(substrResult.empty() ? "Epsilon" : substrResult));
    }

    return result;
}

void LL1::getParsingTable() {
    for (const auto& p : Grammar_rules) {
        rules_map.insert({p.first, split_on_spacial_chars(p.second, regex(R"([\|])"))});
    }

    getFirstSets();
    getFollowSets();
    createTable();
    writeTableResults();
}

void LL1::getFirstSets() {
    stack<string> s;

    for (auto it = rules_map.begin(); it != rules_map.end(); ++it) {
        const auto& key = it->first;
        const auto& val = it->second;
        first_sets.insert({key, set<string>{}});
    }

    for (auto it = rules_map.begin(); it != rules_map.end(); ++it) {
        const auto& key = it->first;
        const auto& val = it->second;
        s.push(key);
        while (!s.empty()) {
            firstForOneKey(key, s);
        }
    }
}

void LL1::getFollowSets() {
    map<string, vector<string>> graph = getGraph();
    follow_sets[Grammar_rules[0].first].insert("$");
    vector<string> order = topological_sort(graph);

    for (auto it = order.begin(); it != order.end(); ++it) {
        const auto& str = *it;
        getFollowForOneKey(str, graph);
    }
}

void LL1::createTable() {
    for (auto it = rules_map.begin(); it != rules_map.end(); ++it) {
        const auto& key = it->first;
        const auto& val = it->second;

        for (auto itProd = val.begin(); itProd != val.end(); ++itProd) {
            const auto& production = *itProd;
            if (production.find("|") != string::npos) continue;
            if (production.find("Epsilon") != string::npos) continue;
            vector<string> prod_parts = split_on_spacial_chars(production, regex(R"([\s]+)"));

            if (isTerminal(prod_parts[0])) {
                if (table[key].count(prod_parts[0]) != 0) cout << "Not LL(1)" << endl;
                table[key][prod_parts[0]] = remove_extra_spaces(production);
            } else {
                for (const string &terminal : first_sets[prod_parts[0]]) {
                    if (table[key].count(terminal) != 0) cout << "Not LL(1)" << endl;
                    table[key][terminal] = remove_extra_spaces(production);
                }
            }
        }

        bool eps = hasEpsilon(val);
        for (const string &follow : follow_sets[key]) {
            if (table[key].count(follow) == 0 || table[key][follow] == "Sync") {
                table[key][follow] = eps ? "Epsilon" : "Sync";
            } else if (eps && table[key][follow] != "Epsilon") {
                cout << "Not LL(1) : Non-terminal(" + key + ") under input (" + follow + ") has production (" +
                        table[key][follow] + ") Then neglect the coming production (Epsilon)" << endl;
            }
        }
    }
}

void LL1::firstForOneKey(const string& key, stack<string>& s) {
    string current_key = s.top();
    s.pop();
    const vector<string>& productions = rules_map[current_key];

    for (const string& p : productions) {
        if (p.find("|") != string::npos) continue;

        vector<string> prod_parts = split_on_spacial_chars(p, regex(R"([\s]+)"));
        const string& firstSymbol = prod_parts[0];

        if (isTerminal(firstSymbol)) {
            if (firstSymbol.find("Epsilon") != string::npos) {
                if (hasEpsilon(rules_map[key])) {
                    first_sets[key].insert(firstSymbol);
                }
            } else {
                first_sets[key].insert(firstSymbol);
            }
        } else {
            s.push(firstSymbol);
            int i = 0;
            while (i < prod_parts.size() && hasEpsilon(rules_map[prod_parts[i]])) {
                if ((i + 1) < prod_parts.size()) {
                    s.push(prod_parts[i + 1]);
                    ++i;
                } else {
                    break;
                }
            }

            if (i == prod_parts.size() - 1 && hasEpsilon(rules_map[prod_parts[i]])) {
                first_sets[key].insert("Epsilon");
            }
        }
    }
}

bool LL1::isTerminal(string str) {
    regex terminal(R"('.*')");
    return regex_match(str,terminal)||str.find("Epsilon") != string::npos;
}

bool LL1::hasEpsilon(vector<string> prods) {
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
map<string, vector<string>> LL1::getGraph() {
    map<string, vector<string>> right_most;

    for (auto it = rules_map.begin(); it != rules_map.end(); ++it) {
        const string& key = it->first;
        const vector<string>& val = it->second;

        right_most[key] = {};

        for (auto prod_it = val.begin(); prod_it != val.end(); ++prod_it) {
            const string& prod = *prod_it;
            vector<string> prod_parts = split_on_spacial_chars(prod, regex(R"([\s]+)"));

            for (int j = prod_parts.size() - 1; j >= 0; --j) {
                if (prod_parts[j].find("Epsilon") != string::npos || prod_parts[j].find("|") != string::npos || isTerminal(prod_parts[j])) {
                    break;
                }

                if (prod_parts[j] != key) {
                    right_most[key].push_back(prod_parts[j]);
                }

                if (!hasEpsilon(rules_map[prod_parts[j]])) {
                    break;
                }
            }
        }
    }

    return right_most;
}

void LL1::getFollowForOneKey(string str, map<string, vector<string>> right_most) {
    for (auto it = rules_map.begin(); it != rules_map.end(); ++it) {
        const string& key = it->first;
        const vector<string>& val = it->second;

        for (auto prod_it = val.begin(); prod_it != val.end(); ++prod_it) {
            const string& prod = *prod_it;

            if (prod.find("|") != string::npos) {
                continue;
            }

            vector<string> prod_parts = split_on_spacial_chars(prod, regex(R"([\s]+)"));

            for (int i = 1; i < prod_parts.size(); i++) {
                if (prod_parts[i - 1] == str) {
                    if (isTerminal(prod_parts[i])) {
                        follow_sets[str].insert(prod_parts[i]);
                    } else {
                        follow_sets[str].insert(first_sets[prod_parts[i]].begin(), first_sets[prod_parts[i]].end());
                    }

                    if (follow_sets[str].find("Epsilon") != follow_sets[str].end()) {
                        follow_sets[str].erase("Epsilon");
                    }
                }
            }

            if (search_in_vector(right_most[key], str)) {
                follow_sets[str].insert(follow_sets[key].begin(), follow_sets[key].end());
            }
        }
    }
}

vector<string> LL1::LL1_parse(string input, stack<string>& s) {
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
        if(isTerminal(top)){
            if(top[0]=='\''){
                if(top.substr(1, input.size())==input){
                    s.pop();
                    if (debug)
                        result.push_back("Matched "+top);
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
                if (debug)
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

void LL1::printGrammarRules(){
    for(pair<string,string>p:Grammar_rules){
        cout<<p.first+" = "+p.second<<endl;
    }

}

void LL1::writeGrammarRules(const string& title) {
    fstream outFile(output_path, ios::out | ios::app);
    outFile << "\n" + title << "\n";

    for (const auto& rule : Grammar_rules) {
        outFile << rule.first << "," << rule.second << "\n";
    }

    outFile << "\n";
    outFile.flush();
    outFile.close();
}

void LL1::writeTableResults() {
    fstream outFile(output_path, ios::out | ios::app);

    outFile << "\nFirst_sets\n";
    for (auto it = first_sets.begin(); it != first_sets.end(); ++it) {
        outFile << it->first << ",";
        for (auto setIt = it->second.begin(); setIt != it->second.end(); ++setIt) {
            outFile << *setIt + " ";
        }
        outFile << "\n";
    }

    outFile << "\nFollow_sets:\n";
    for (auto it = follow_sets.begin(); it != follow_sets.end(); ++it) {
        outFile << it->first << ",";
        for (auto setIt = it->second.begin(); setIt != it->second.end(); ++setIt) {
            outFile << *setIt + " ";
        }
        outFile << "\n";
    }

    outFile << "\nParsing table:\n";
    for (auto it = table.begin(); it != table.end(); ++it) {
        outFile << it->first << "\n";
        for (auto prodIt = it->second.begin(); prodIt != it->second.end(); ++prodIt) {
            outFile << " ," + prodIt->first + "," + prodIt->second + "\n";
        }
    }

    outFile << "\n";
    outFile.flush();
    outFile.close();
}
