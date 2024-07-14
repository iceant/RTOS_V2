cmake_minimum_required(VERSION 3.5)
project(CodeAI C)

add_subdirectory(../SDK ${CMAKE_CURRENT_BINARY_DIR}/SDK)

########################################################################################################################

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
list(FILTER SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

if (SOURCES)
    add_library(${PROJECT_NAME} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC SDK)
endif ()

########################################################################################################################

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.t.c)
foreach (item IN LISTS TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] TestUnit: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()
