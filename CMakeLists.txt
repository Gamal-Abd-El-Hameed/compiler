cmake_minimum_required(VERSION 3.21)
project(Phase_1_)

set(CMAKE_CXX_STANDARD 14)

add_executable(Phase_1_ main.cpp Parser.cpp Parser.h State.cpp State.h helper.h NFA.cpp NFA.h NFA_Generator.cpp NFA_Generator.h Identifier.cpp Identifier.h parser_generator.cpp parser_generator.h)
