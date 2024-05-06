
################################################################################
## cmake setup
cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
##
################################################################################

################################################################################
## Project setup
set(PROJECT_NAME temporary_file_handler)

# Versioning
project(
    ${PROJECT_NAME}
    VERSION 0.1.0.0
    LANGUAGES CXX)

################################################################################
## Build Options (Turn ON or OFF as needed)
option(BUILD_TESTS "Option to build the test apps" OFF) # Builds small test app
option(BUILD_UNIVERSAL_BINARY "Option to build universal binary (x86_64 and arm64)" OFF)
################################################################################

################################################################################
## Generic build setup
include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sets up link-time optimization if supported by compiler
include(CheckIPOSupported)
check_ipo_supported(RESULT lto)
##
################################################################################


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) # Turns on testing if library is built alone
    set(BUILD_TESTS ON)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)

# ##############################################################################
# Build steps
set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

set(SOURCE_FILES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_FILES_DIRECTORY_BUILD ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_TARGET_NAME})
set(INCLUDE_FILES_DIRECTORY_INSTALL include/${LIBRARY_TARGET_NAME})

file(GLOB_RECURSE ${LIBRARY_TARGET_NAME}_SRC LIST_DIRECTORIES false CONFIGURE_DEPENDS ${SOURCE_FILES_DIRECTORY}/*.cpp ${SOURCE_FILES_DIRECTORY}/*.hpp)
file(GLOB_RECURSE ${LIBRARY_TARGET_NAME}_BUILD_INTERFACE_HPP LIST_DIRECTORIES false CONFIGURE_DEPENDS ${INCLUDE_FILES_DIRECTORY_BUILD}/*.hpp)
file(GLOB_RECURSE ${LIBRARY_TARGET_NAME}_INSTALL_INTERFACE_HPP LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS ${INCLUDE_FILES_DIRECTORY_INSTALL}/*.hpp)

#update_deps_file(${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_BUILD_INTERFACE_HPP})

# Must use an empty string "" in add_library in order to use target_sources()
add_library(${LIBRARY_TARGET_NAME} "")

target_sources(
	${LIBRARY_TARGET_NAME}
		PRIVATE
			${${LIBRARY_TARGET_NAME}_SRC}
		PUBLIC
			$<BUILD_INTERFACE:${${LIBRARY_TARGET_NAME}_BUILD_INTERFACE_HPP}>
        	$<INSTALL_INTERFACE:${${LIBRARY_TARGET_NAME}_INSTALL_INTERFACE_HPP}>)

add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

# std::filesystem is inconsistently added in libstdc++ between clang and gcc versions.
# This is a fix to check for compiler compatibility and whether an extra link is needed.

# Setting empty filesystem_lib link
set(filesystem_lib "")

# Check for GNU gcc versions below 9 which need the extra filesystem link.
if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)

    # Check for version below 7 which only include "experimental/filesystem"
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
        message(FATAL_ERROR "g++ compiler versions < 7 do not support std::filesystem")
    endif()

    set(filesystem_lib "stdc++fs")
endif()

# Same as above but for clang.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    # Check for version below 7 which only include "experimental/filesystem"
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
        message(FATAL_ERROR "clang compiler versions < 7 do not support std::filesystem")
    endif()

    set(filesystem_lib "stdc++fs")
endif()

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC ${filesystem_lib})

target_include_directories(${LIBRARY_TARGET_NAME}
    PUBLIC
        #${CMAKE_CURRENT_LIST_DIR}/include
		${INHERITED_PUBLIC_INCLUDE_DIR}
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Strip the binaries in "release" build
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

##############################################
# Installation config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${LIBRARY_TARGET_NAME})

set_target_properties(
	${LIBRARY_TARGET_NAME}
		PROPERTIES
  			PUBLIC_HEADER
				${${LIBRARY_TARGET_NAME}_BUILD_INTERFACE_HPP}
)


install(TARGETS ${LIBRARY_TARGET_NAME}
    EXPORT ${LIBRARY_TARGET_NAME}Targets
    LIBRARY 	DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT lib
    ARCHIVE 	DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT static_lib
	RUNTIME 	DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bin
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME} COMPONENT dev
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Install the Export target
install(
  EXPORT ${LIBRARY_TARGET_NAME}Targets
  FILE
    ${LIBRARY_TARGET_NAME}Targets.cmake
  NAMESPACE
    ${LIBRARY_TARGET_NAME}::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

##############################################
#[[
Exporting from the build tree. Allows other projects in the build tree to use
the library target without calling FindPackage().
]]
export(
	TARGETS ${LIBRARY_TARGET_NAME}
	FILE ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_NAME}Targets.cmake
	NAMESPACE ${LIBRARY_TARGET_NAME}::
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${LIBRARY_TARGET_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_TARGET_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

#Register package in user's package registry
export(PACKAGE ${LIBRARY_TARGET_NAME})

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif(NOT TARGET uninstall)

# Colorized Output Setup
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(Reset         "${Esc}[m")
  set(Bold          "${Esc}[1m")
  set(Red           "${Esc}[31m")
  set(Green         "${Esc}[32m")
  set(Blue          "${Esc}[34m")
  set(Cyan          "${Esc}[36m")
  set(Magenta       "${Esc}[35m")
  set(Yellow        "${Esc}[33m")
  set(White         "${Esc}[37m")
  set(BoldRed       "${Esc}[1;31m")
  set(BoldGreen     "${Esc}[1;32m")
  set(BoldBlue      "${Esc}[1;34m")
  set(BoldCyan      "${Esc}[1;36m")
  set(BoldMagenta   "${Esc}[1;35m")
  set(BoldYellow    "${Esc}[1;33m")
  set(BoldWhite     "${Esc}[1;37m")
endif()

if(DEPENDENCY_LIST)
	set(DEPENDENCY_MESSAGE ${DEPENDENCY_LIST})
else()
	set(DEPENDENCY_MESSAGE "None")
endif()

# Final summary
message("
*******************************************************************************
CMake configuration completed with the following values:
Library Name:
	${LIBRARY_TARGET_NAME}
Alias for use with target_link_libraries():
	${LIBRARY_TARGET_NAME}::${LIBRARY_TARGET_NAME}
Dependencies for ${LIBRARY_TARGET_NAME}:
	${DEPENDENCY_MESSAGE}
*******************************************************************************
"
)

###############################################################################
###############################################################################
#[[

END OF FILE

CMakeLists.txt
Generated from libCMakeConfig.cmake
]]
###############################################################################
###############################################################################
