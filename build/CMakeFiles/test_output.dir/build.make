# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build

# Include any dependencies generated for this target.
include CMakeFiles/test_output.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_output.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_output.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_output.dir/flags.make

CMakeFiles/test_output.dir/tests/test_A.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_A.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_A.cpp
CMakeFiles/test_output.dir/tests/test_A.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_output.dir/tests/test_A.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_A.cpp.o -MF CMakeFiles/test_output.dir/tests/test_A.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_A.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_A.cpp

CMakeFiles/test_output.dir/tests/test_A.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_A.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_A.cpp > CMakeFiles/test_output.dir/tests/test_A.cpp.i

CMakeFiles/test_output.dir/tests/test_A.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_A.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_A.cpp -o CMakeFiles/test_output.dir/tests/test_A.cpp.s

CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_arithmetic.cpp
CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o -MF CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_arithmetic.cpp

CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_arithmetic.cpp > CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.i

CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_arithmetic.cpp -o CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.s

CMakeFiles/test_output.dir/tests/test_csv.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_csv.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_csv.cpp
CMakeFiles/test_output.dir/tests/test_csv.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_output.dir/tests/test_csv.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_csv.cpp.o -MF CMakeFiles/test_output.dir/tests/test_csv.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_csv.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_csv.cpp

CMakeFiles/test_output.dir/tests/test_csv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_csv.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_csv.cpp > CMakeFiles/test_output.dir/tests/test_csv.cpp.i

CMakeFiles/test_output.dir/tests/test_csv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_csv.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_csv.cpp -o CMakeFiles/test_output.dir/tests/test_csv.cpp.s

CMakeFiles/test_output.dir/tests/test_iris.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_iris.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_iris.cpp
CMakeFiles/test_output.dir/tests/test_iris.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_output.dir/tests/test_iris.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_iris.cpp.o -MF CMakeFiles/test_output.dir/tests/test_iris.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_iris.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_iris.cpp

CMakeFiles/test_output.dir/tests/test_iris.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_iris.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_iris.cpp > CMakeFiles/test_output.dir/tests/test_iris.cpp.i

CMakeFiles/test_output.dir/tests/test_iris.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_iris.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_iris.cpp -o CMakeFiles/test_output.dir/tests/test_iris.cpp.s

CMakeFiles/test_output.dir/tests/test_main.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_main.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_main.cpp
CMakeFiles/test_output.dir/tests/test_main.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/test_output.dir/tests/test_main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_main.cpp.o -MF CMakeFiles/test_output.dir/tests/test_main.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_main.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_main.cpp

CMakeFiles/test_output.dir/tests/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_main.cpp > CMakeFiles/test_output.dir/tests/test_main.cpp.i

CMakeFiles/test_output.dir/tests/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_main.cpp -o CMakeFiles/test_output.dir/tests/test_main.cpp.s

CMakeFiles/test_output.dir/tests/test_table.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_table.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_table.cpp
CMakeFiles/test_output.dir/tests/test_table.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/test_output.dir/tests/test_table.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_table.cpp.o -MF CMakeFiles/test_output.dir/tests/test_table.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_table.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_table.cpp

CMakeFiles/test_output.dir/tests/test_table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_table.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_table.cpp > CMakeFiles/test_output.dir/tests/test_table.cpp.i

CMakeFiles/test_output.dir/tests/test_table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_table.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_table.cpp -o CMakeFiles/test_output.dir/tests/test_table.cpp.s

CMakeFiles/test_output.dir/tests/test_value.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_value.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_value.cpp
CMakeFiles/test_output.dir/tests/test_value.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/test_output.dir/tests/test_value.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_value.cpp.o -MF CMakeFiles/test_output.dir/tests/test_value.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_value.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_value.cpp

CMakeFiles/test_output.dir/tests/test_value.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_value.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_value.cpp > CMakeFiles/test_output.dir/tests/test_value.cpp.i

CMakeFiles/test_output.dir/tests/test_value.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_value.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_value.cpp -o CMakeFiles/test_output.dir/tests/test_value.cpp.s

CMakeFiles/test_output.dir/tests/test_weights.cpp.o: CMakeFiles/test_output.dir/flags.make
CMakeFiles/test_output.dir/tests/test_weights.cpp.o: /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_weights.cpp
CMakeFiles/test_output.dir/tests/test_weights.cpp.o: CMakeFiles/test_output.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/test_output.dir/tests/test_weights.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_output.dir/tests/test_weights.cpp.o -MF CMakeFiles/test_output.dir/tests/test_weights.cpp.o.d -o CMakeFiles/test_output.dir/tests/test_weights.cpp.o -c /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_weights.cpp

CMakeFiles/test_output.dir/tests/test_weights.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_output.dir/tests/test_weights.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_weights.cpp > CMakeFiles/test_output.dir/tests/test_weights.cpp.i

CMakeFiles/test_output.dir/tests/test_weights.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_output.dir/tests/test_weights.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/tests/test_weights.cpp -o CMakeFiles/test_output.dir/tests/test_weights.cpp.s

# Object files for target test_output
test_output_OBJECTS = \
"CMakeFiles/test_output.dir/tests/test_A.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_csv.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_iris.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_main.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_table.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_value.cpp.o" \
"CMakeFiles/test_output.dir/tests/test_weights.cpp.o"

# External object files for target test_output
test_output_EXTERNAL_OBJECTS =

test_output: CMakeFiles/test_output.dir/tests/test_A.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_arithmetic.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_csv.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_iris.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_main.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_table.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_value.cpp.o
test_output: CMakeFiles/test_output.dir/tests/test_weights.cpp.o
test_output: CMakeFiles/test_output.dir/build.make
test_output: /opt/homebrew/lib/libgtest_main.a
test_output: /opt/homebrew/lib/libgtest.a
test_output: CMakeFiles/test_output.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable test_output"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_output.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_output.dir/build: test_output
.PHONY : CMakeFiles/test_output.dir/build

CMakeFiles/test_output.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_output.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_output.dir/clean

CMakeFiles/test_output.dir/depend:
	cd /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build /Users/sermetpekin/Documents/git_repos_public/PUBLIC_microgradcpp/microgradCpp/build/CMakeFiles/test_output.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test_output.dir/depend

