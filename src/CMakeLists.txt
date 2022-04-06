cmake_minimum_required(VERSION 3.2.0)
project(planet_generation)

add_subdirectory(ext/glfw)
add_subdirectory(ext/glbinding)

include_directories(SYSTEM ext/glfw/include)
include_directories(SYSTEM ext/glbinding/include)
include_directories(SYSTEM ext/noiseutils)
include_directories(SYSTEM ext/eigen3)
include_directories(ext/)
include_directories(/usr/local/include)

set (CMAKE_CXX_STANDARD 11)

if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

set(SRC_FILES
    visualisation/main.cpp
    visualisation/viewer.cpp
    visualisation/viewer.h
    visualisation/shader.cpp
    visualisation/shader.h
    visualisation/opengl.h
    visualisation/camera.h
    visualisation/camera.cpp
    visualisation/mesh.h
    visualisation/mesh.cpp
    visualisation/trackball.cpp
    visualisation/trackball.h
    visualisation/quadtree.cpp
    visualisation/quadtree.h
    visualisation/maploader.cpp
    visualisation/maploader.h
    visualisation/heightMapGen.cpp
    visualisation/heightMapGen.h
    ext/noiseutils/noiseutils.cpp
    )

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}/../data")
add_executable(planet_generation ${SRC_FILES})

#ImageMagick
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
find_package(ImageMagick COMPONENTS Magick++)

target_compile_options(glfw PRIVATE "-w")
target_compile_options(glbinding PRIVATE "-w")

set_source_files_properties(ext/eigen3
                            PROPERTIES)
                            #COMPILE_FLAGS -Wno-unused-parameter)

target_include_directories(planet_generation PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            ${ImageMagick_INCLUDE_DIRS})

target_include_directories(planet_generation PUBLIC 
                            /usr/local/include/noise)
target_link_libraries(planet_generation PUBLIC noise glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${ImageMagick_LIBRARIES} glbinding)

function(IndicateExternalFile _target)
    foreach(_file IN ITEMS ${ARGN})
        if ((IS_ABSOLUTE "${_file}" AND EXISTS "${_file}") OR
            (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_file}"))
            target_sources(${_target} PRIVATE "${_file}")
        endif()
    endforeach()
endfunction()

file(GLOB SHADERS "../data/shaders/*.vert" "../data/shaders/*.frag")
IndicateExternalFile("planet_generation" ${SHADERS})