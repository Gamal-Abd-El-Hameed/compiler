#include <iostream>
#include "Parser.h"
#include "State.h"
#include "helper.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "Identifier.h"
#include "parser_generator.h"

int main(){
    Parser p;
    p.parseFile(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/ak/grammar.txt)");
    NFA_Generator g;
    g.generate_all_NFAs(Parser::RE_expressions,p.raw_RE_definitions);
    Identifier i;

    parser_generator pg;
    pg.read_file(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/ak/test.txt)");
    pg.removeLR();
    pg.left_factor();
    pg.get_parsing_table();
    // start writing to output_path

//    cout<<"___________________________________________Left derivation_________________________________________"<<endl;
    i.parse_string(R"(/media/gamal/01D5257AA3420B70/semester/compilers/project/ak/input.txt)",&pg);
        for(pair<string,string> pair:i.acceptedTokens){
//        cout<<pair.first<<":"<<pair.second<<endl;
    }

//    for(auto it = pg.table.cbegin(); it != pg.table.cend(); ++it){
//        std::cout << it->first << " " << ">>>>>> ";
//        for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2){
//            std::cout << it2->first << " " << it2->second;
//        }
//        std::cout << "\n";
//    }


}
