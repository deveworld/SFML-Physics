cmake_minimum_required(VERSION 3.10)
project(sfml-first VERSION 0.1.0)

set(SFML_VERSION "2.5.1")

IF (WIN32)
    # Include local sfml cmake config
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
    # Link sfml statically (Optional)
    set(SFML_STATIC_LIBRARIES TRUE)
ENDIF()

find_package(SFML ${SFML_VERSION} COMPONENTS graphics system window REQUIRED)

file(GLOB SOURCES src/*.cpp)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES}
)

set(SFML_LIBS sfml-audio sfml-network sfml-system sfml-window sfml-graphics)

target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE "include" "lib")

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)