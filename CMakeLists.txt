cmake_minimum_required(VERSION 3.16)
project(SolarSystem)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# GLAD (manual source)
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)

# Your executable
add_executable(SolarSystem src/main.cpp)

target_link_libraries(SolarSystem glfw glad glm)