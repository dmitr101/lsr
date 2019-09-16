cmake_minimum_required(VERSION 3.8)

include(ExternalProject)

ExternalProject_Add(stb_EXTERNAL
	GIT_REPOSITORY "https://github.com/nothings/stb.git"
	GIT_TAG "master"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND "")
ExternalProject_Get_Property(stb_EXTERNAL SOURCE_DIR)
set(stb_INCLUDE_DIR "${SOURCE_DIR}")
file(MAKE_DIRECTORY "${SOURCE_DIR}")

add_library(stb INTERFACE IMPORTED)
add_dependencies(stb stb_EXTERNAL)
set_property(TARGET stb 
	PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${stb_INCLUDE_DIR}")

