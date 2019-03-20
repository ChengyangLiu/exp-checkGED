cmake_minimum_required(VERSION 3.2)

project(exp-checkGED)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++14)

option(E_FIRST    "Set to switch to check edges' info first"  OFF)
option(V_FIRST    "Set to switch to check neighbors' info first" OFF)
option(BOOST_GRAPH    "Set to switch to use boost_graph"  OFF)
if(E_FIRST)
  add_definitions(-DE_FIRST)
  message(STATUS "Use E_FIRST")
elseif(V_FIRST)
  add_definitions(-DV_FIRST)
  message(STATUS "Use V_FIRST")
else()
  add_definitions(-DE_FIRST)
  message(STATUS "Use E_FIRST")
endif()

if(BOOST_GRAPH)
  add_definitions(-DBOOST_GRAPH)
  message(STATUS "Use Boost Graph Library")
endif()

find_package(Boost REQUIRED COMPONENTS filesystem)
if(NOT Boost_FOUND)
    message("Not found Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
message("${Boost_INCLUDE_DIRS}")
message("${Boost_LIBRARIES}")

include_directories(include)
aux_source_directory(src DIR_SRCS)

SET(SRC_PATH ${DIR_SRCS})
add_executable(${PROJECT_NAME} ${SRC_PATH})

target_link_libraries(exp-checkGED ${Boost_LIBRARIES})
