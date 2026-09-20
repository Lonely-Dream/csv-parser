cmake_minimum_required(VERSION 3.15)
project(csv_gcc_compatibility LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

function(add_float_smoke name)
    add_executable(${name} smoke.cpp)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

if(CSV_COMPAT_SINGLE_HEADER_DIR)
    # Standalone consumers deliberately inherit no csv target definitions.
    find_package(Threads REQUIRED)
    foreach(name generated_float_smoke generated_float_optout)
        add_float_smoke(${name})
        target_include_directories(${name} PRIVATE "${CSV_COMPAT_SINGLE_HEADER_DIR}")
        target_link_libraries(${name} PRIVATE Threads::Threads)
    endforeach()
    target_compile_definitions(generated_float_optout PRIVATE
        CLASSIFY_SCALAR_DISABLE_STD_FLOAT_FROM_CHARS=1
        CSV_COMPAT_EXPECT_FLOAT_OPTOUT=1)
else()
    set(CSV_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(CSV_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
    set(BUILD_PYTHON OFF CACHE BOOL "" FORCE)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../../.." csv-library)
    foreach(library csv csv_no_simd)
        add_float_smoke(${library}_float_smoke)
        target_link_libraries(${library}_float_smoke PRIVATE ${library})
    endforeach()
endif()
