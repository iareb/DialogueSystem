cmake_minimum_required(VERSION 3.8)

project("DialogueSystem")

# Executable
add_executable(DialogueSystem
    "main.cpp"
    "DialogueGraph.h"
 "DialogueParser.h")

# C++ standard
set_property(TARGET DialogueSystem PROPERTY CXX_STANDARD 20)

# MSVC debug info
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)

    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,\
            $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,\
            $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()