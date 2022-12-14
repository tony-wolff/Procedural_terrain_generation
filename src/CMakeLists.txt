cmake_minimum_required(VERSION 3.2.0)
project(planet_generation)
enable_testing()

add_subdirectory(ext/glfw)
add_subdirectory(ext/glbinding)

include_directories(ext/glfw/include)
include_directories(ext/glbinding/include)
include_directories(ext/noiseutils)
include_directories(ext/eigen3)
include_directories(ext/)
include_directories(/usr/local/include)

set (CMAKE_CXX_STANDARD 11)

if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

set(SRC_FILES
    visualisation/main.cpp
    visualisation/ViewerOpenGL.cpp
    visualisation/ViewerOpenGL.h
    visualisation/Shader.cpp
    visualisation/Shader.h
    visualisation/opengl.h
    visualisation/Camera.h
    visualisation/Camera.cpp
    visualisation/Terrain.h
    visualisation/Terrain.cpp
    visualisation/TrackBall.cpp
    visualisation/TrackBall.h
    visualisation/QuadTree.cpp
    visualisation/QuadTree.h
    visualisation/MapLoader.cpp
    visualisation/MapLoader.h
    visualisation/HeightMapGen.cpp
    visualisation/HeightMapGen.h
    visualisation/TerrainGeneration.h
    visualisation/ViewerAbstract.h
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

if (APPLE)
    target_link_libraries(planet_generation PUBLIC /usr/local/lib/libnoise.a /usr/local/lib/libcppunit.a glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${ImageMagick_LIBRARIES} glbinding)
else()
    target_link_libraries(planet_generation PUBLIC noise glfw ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${ImageMagick_LIBRARIES} glbinding)
endif()

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

# Tests
set(SRC_FILES_test1
    visualisation/QuadTree.h
    visualisation/QuadTree.cpp
    test/TestQuadTree.cpp
)

set(SRC_FILES_test2
    visualisation/HeightMapGen.h
    visualisation/HeightMapGen.cpp
    test/TestHeightMapGen.cpp
    ext/noiseutils/noiseutils.cpp
)

add_executable(test_quadtree ${SRC_FILES_test1})
if (APPLE)
    target_link_libraries(test_quadtree PUBLIC /usr/local/lib/libcppunit.a glbinding)
else()
    target_link_libraries(test_quadtree -lcppunit glbinding)
endif()

add_executable(test_heightmap ${SRC_FILES_test2})

target_include_directories(test_heightmap PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            ${ImageMagick_INCLUDE_DIRS})

if (APPLE)
    target_link_libraries(test_heightmap PUBLIC /usr/local/lib/libnoise.a /usr/local/lib/libcppunit.a glbinding ${ImageMagick_LIBRARIES})
else()
    target_link_libraries(test_heightmap noise -lcppunit glbinding ${ImageMagick_LIBRARIES})
endif()
    
add_test(NAME test_quadtree COMMAND test_quadtree)
add_test(NAME test_heightmap COMMAND test_heightmap)
