cmake_minimum_required(VERSION 3.2.0)
project(planet_generation)

set (CMAKE_CXX_STANDARD 11)

include_directories(include/Eigen)

set(SRC_FILES
    main.cpp 
    maploader.cpp
    maprenderer.cpp
    )

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}../../data")
add_executable(planet_generation ${SRC_FILES})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

#ImageMagick
add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)
find_package(ImageMagick COMPONENTS Magick++)

target_include_directories(planet_generation PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>
                           ${ImageMagick_INCLUDE_DIRS})
target_link_libraries(planet_generation PUBLIC glfw OpenGL::GL ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS} ${ImageMagick_LIBRARIES})