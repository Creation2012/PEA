cmake_minimum_required(VERSION 3.0.0)
project(CMakeLists.txt VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(
    GraphMatrix
    GraphMatrix.h
    GraphMatrix.cpp
)

add_library(
    BruteForce 
    BruteForce.h
    BruteForce.cpp
)

add_library(
    HeldKarp 
    HeldKarp.h
    HeldKarp.cpp
)

add_library(
    BranchAndBound 
    BranchAndBound.h
    BranchAndBound.cpp
)

add_library(
    SimulatedAnnealing 
    SimulatedAnnealing.h
    SimulatedAnnealing.cpp
)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE GraphMatrix HeldKarp BranchAndBound SimulatedAnnealing)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_custom_command(TARGET main PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:main>)
include(CPack)