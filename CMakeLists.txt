cmake_minimum_required(VERSION 3.16)

project(ProxyOrm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) # Указываем стандарт с++
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON) # позволяет добавлять файлы ресурсов qrc без использования qt_add_resources

set(VERSION "0.0.1") # Необязательно, так можно указать версию проекта для своих нужд
add_definitions(-D APP_VERSION=\\"${VERSION}\\")

find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup(REQUIRES 6.5)

add_subdirectory(src/app)
add_subdirectory(src/proxyorm)
add_subdirectory(src/tests)
add_subdirectory(src/tests/TestProxyOrmValue)
