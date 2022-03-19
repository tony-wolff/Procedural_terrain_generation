cmake_minimum_required(VERSION 3.2.0)
project(planet_generation)


add_executable(planet_generation testglfw.cpp)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(planet_generation PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(planet_generation PUBLIC glfw OpenGL::GL ${CMAKE_DL_LIBS})