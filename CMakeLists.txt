cmake_minimum_required(VERSION 4.2.3)
project(ModelViewer)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/build/vendor CACHE PATH "" FORCE)

FetchContent_Declare(
    glad
    GIT_SHALLOW TRUE
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    SOURCE_SUBDIR cmake
)
FetchContent_Declare(
    glfw3
    GIT_SHALLOW TRUE
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_Declare(
    glm
    GIT_SHALLOW TRUE
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)
FetchContent_Declare(
    imgui
    GIT_SHALLOW TRUE
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    SOURCE_SUBDIR NULL
)
FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

find_package(OpenGL REQUIRED)
FetchContent_MakeAvailable(glad glfw3 glm imgui stb_image)
glad_add_library(glad
    STATIC
    API gl:core=3.3
    LOCATION ${PROJECT_SOURCE_DIR}/build/vendor/glad-build/${TARGET}
)

add_library(imgui
    STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(stb_image
    INTERFACE
)
file(COPY ${stb_image_SOURCE_DIR}/stb_image.h 
    DESTINATION 
        "${PROJECT_SOURCE_DIR}/build/vendor/stb_image-build/include/stb_image")

file(COPY ${stb_image_SOURCE_DIR}/stb_image_write.h 
    DESTINATION 
        "${PROJECT_SOURCE_DIR}/build/vendor/stb_image-build/include/stb_image")

set(STBIMAGE_INSTALL_DIR "${PROJECT_SOURCE_DIR}/build/vendor/stb_image-build")

target_include_directories(imgui
    PUBLIC
        ${imgui_SOURCE_DIR} 
        ${imgui_SOURCE_DIR}/backends
)

# -- PROJECT -- #
add_executable(${PROJECT_NAME} src/main.cpp src/shader.cpp src/shader.h)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        glad
        glm
        imgui
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        src/
        build/vendor/stb_image-build/include
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DCIMGUI_USE_OPENGL3 
    -DCIMGUI_USE_GLFW
)
