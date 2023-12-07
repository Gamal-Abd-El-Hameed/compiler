cmake_minimum_required(VERSION 3.23)
project(compiler)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(compiler
        DFA.cpp
        DFA.h
        helper.h
        InputReader.cpp
        InputReader.h
        main.cpp
        minimize.h
        NFA.cpp
        NFA.h
        NFA_Generator.cpp
        NFA_Generator.h
        RulesReader.cpp
        RulesReader.h
        SetStates.cpp
        SetStates.h
        State.cpp
        State.h)
