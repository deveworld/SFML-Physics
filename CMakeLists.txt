cmake_minimum_required(VERSION 3.10)
project(sfml-physics VERSION 0.1.0)

set(SFML_VERSION "2.5.1")

find_package(SFML ${SFML_VERSION} COMPONENTS graphics system window REQUIRED)

file(GLOB SOURCES src/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
)

set(SFML_LIBS sfml-audio sfml-network sfml-system sfml-window sfml-graphics)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)