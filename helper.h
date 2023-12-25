#include <bits/stdc++.h>
#ifndef PHASE_1__HELPER_H
#define PHASE_1__HELPER_H
static int id_generator=0;
static string output_path=R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/ak/result.txt)";
static string remove_spaces(string input){
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}
static string remove_leading_spaces(string str){
    int i;
    for(i=0;i<str.size();i++){
          if(str[i]!=' ') break;
    }
    return str.substr(i,str.size());
}

static vector<string> split_on_spacial_chars(string str,regex rgx=regex(R"([+()*\|\-,:?\s\\]+)")) {
    vector<string> result;
    string current_string="";
    for(char c:str){
        if(regex_match(string(1,c),rgx)){
            if(!current_string.empty()){
                result.push_back(remove_leading_spaces(current_string));
            }
            if(c!=' '){
                result.push_back(string(1, c));
            }
           current_string="";
        }
        else{
            current_string.append(string(1,c));
        }
    }
    if(!current_string.empty())result.push_back(remove_leading_spaces(current_string));
    return result;
}
static bool is_spacial_character(string c){
    regex rgx(R"([+()*\|\?\s\\]+)");
    return regex_match(c,rgx);
}
static vector<string> generate_infix(vector<string>RE_expression_tokens) {
    for (int i = 1; i < RE_expression_tokens.size(); i++) {
        if(RE_expression_tokens.at(i)=="\\"){
            i++;
        }
        else if (RE_expression_tokens.at(i) == "(" && RE_expression_tokens.at(i-1)!="|" && RE_expression_tokens.at(i-1)!="(") {
            RE_expression_tokens.insert(RE_expression_tokens.begin() + i, string(1,'`'));
            i+=1;
        } else if (!is_spacial_character(RE_expression_tokens.at(i)) &&
                   (RE_expression_tokens.at(i - 1) == "*" || RE_expression_tokens.at(i - 1) == "+" || !is_spacial_character(RE_expression_tokens.at(i - 1)) || RE_expression_tokens.at(i - 1) == ")")) {
            RE_expression_tokens.insert(RE_expression_tokens.begin() + i, string(1,'`'));
            i+=1;
        }
    }
    return RE_expression_tokens;
}
static string surround_parentheses(string input){
    if(is_spacial_character(input)) return input;
    for(int i = 0; i < input.size(); i++) {
        input.insert(i, "(");
        input.insert(i + 2, ")");
        i += 2;
    }
    return input;
}

//return precedence of operators
static int prec(string c)
{
    if (c == "*"|| c=="+")
        return 3;
    else if (c == "`" )
        return 2;
    else if (c == "|" )
        return 1;
    else
        return -1;
}

//convert infix expression to postfix expression
static vector<string> infixToPostfix(vector<string>RE_expression_tokens)
{
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
       else if (!is_spacial_character(c) && c!="`")
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
static string accumlator(vector<string>input,string delimter){
    string result="";
    for(string s:input){
        result.append(s+delimter);
    }
    return result.substr(0,result.size()-1);

}
static vector<string> subvector(vector<string>v, int m, int n) {
    vector<string>result;
    for(int i=m;i<n;i++){
        result.emplace_back(v[i]);
    }
    return result;
}
static bool search_in_vector(vector<string>vec,string str){
    return (find(vec.begin(), vec.end(), str) != vec.end());
}
// A recursive function used by topologicalSort
static void topological_sortUtil(string v, map<string,bool>& visited,
                                stack<string>& Stack,map<string,vector<string>>graph)
{
    // Mark the current node as visited.
    visited[v] = true;

    // Recur for all the vertices
    // adjacent to this vertex
    vector<string>::iterator i;
    for (i = graph[v].begin(); i != graph[v].end(); ++i)
        if (!visited[*i])
            topological_sortUtil(*i, visited, Stack,graph);

    // Push current vertex to stack
    // which stores result
    Stack.push(v);
}

// The function to do Topological Sort.
// It uses recursive topologicalSortUtil()
static vector<string> topological_sort(map<string,vector<string>>graph )
{
    vector<string> result;
    stack<string> Stack;
    // Mark all the vertices as not visited
    map<string,bool> visited;
    for(auto const &[key, val]:graph){
        visited.insert({key,false});
    }

    // Call the recursive helper function
    // to store Topological
    // Sort starting from all
    // vertices one by one
    for (auto const &[key, val]:graph)
        if (!visited[key])
            topological_sortUtil(key, visited, Stack,graph);

    // Print contents of stack
    while (!Stack.empty()) {
//        cout << Stack.top() << " ";
        result.push_back(Stack.top());
        Stack.pop();
    }
    return result;

}
static string group_naming(string str){
    vector<string>parts= split_on_spacial_chars(str,regex(R"([\s]+)"));
    string result="";
    for(int i=0;i<parts.size()-1;i++){
        result+=parts[i]+"_";
    }
    result+=parts[parts.size()-1];
    return result;
}
static string remove_extra_spaces(string str)
{
    str=str+" ";
    int n = str.length();
    int i = 0, j = -1;
    bool spaceFound = false;
    while (++j < n && str[j] == ' ');
    while (j < n)
    {
        if (str[j] != ' ')
        {
            if ((str[j] == '.' || str[j] == ',' ||
                 str[j] == '?') && i - 1 >= 0 &&
                str[i - 1] == ' ')
                str[i - 1] = str[j++];
            else
                str[i++] = str[j++];
            spaceFound = false;
        }
        else if (str[j++] == ' ')
        {
            if (!spaceFound)
            {
                str[i++] = ' ';
                spaceFound = true;
            }
        }
    }
    if (i <= 1)
        str.erase(str.begin() + i, str.end());
    else
        str.erase(str.begin() + i - 1, str.end());
    return str;
}

#endif //PHASE_1__HELPER_H
