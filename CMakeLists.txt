cmake_minimum_required(VERSION 3.8)

project("DialogueSystem")

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(nlohmann_json)

# Executable
add_executable(DialogueSystem
    "main.cpp"
    "DialogueGraph.h"
 "DialogueParser.h")
target_link_libraries(DialogueSystem PRIVATE nlohmann_json::nlohmann_json)

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

file(GLOB DIALOGUE_FILES "${CMAKE_SOURCE_DIR}/files/*.json")

foreach(FILE ${DIALOGUE_FILES})
    get_filename_component(FILENAME ${FILE} NAME)
    configure_file(
            ${FILE}
            ${CMAKE_BINARY_DIR}/files/${FILENAME}
            COPYONLY
    )
endforeach()