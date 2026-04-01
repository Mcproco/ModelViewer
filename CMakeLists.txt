cmake_minimum_required(VERSION 4.2.3)
project(ModelViewer)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

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
    cglm
    GIT_SHALLOW TRUE
    GIT_REPOSITORY https://github.com/recp/cglm.git
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(
    cimgui
    GIT_SHALLOW TRUE
    GIT_REPOSITORY https://github.com/cimgui/cimgui.git
    SOURCE_SUBDIR NULL
)
find_package(OpenGL REQUIRED)
FetchContent_MakeAvailable(glad glfw3 cglm cimgui)
glad_add_library(glad
    STATIC
    API gl:core=3.3
    LOCATION ${PROJECT_SOURCE_DIR}/build/vendor/glad-build/${TARGET}
)

add_library(cimgui
    STATIC
        ${cimgui_SOURCE_DIR}/cimgui.cpp
        ${cimgui_SOURCE_DIR}/cimgui_impl.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(cimgui
    PUBLIC
        ${cimgui_SOURCE_DIR} 
        ${cimgui_SOURCE_DIR}/imgui 
        ${cimgui_SOURCE_DIR}/imgui/backends
)
target_compile_definitions(cimgui PRIVATE 
	IMGUI_IMPL_API=extern\ \"C\"
	IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_link_libraries(cimgui PRIVATE glfw)



# -- PROJECT -- #
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        glfw
        glad
        cglm
        cimgui
)
target_include_directories(${PROJECT_NAME} 
    PRIVATE
        src/
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DCIMGUI_USE_OPENGL3 
    -DCIMGUI_USE_GLFW
)
