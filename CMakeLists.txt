cmake_minimum_required(VERSION 3.10)

##############################################################################################################################
# Vars and Versions:
set(SVERSION 2.7.1)

set(PROJECT_HOMEPAGE_URL "https://github.com/unmanarc/mantidsExamples")
set(PROJECT_CONTACT "Aaron G. Mizrachi P. <aaron@unmanarc.com>")
##############################################################################################################################

##############################################################################################################################
# C++ Global Options Settings:
set(CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
##############################################################################################################################

##############################################################################################################################
# Project Settings:
project(mantidsExamples)
project(${PROJECT_NAME} VERSION ${SVERSION} DESCRIPTION "libMantids Examples")
##############################################################################################################################

##############################################################################################################################
# Other CMake Global settings:
include(GNUInstallDirs)
##############################################################################################################################

#############################################################################################################################
# Subprojects:
ADD_SUBDIRECTORY(mdzAuth)
ADD_SUBDIRECTORY(mdzNetwork)
ADD_SUBDIRECTORY(mdzProtocols)
ADD_SUBDIRECTORY(mdzServices)
#ADD_SUBDIRECTORY(mdzXRPC)

#############################################################################################################################

