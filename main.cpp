#include <iostream>
#include "RulesReader.h"
#include "State.h"
#include "helper.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "Identifier.h"
#include "SetStates.h"
#include "DFA.h"
#include "minimize.h"

int main(){
    /*
    State* state0 = new State();
    State* state1 = new State();
    State* state2 = new State();
    State* state3 = new State();
    State* state4 = new State();
    State* state5 = new State();
    State* state6 = new State();
    State* state7 = new State();
    State* state8 = new State();
    State* state9 = new State();
    State* state10 = new State();
    state10->accepted = true ;


    state0->nextStates.insert({'\0',vector<State*>{state1,state7}});
    state1->nextStates.insert({'\0',vector<State*>{state4,state2}});
    state2->nextStates.insert({'a',vector<State*>{state3}});
    state3->nextStates.insert({'\0',vector<State*>{state6}});
    state4->nextStates.insert({'b',vector<State*>{state5}});
    state5->nextStates.insert({'\0',vector<State*>{state6}});
    state6->nextStates.insert({'\0',vector<State*>{state1,state7}});
    state7->nextStates.insert({'a',vector<State*>{state8}});
    state8->nextStates.insert({'b',vector<State*>{state9}});
    state9->nextStates.insert({'b',vector<State*>{state10}});

    SetStates s = SetStates(set<State*>{state0,state3});
    s.Eclosure();
    SetStates t = s.moveTo('a');
    cout << t.getStatesIds();


    NFA NFAfinal = NFA(state0,vector<State*>{state10});

    DFA DFAfinal = DFA();


    DFAfinal.generateDFA(&NFAfinal);
    cout<<"transition : "<<DFAfinal.transitions.size() << "\n" ;


    /*
    cout << "remaining states :\n\n";
    DFAfinal.remainingStates();

    cout << "fianl states :\n\n";
    DFAfinal.acceptingStates();


    vector<vector<SetStates*>> groups = minimizeDFA(DFAfinal.remainingStates(),DFAfinal.acceptingStates());

    cout << "-----------------------------------------\n\n" ;
    for (vector<SetStates*> group : groups){
        cout << "group\n" ;
        for (SetStates* ss : group)
            cout << ss->getStatesIds() << " ";
        cout << "\n--------------------\n\n";
    }


    generateMinDFA(groups);

*/
    RulesReader p;
    p.readFile(R"(/media/gamal/New Volume/semester/compilers/project/compiler/grammar.txt)");
    NFA_Generator g;
    g.generate_all_NFAs(RulesReader::regularDefinitions, p.rawRegularExpressions);
    Identifier i;
    i.parse_string(R"(/media/gamal/New Volume/semester/compilers/project/compiler/input.txt)");
    // write the accepted tokens to the output file
    fstream outfile;
    outfile.open(R"(/media/gamal/New Volume/semester/compilers/project/compiler/output.txt)",ios::out);
    for(pair<string,string> pair:i.acceptedTokens){
        outfile<<pair.second<<endl;
        cout<<pair.first<<" --> "<<pair.second<<endl;
    }

//    cout << RulesReader::tokens["if"].first << RulesReader::tokens["id"].first;

    DFA* DFAfinal = new DFA();
    NFA* NFAfinal = new NFA();
    NFAfinal = NFA_Generator::total_NFA ;
    DFAfinal->generateDFA(NFA_Generator::total_NFA);

    vector<vector<SetStates*>> groups = minimizeDFA(DFAfinal->remainingStates(),DFAfinal->acceptingStates());

    DFA* minimizeDFA = new DFA();

    minimizeDFA = generateMinDFA(groups);

    fstream newfile;
    string sourceinput ;
    newfile.open(R"(/media/gamal/New Volume/semester/compilers/project/compiler/input.txt)",ios::in); //open a file to perform read operation using file object

    if (newfile.is_open()){
        string tp;
        while(getline(newfile, tp)){
            sourceinput += ' ' + tp ;
        }
        newfile.close();
    }

    patternMatching(sourceinput,DFAfinal);


    cout << "\nThe DFA has : " << DFAfinal->transitions.size() << " states.\n";
    cout << "\nThe minimized DFA has : " << minimizeDFA->transitions.size() << " states.\n";

    minimizeDFA->writeToFile(R"(/media/gamal/New Volume/semester/compilers/project/compiler/minimizedDFA.txt)");
    return 0 ;
}
