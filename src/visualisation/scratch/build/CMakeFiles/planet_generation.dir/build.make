# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build

# Include any dependencies generated for this target.
include CMakeFiles/planet_generation.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/planet_generation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/planet_generation.dir/flags.make

CMakeFiles/planet_generation.dir/main.cpp.o: CMakeFiles/planet_generation.dir/flags.make
CMakeFiles/planet_generation.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/planet_generation.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/planet_generation.dir/main.cpp.o -c /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/main.cpp

CMakeFiles/planet_generation.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/planet_generation.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/main.cpp > CMakeFiles/planet_generation.dir/main.cpp.i

CMakeFiles/planet_generation.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/planet_generation.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/main.cpp -o CMakeFiles/planet_generation.dir/main.cpp.s

CMakeFiles/planet_generation.dir/maploader.cpp.o: CMakeFiles/planet_generation.dir/flags.make
CMakeFiles/planet_generation.dir/maploader.cpp.o: ../maploader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/planet_generation.dir/maploader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/planet_generation.dir/maploader.cpp.o -c /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/maploader.cpp

CMakeFiles/planet_generation.dir/maploader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/planet_generation.dir/maploader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/maploader.cpp > CMakeFiles/planet_generation.dir/maploader.cpp.i

CMakeFiles/planet_generation.dir/maploader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/planet_generation.dir/maploader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/maploader.cpp -o CMakeFiles/planet_generation.dir/maploader.cpp.s

CMakeFiles/planet_generation.dir/maprenderer.cpp.o: CMakeFiles/planet_generation.dir/flags.make
CMakeFiles/planet_generation.dir/maprenderer.cpp.o: ../maprenderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/planet_generation.dir/maprenderer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/planet_generation.dir/maprenderer.cpp.o -c /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/maprenderer.cpp

CMakeFiles/planet_generation.dir/maprenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/planet_generation.dir/maprenderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/maprenderer.cpp > CMakeFiles/planet_generation.dir/maprenderer.cpp.i

CMakeFiles/planet_generation.dir/maprenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/planet_generation.dir/maprenderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/maprenderer.cpp -o CMakeFiles/planet_generation.dir/maprenderer.cpp.s

# Object files for target planet_generation
planet_generation_OBJECTS = \
"CMakeFiles/planet_generation.dir/main.cpp.o" \
"CMakeFiles/planet_generation.dir/maploader.cpp.o" \
"CMakeFiles/planet_generation.dir/maprenderer.cpp.o"

# External object files for target planet_generation
planet_generation_EXTERNAL_OBJECTS =

planet_generation: CMakeFiles/planet_generation.dir/main.cpp.o
planet_generation: CMakeFiles/planet_generation.dir/maploader.cpp.o
planet_generation: CMakeFiles/planet_generation.dir/maprenderer.cpp.o
planet_generation: CMakeFiles/planet_generation.dir/build.make
planet_generation: /usr/local/lib/libglfw3.a
planet_generation: /usr/lib/x86_64-linux-gnu/libGL.so
planet_generation: /usr/local/lib/libMagick++-7.Q16HDRI.so
planet_generation: /usr/lib/x86_64-linux-gnu/librt.so
planet_generation: /usr/lib/x86_64-linux-gnu/libm.so
planet_generation: /usr/lib/x86_64-linux-gnu/libX11.so
planet_generation: CMakeFiles/planet_generation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable planet_generation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/planet_generation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/planet_generation.dir/build: planet_generation

.PHONY : CMakeFiles/planet_generation.dir/build

CMakeFiles/planet_generation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/planet_generation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/planet_generation.dir/clean

CMakeFiles/planet_generation.dir/depend:
	cd /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build /home/athom/workspace/pdp-procedural_planet_generation/src/visualisation/scratch/build/CMakeFiles/planet_generation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/planet_generation.dir/depend
