find_package(Qt6 REQUIRED COMPONENTS Core Test Gui)

add_library(helpers STATIC
    src/standardlistmodel.cpp
    src/standardlistmodel.h
)

target_include_directories(helpers PUBLIC src)
target_link_libraries(helpers Qt6::Core Qt6::Gui)

add_subdirectory(TestProxyOrmValue)
add_subdirectory(TestWhere)
add_subdirectory(TestProxyOrmModel)

