cmake_minimum_required(VERSION 3.16)

# =========================
# COMPILER
# =========================
set(CMAKE_C_COMPILER clang CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER clang++ CACHE STRING "" FORCE)

project(meowc C CXX)

# =========================
# OPTIONS
# =========================
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# LLVM
# =========================
find_package(LLVM REQUIRED CONFIG)

message(STATUS "================ LLVM DEBUG ================")
message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM library dirs: ${LLVM_LIBRARY_DIRS}")
message(STATUS "============================================")

# =========================
# EXECUTABLE
# =========================
add_executable(meowc
    src/main.c
    src/lexer.c
    src/parser.c
    src/codegen.c
)

# =========================
# LLVM COMPONENTS
# =========================
llvm_map_components_to_libnames(
    LLVM_LIBS
    core
    support
    irreader
    native
)

message(STATUS "LLVM_LIBS: ${LLVM_LIBS}")

# =========================
# FIX: IMPORTANT PART
# =========================
# Вместо ручных include/defines — делаем LLVM частью target usage

target_include_directories(meowc SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(meowc PRIVATE ${LLVM_DEFINITIONS})

# =========================
# LINKING
# =========================
target_link_libraries(meowc PRIVATE ${LLVM_LIBS} stdc++)

target_compile_options(meowc PRIVATE
    -I${LLVM_INCLUDE_DIRS}
)
