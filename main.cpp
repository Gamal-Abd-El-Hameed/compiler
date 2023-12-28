#include <iostream>
#include "RulesReader.h"
#include "State.h"
#include "utility.h"
#include "NFA_Generator.h"
#include "InputReader.h"
#include "minimize.h"
#include "LL1.h"

int main() {
    RulesReader::readFile(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/grammar.txt)");
    NFA_Generator NFAGenerator;
    NFAGenerator.generateNfAs(RulesReader::regularDefinitions, RulesReader::rawRegularExpressions);

    LL1 pg;
    pg.read_file(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/test.txt)");
    pg.removeLR();
    pg.left_factor();
    pg.get_parsing_table();
    InputReader inputReader;
    inputReader.parse_string(R"(/home/mahmoud/MyComputer/compiler-lastnight/compiler/input.txt)",&pg);
}
