cmake_minimum_required(VERSION 3.16)

project(proxyorm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
set(MY_PROJECT_LIBRARIES Qt6::Core)

add_library(proxyorm STATIC
    aggregatebyjoin.cpp
    aggregatebyjoin.h
    aggregatebyrow.cpp
    aggregatebyrow.h
    case.cpp
    case.h
    fromsource.cpp
    fromsource.h
    join.cpp
    join.h
    isource.cpp
    isource.h
    orwhere.cpp
    orwhere.h
    join.cpp
    join.h
    where.cpp
    where.h
    sort.cpp
    sort.h
    proxyormmodel.cpp
    proxyormmodel.h
    proxyormvalue.cpp
    proxyormvalue.h
)

cmake_minimum_required(VERSION 3.16)

project(proxyorm LANGUAGES CXX)


target_link_libraries(proxyorm PRIVATE ${MY_PROJECT_LIBRARIES})

target_include_directories(proxyorm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(proxyorm PUBLIC extern)
