cmake_minimum_required (VERSION 3.18.4)
message(STATUS "Cmake version: ${CMAKE_VERSION}")

project (nlb)

find_package(GTest REQUIRED)
find_package(nlohmann_json 3.9.1 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CLASSES_CPP src/main)

set(CLASSES src/network_load_balancer
            src/parser)

foreach(cs_cpp ${CLASSES_CPP})
    list(APPEND SOURCES ${cs_cpp}.cpp)
endforeach()

foreach(cs_р ${CLASSES_H})
    list(APPEND SOURCES ${cs_h}.h)
endforeach()

foreach(class ${CLASSES})
    list(APPEND SOURCES ${class}.cpp)
    list(APPEND HEADERS ${class}.h)
endforeach()

add_executable (
    ${PROJECT_NAME} ${HEADERS}
                    ${SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
)


add_executable (
        test_nlb tests/test_nlb.cpp
)

target_link_libraries(
    test_nlb PRIVATE
    gtest gtest_main -lpthread
)

set_target_properties (
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -W
    -Wall
    -Werror
    -Wextra
    -Wpedantic
)