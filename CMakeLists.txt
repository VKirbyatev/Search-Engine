cmake_minimum_required(VERSION 3.17)
project(clowndex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB CLOWNDEX_FILES "Clowndex/*.cpp" "Clowndex/indices/*.cpp" "Clowndex/SearhEngine/*.cpp")

add_executable(${PROJECT_NAME} ${CLOWNDEX_FILES})

target_link_libraries(${PROJECT_NAME})