cmake_minimum_required(VERSION 3.2.0)
project(planet_generation)

set (CMAKE_CXX_STANDARD 11)

set(SRC_FILES
    main.cpp 
    maploader.cpp
    )

add_definitions(-DDATA_DIR="${PROJECT_SOURCE_DIR}../../data")
add_executable(planet_generation ${SRC_FILES})

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)

target_include_directories(planet_generation PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>
                           ${PNG_INCLUDE_DIR})
target_link_libraries(planet_generation PUBLIC glfw OpenGL::GL ${PNG_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_DL_LIBS})