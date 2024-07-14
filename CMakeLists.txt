cmake_minimum_required(VERSION 3.5)
project(RTOS C)
enable_language(ASM)

option(USE_TLSF "USE TLSF AS MEMORY ALLOCATOR" ON)

add_subdirectory(CodeAI)
