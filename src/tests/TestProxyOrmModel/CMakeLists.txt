cmake_minimum_required(VERSION 3.16)

project(TestProxyOrmModel LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Gui)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




add_executable(TestProxyOrmModel tst_testproxyormmodel.cpp)
target_link_libraries(TestProxyOrmModel PRIVATE Qt6::Test Qt6::Gui helpers proxyorm)

add_executable(TestFromSource tst_testfromsource.cpp)
target_link_libraries(TestFromSource PRIVATE Qt6::Test Qt6::Gui helpers proxyorm)

add_executable(TestJoin tst_testjoin.cpp)
target_link_libraries(TestJoin PRIVATE Qt6::Test Qt6::Gui helpers proxyorm)

add_executable(TestAggregate tst_testaggregatebyrow.cpp)
target_link_libraries(TestAggregate PRIVATE Qt6::Test Qt6::Gui helpers proxyorm)

add_executable(TestCase tst_testcase.cpp)
target_link_libraries(TestCase PRIVATE Qt6::Test Qt6::Gui helpers proxyorm)

add_test(NAME TestProxyOrmModel COMMAND TestProxyOrmModel)

target_link_libraries(TestProxyOrmModel PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Gui proxyorm helpers)

