cmake_minimum_required(VERSION 3.5)
project(Compiler C)


########################################################################################################################
if (CMAKE_C_COMPILER_ID MATCHES GNU)
    set(COMPILER_TYPE GCC)
else ()
    message(FATAL_ERROR "Please specify 'COMPILER_TYPE' !!!")
endif ()

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${COMPILER_TYPE}/*.c)
list(FILTER SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

if (SOURCES)
    add_library(${PROJECT_NAME} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${COMPILER_TYPE})
endif ()
