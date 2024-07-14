cmake_minimum_required(VERSION 3.5)
project(TLSF C)

include(FetchContent)

FetchContent_Declare(tlsf
    URL http://www.gii.upv.es/tlsf/files/src/TLSF-2.4.6.tbz2
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(tlsf)
message(STATUS "tlsf_SOURCE_DIR: ${tlsf_SOURCE_DIR}")

file(GLOB SOURCES ${tlsf_SOURCE_DIR}/src/*.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

if (SOURCES)
    add_library(${PROJECT_NAME} ${SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${tlsf_SOURCE_DIR}/src)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_PRINTF=0)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TLSF_USE_LOCKS=1)
endif ()

