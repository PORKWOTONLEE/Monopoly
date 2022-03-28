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
CMAKE_SOURCE_DIR = /home/ben/Monopoly

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ben/Monopoly/build

# Utility rule file for Vita2d_Test.vpk.

# Include the progress variables for this target.
include CMakeFiles/Vita2d_Test.vpk.dir/progress.make

CMakeFiles/Vita2d_Test.vpk: Vita2d_Test.vpk.out
	/usr/bin/cmake -E copy /home/ben/Monopoly/build/Vita2d_Test.vpk.out Vita2d_Test.vpk

Vita2d_Test.vpk.out: Vita2d_Test.vpk_param.sfo
Vita2d_Test.vpk.out: Vita2d_Test.self
Vita2d_Test.vpk.out: ../sce_sys/icon0.png
Vita2d_Test.vpk.out: ../sce_sys/livearea/contents/bg.png
Vita2d_Test.vpk.out: ../sce_sys/livearea/contents/startup.png
Vita2d_Test.vpk.out: ../sce_sys/livearea/contents/template.xml
Vita2d_Test.vpk.out: ../assets/font.ttf
Vita2d_Test.vpk.out: ../assets/map.csv
Vita2d_Test.vpk.out: ../assets/line.png
Vita2d_Test.vpk.out: ../assets/delim.png
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ben/Monopoly/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building vpk Vita2d_Test.vpk"
	/usr/local/vitasdk/bin/vita-pack-vpk -a /home/ben/Monopoly/sce_sys/icon0.png=sce_sys/icon0.png -a /home/ben/Monopoly/sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png -a /home/ben/Monopoly/sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png -a /home/ben/Monopoly/sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml -a /home/ben/Monopoly/assets/font.ttf=assets/font.ttf -a /home/ben/Monopoly/assets/map.csv=assets/map.csv -a /home/ben/Monopoly/assets/line.png=assets/line.png -a /home/ben/Monopoly/assets/delim.png=assets/delim.png -s /home/ben/Monopoly/build/Vita2d_Test.vpk_param.sfo -b /home/ben/Monopoly/build/Vita2d_Test.self /home/ben/Monopoly/build/Vita2d_Test.vpk.out

Vita2d_Test.vpk_param.sfo: Vita2d_Test.self
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ben/Monopoly/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating param.sfo for Vita2d_Test.vpk"
	/usr/local/vitasdk/bin/vita-mksfoex -s APP_VER=01.00 -s TITLE_ID=VSDK00125 "Vita2d Test" /home/ben/Monopoly/build/Vita2d_Test.vpk_param.sfo

Vita2d_Test.vpk: CMakeFiles/Vita2d_Test.vpk
Vita2d_Test.vpk: Vita2d_Test.vpk.out
Vita2d_Test.vpk: Vita2d_Test.vpk_param.sfo
Vita2d_Test.vpk: CMakeFiles/Vita2d_Test.vpk.dir/build.make

.PHONY : Vita2d_Test.vpk

# Rule to build all files generated by this target.
CMakeFiles/Vita2d_Test.vpk.dir/build: Vita2d_Test.vpk

.PHONY : CMakeFiles/Vita2d_Test.vpk.dir/build

CMakeFiles/Vita2d_Test.vpk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Vita2d_Test.vpk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Vita2d_Test.vpk.dir/clean

CMakeFiles/Vita2d_Test.vpk.dir/depend:
	cd /home/ben/Monopoly/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ben/Monopoly /home/ben/Monopoly /home/ben/Monopoly/build /home/ben/Monopoly/build /home/ben/Monopoly/build/CMakeFiles/Vita2d_Test.vpk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Vita2d_Test.vpk.dir/depend
