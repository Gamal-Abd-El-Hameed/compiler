#include <iostream>
#include "RulesReader.h"
#include "State.h"
#include "utility.h"
#include "NFA_Generator.h"
#include "InputReader.h"
#include "minimize.h"
#include "LL1.h"

int main() {
    RulesReader::readFile(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/compiler/grammar.txt)");
    NFA_Generator NFAGenerator;
    NFAGenerator.generateNfAs(RulesReader::regularDefinitions, RulesReader::rawRegularExpressions);

    LL1 pg;
    pg.read_file(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/compiler/test.txt)");
    pg.removeLR();
    pg.left_factor();
    pg.get_parsing_table();
    InputReader inputReader;
    inputReader.parse_string(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/compiler/input.txt)",&pg);
}
