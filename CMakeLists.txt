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

# GLAD
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)

# Executable
add_executable(SolarSystem
    src/main.cpp
    src/shaderClass.cpp
    src/VBO.cpp
    src/VAO.cpp
    src/EBO.cpp
    src/camera.cpp
)

target_include_directories(SolarSystem PUBLIC include)

target_link_libraries(SolarSystem glfw glad glm)

set_target_properties(SolarSystem PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    VS_DEBUGGER_COMMAND_ARGUMENTS ""
)

target_compile_definitions(SolarSystem PRIVATE
    SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/"
)