# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/qrpucp/px4_control/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qrpucp/px4_control/build

# Include any dependencies generated for this target.
include off_board/CMakeFiles/offb_node.dir/depend.make

# Include the progress variables for this target.
include off_board/CMakeFiles/offb_node.dir/progress.make

# Include the compile flags for this target's objects.
include off_board/CMakeFiles/offb_node.dir/flags.make

off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o: off_board/CMakeFiles/offb_node.dir/flags.make
off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o: /home/qrpucp/px4_control/src/off_board/src/one_drone_mission_planning.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/qrpucp/px4_control/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o"
	cd /home/qrpucp/px4_control/build/off_board && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o -c /home/qrpucp/px4_control/src/off_board/src/one_drone_mission_planning.cpp

off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.i"
	cd /home/qrpucp/px4_control/build/off_board && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/qrpucp/px4_control/src/off_board/src/one_drone_mission_planning.cpp > CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.i

off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.s"
	cd /home/qrpucp/px4_control/build/off_board && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/qrpucp/px4_control/src/off_board/src/one_drone_mission_planning.cpp -o CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.s

off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.requires:

.PHONY : off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.requires

off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.provides: off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.requires
	$(MAKE) -f off_board/CMakeFiles/offb_node.dir/build.make off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.provides.build
.PHONY : off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.provides

off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.provides.build: off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o


# Object files for target offb_node
offb_node_OBJECTS = \
"CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o"

# External object files for target offb_node
offb_node_EXTERNAL_OBJECTS =

/home/qrpucp/px4_control/devel/lib/off_board/offb_node: off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: off_board/CMakeFiles/offb_node.dir/build.make
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /home/qrpucp/px4_control/devel/lib/libCubicSplinePlanner.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/libroscpp.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_signals.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/librosconsole.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/libxmlrpcpp.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/libroscpp_serialization.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/librostime.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /opt/ros/kinetic/lib/libcpp_common.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/qrpucp/px4_control/devel/lib/off_board/offb_node: off_board/CMakeFiles/offb_node.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/qrpucp/px4_control/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/qrpucp/px4_control/devel/lib/off_board/offb_node"
	cd /home/qrpucp/px4_control/build/off_board && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/offb_node.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
off_board/CMakeFiles/offb_node.dir/build: /home/qrpucp/px4_control/devel/lib/off_board/offb_node

.PHONY : off_board/CMakeFiles/offb_node.dir/build

off_board/CMakeFiles/offb_node.dir/requires: off_board/CMakeFiles/offb_node.dir/src/one_drone_mission_planning.cpp.o.requires

.PHONY : off_board/CMakeFiles/offb_node.dir/requires

off_board/CMakeFiles/offb_node.dir/clean:
	cd /home/qrpucp/px4_control/build/off_board && $(CMAKE_COMMAND) -P CMakeFiles/offb_node.dir/cmake_clean.cmake
.PHONY : off_board/CMakeFiles/offb_node.dir/clean

off_board/CMakeFiles/offb_node.dir/depend:
	cd /home/qrpucp/px4_control/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qrpucp/px4_control/src /home/qrpucp/px4_control/src/off_board /home/qrpucp/px4_control/build /home/qrpucp/px4_control/build/off_board /home/qrpucp/px4_control/build/off_board/CMakeFiles/offb_node.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : off_board/CMakeFiles/offb_node.dir/depend

