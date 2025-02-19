cmake_minimum_required(VERSION 3.10)
project(hw1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Python3 COMPONENTS Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
link_libraries(${Python3_LIBRARIES})

add_custom_target(dc
    COMMAND clang++ -std=c++17 -o data_clean ${CMAKE_SOURCE_DIR}/src/data_clean.cpp
    COMMAND ${CMAKE_SOURCE_DIR}/data_clean
    COMMAND rm ${CMAKE_SOURCE_DIR}/data_clean
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Cleaning dataset..."
)

add_custom_target(bf
    COMMAND clang++ -std=c++17 -o bf_spatial_search ${CMAKE_SOURCE_DIR}/src/bf_spatial_search.cpp
    COMMAND ${CMAKE_SOURCE_DIR}/bf_spatial_search
    COMMAND rm ${CMAKE_SOURCE_DIR}/bf_spatial_search
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running brute-force spatial search..."
)

add_custom_target(run
    COMMAND clang++ -std=c++17 -I${INCLUDE_DIR} -o spatial_search 
        ${SRC_DIR}/grid_index.cpp 
        ${SRC_DIR}/kd_tree.cpp 
        ${SRC_DIR}/main.cpp
    COMMAND ${CMAKE_SOURCE_DIR}/spatial_search
    COMMAND rm ${CMAKE_SOURCE_DIR}/spatial_search
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running Spatial Search (Grid Index + KD-Tree)..."
)
