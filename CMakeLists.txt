cmake_minimum_required(VERSION 3.16)

project(meowc C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(meowc
    src/main.c
    src/lexer.c
    src/parser.c
    src/codegen.c
)

llvm_map_components_to_libnames(
    LLVM_LIBS
    core
    support
    irreader
    native
)

target_link_libraries(
    meowc
    ${LLVM_LIBS}
    stdc++
)
