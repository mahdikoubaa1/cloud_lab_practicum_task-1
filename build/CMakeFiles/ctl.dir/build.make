# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mahdi/Documents/Task1-mahdikoubaa1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mahdi/Documents/Task1-mahdikoubaa1/build

# Include any dependencies generated for this target.
include CMakeFiles/ctl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ctl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ctl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ctl.dir/flags.make

CMakeFiles/ctl.dir/src/ctl.cc.o: CMakeFiles/ctl.dir/flags.make
CMakeFiles/ctl.dir/src/ctl.cc.o: ../src/ctl.cc
CMakeFiles/ctl.dir/src/ctl.cc.o: CMakeFiles/ctl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mahdi/Documents/Task1-mahdikoubaa1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ctl.dir/src/ctl.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ctl.dir/src/ctl.cc.o -MF CMakeFiles/ctl.dir/src/ctl.cc.o.d -o CMakeFiles/ctl.dir/src/ctl.cc.o -c /home/mahdi/Documents/Task1-mahdikoubaa1/src/ctl.cc

CMakeFiles/ctl.dir/src/ctl.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ctl.dir/src/ctl.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mahdi/Documents/Task1-mahdikoubaa1/src/ctl.cc > CMakeFiles/ctl.dir/src/ctl.cc.i

CMakeFiles/ctl.dir/src/ctl.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ctl.dir/src/ctl.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mahdi/Documents/Task1-mahdikoubaa1/src/ctl.cc -o CMakeFiles/ctl.dir/src/ctl.cc.s

# Object files for target ctl
ctl_OBJECTS = \
"CMakeFiles/ctl.dir/src/ctl.cc.o"

# External object files for target ctl
ctl_EXTERNAL_OBJECTS =

ctl: CMakeFiles/ctl.dir/src/ctl.cc.o
ctl: CMakeFiles/ctl.dir/build.make
ctl: libcloudlab.a
ctl: /usr/lib/x86_64-linux-gnu/libfmt.so.8.1.1
ctl: /usr/lib/x86_64-linux-gnu/libprotobuf.so
ctl: /usr/lib/librocksdb.so
ctl: /usr/lib/x86_64-linux-gnu/libevent.so
ctl: /usr/lib/x86_64-linux-gnu/libevent_pthreads.so
ctl: CMakeFiles/ctl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mahdi/Documents/Task1-mahdikoubaa1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ctl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ctl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ctl.dir/build: ctl
.PHONY : CMakeFiles/ctl.dir/build

CMakeFiles/ctl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ctl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ctl.dir/clean

CMakeFiles/ctl.dir/depend:
	cd /home/mahdi/Documents/Task1-mahdikoubaa1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mahdi/Documents/Task1-mahdikoubaa1 /home/mahdi/Documents/Task1-mahdikoubaa1 /home/mahdi/Documents/Task1-mahdikoubaa1/build /home/mahdi/Documents/Task1-mahdikoubaa1/build /home/mahdi/Documents/Task1-mahdikoubaa1/build/CMakeFiles/ctl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ctl.dir/depend
