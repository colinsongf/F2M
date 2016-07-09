# Copyright (c) 2016 by contributors.
# Authour: Chao Ma (mctt90@gmail.com)
#
# This is the root CMakeLists.txt file of the F2M project,
# which build all subdirectories (packages) in the order of
# inter-package dependence.
#
# Before you build this project, please you need to set up
# the dependent libraries following this doc:
#
#
#
# Now, you can build the F2M using the following commands:
#
# $> mkdir build
# $> cmake ..
# $> make
#
# Thus you check out the F2M project and build it in a
# subdirectory ``build". If you want further to install the 
# built project, you can modify the default installation directory:
#
#   set(CMAKE_INSTALL_PREFIX "/home/public/F2M")
#
# and type the command
#
# $> make install
#
project ("F2M")

#-------------------------------------------------------------------------------
# Take almost all warnings;
# Take warnings as errors;
# Do not generate debug symbols;
# Optimazation level 3;
#-------------------------------------------------------------------------------
add_definitions(" -Wall -Wno-sign-compare -Werror -O3 ")

#-------------------------------------------------------------------------------
# Declare where our project will be installed.
#-------------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "home/alex/F2M_exe")

#-------------------------------------------------------------------------------
# Ensure executables are statically linked with libraries.
#-------------------------------------------------------------------------------
if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
endif()

#-------------------------------------------------------------------------------
# Set include paths and library paths.
#
# F2M depends on the following 
#-------------------------------------------------------------------------------
