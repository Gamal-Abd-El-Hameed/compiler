#include <iostream>
#include "RulesReader.h"
#include "State.h"
#include "utility.h"
#include "NFA_Generator.h"
#include "InputReader.h"
#include "minimize.h"
#include "LL1.h"
#include "Parser.h"

int main() {
    RulesReader::readFile(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/compiler/grammar.txt)");
    NFA_Generator NFAGenerator;
    NFAGenerator.generateNfAs(RulesReader::regularDefinitions, RulesReader::rawRegularExpressions);

    InputReader inputReader;
    inputReader.readFile(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/compiler/input.txt)");

    LL1 ll1;
    ll1.read_grammar(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/compiler/grammar2.txt)");
    vector<string> input(inputReader.acceptedTokens.size());
    for (const pair<string,string>& pair:inputReader.acceptedTokens) {
        input.emplace_back(pair.first);
    }
    Parser parser;
    parser.Parse(input, &ll1);
    return 0 ;
}
