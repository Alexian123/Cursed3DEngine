cmake_minimum_required(VERSION 3.10)
project(Cursed3DEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/KeyboardHandler.cpp
    src/Map.cpp
    src/Player.cpp
    src/Renderer.cpp
    src/Vector2F.cpp
    src/Utils.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} ncursesw)
