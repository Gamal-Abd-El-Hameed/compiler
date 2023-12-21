/*
#include <iostream>
#include "RulesReader.h"
#include "State.h"
#include "utility.h"
#include "NFA_Generator.h"
#include "InputReader.h"
#include "SetStates.h"
#include "DFA.h"
#include "minimize.h"

int main() {
    RulesReader::readFile(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/grammar2.txt)");
    NFA_Generator NFAGenerator;
    NFAGenerator.generateNfAs(RulesReader::regularDefinitions, RulesReader::rawRegularExpressions);

    InputReader inputReader;
    inputReader.readFile(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/input.txt)");

    fstream outfile;
    outfile.open(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/output.txt)",ios::out);
    for (const pair<string,string>& pair:inputReader.acceptedTokens) {
        outfile << pair.second << endl;
        cout << pair.first << " --> " << pair.second << endl;
    }

    DFA* finalDFA = new DFA();
    finalDFA->generateDFA(NFA_Generator::combinedNFA);
    vector<vector<SetStates*>> groups = minimizeDFA(finalDFA->remainingStates(), finalDFA->acceptingStates());
    DFA* minimizeDFA = generateMinDFA(groups);
    cout << "\nThe DFA has : " << finalDFA->stateIdToSetStatesMap.size() << " states.\n";
    //cout << "The minimized DFA has : " << minimizeDFA->stateIdToSetStatesMap.size() << " states.\n";
    minimizeDFA->writeToFile(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/minimizedDFA.txt)");
    return 0 ;
}
*/
