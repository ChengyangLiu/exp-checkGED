cmake_minimum_required(VERSION 3.2)

project(exp-checkGED)
SET(CMAKE_C_COMPILER g++)
add_compile_options(-std=c++11)

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
