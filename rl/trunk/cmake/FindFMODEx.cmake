# Find FMODEx
#
# This module defines
# FMODEX_FOUND
# FMODEX_INCLUDE_DIR
# FMODEX_LIBRARY
#
# Based on the CMakeLists.txt of zdoom and FindFMOD.cmake of openfrag
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# On a new cmake run, we do not need to be verbose
IF(FMODEX_INCLUDE_DIR AND FMODEX_LIBRARY)
	SET(FMODEX_FIND_QUIETLY TRUE)
ENDIF(FMODEX_INCLUDE_DIR AND FMODEX_LIBRARY)

# Check for for 32/64 bit architecture
IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
	SET(X64 64)
ENDIF(CMAKE_SIZEOF_VOID_P MATCHES "8")

IF(WIN32)
	SET(FMODEX_INC_DIR_SUFFIXES PATH_SUFFIXES inc)
	SET(FMODEX_LIB_DIR_SUFFIXES PATH_SUFFIXES lib)
ELSE(WIN32)
	SET(FMODEX_INCLUDE_SEARCH_DIRS
	    /usr/include
	    /usr/local/include
	    /opt/fmodex/fmodex/include
            /opt/include)
	SET(FMODEX_LIBRARY_SEARCH_DIRS
	    /usr/lib
	    /usr/lib64
	    /usr/local/lib
	    /usr/local/lib64
	    /opt/fmodex/lib
	    /opt/fmodex/lib64)
	SET(FMODEX_INC_DIR_SUFFIXES PATH_SUFFIXES fmodex)
ENDIF(WIN32)

# Decide on the name of the FMODEx library we want to use
IF(NOT FMODEX_LIBRARY_NAME AND MSVC)
	SET(FMODEX_LIBRARY_NAME fmodex${X64}_vc)
ENDIF(NOT FMODEX_LIBRARY_NAME AND MSVC)

IF(NOT FMODEX_LIBRARY_NAME AND BORLAND)
	SET(FMODEX_LIBRARY_NAME fmodex${X64}_bc)
ENDIF(NOT FMODEX_LIBRARY_NAME AND BORLAND)

IF(NOT FMODEX_LIBRARY_NAME)
	SET(FMODEX_LIBRARY_NAME fmodex${X64})
ENDIF(NOT FMODEX_LIBRARY_NAME)

IF (NOT FMODEX_FIND_QUIETLY)
	MESSAGE(STATUS "Checking for FMODEx")
ENDIF(NOT FMODEX_FIND_QUIETLY)

# Search for header files and libraries
FIND_PATH(FMODEX_INCLUDE_DIR fmod.h
          PATHS ${FMODEX_INCLUDE_SEARCH_PATHS}
          PATH_SUFFIXES ${FMODEX_INC_DIR_SUFFIXES})
IF(FMODEX_INCLUDE_DIR)
	IF (NOT FMODEX_FIND_QUIETLY)
		MESSAGE(STATUS "  includes: ${FMODEX_INCLUDE_DIR}")
	ENDIF(NOT FMODEX_FIND_QUIETLY)
ELSE(FMODEX_INCLUDE_DIR)
	MESSAGE(SEND_ERROR "FMODEx include files could not be found.")
ENDIF(FMODEX_INCLUDE_DIR)

FIND_LIBRARY(FMODEX_LIBRARY ${FMODEX_LIBRARY_NAME}
             PATHS ${FMODEX_LIBRARY_SEARCH_DIRS}
             PATH_SUFFIXES ${FMOD_LIB_DIR_SUFFIXES})
IF(FMODEX_LIBRARY)
	IF (NOT FMODEX_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries: ${FMODEX_LIBRARY}")
	ENDIF(NOT FMODEX_FIND_QUIETLY)
ELSE(FMODEX_LIBRARY)
	MESSAGE(SEND_ERROR, "FMODex library could not be found.")
ENDIF(FMODEX_LIBRARY)

IF(FMODEX_INCLUDE_DIR AND FMODEX_LIBRARY)
	SET(FMODEX_FOUND TRUE)
ENDIF(FMODEX_INCLUDE_DIR AND FMODEX_LIBRARY)

# Allow customisation of paths
SET(FMODEX_INCLUDE_DIR ${FMODEX_INCLUDE_DIR})
SET(FMODEX_LIBRARY ${FMODEX_LIBRARY})

IF(FMODEX_INCLUDE_DIR)
	# Extract the library version from the header file
	SET(FMODEX_VERSION 0)
	FILE(READ "${FMODEX_INCLUDE_DIR}/fmod.h"
	     _FMODEX_FMOD_H_CONTENTS)
	STRING(REGEX REPLACE ".*#define FMOD_VERSION    0x([0-9]+).*" "\\1"
	       FMODEX_VERSION "${_FMODEX_FMOD_H_CONTENTS}")
	IF(NOT "${FMODEX_VERSION}" STREQUAL 0)
		MATH(EXPR FMODEX_MAJOR_VERSION "${FMODEX_VERSION} / 10000")
		MATH(EXPR FMODEX_MINOR_VERSION "${FMODEX_VERSION} / 100 % 100")
		MATH(EXPR FMODEX_PATCH_VERSION "${FMODEX_VERSION} % 100")
		SET(FMODEX_VERSION "${FMODEX_MAJOR_VERSION}.${FMODEX_MINOR_VERSION}.${FMODEX_PATCH_VERSION}")
	ENDIF(NOT "${FMODEX_VERSION}" STREQUAL 0)

	# Compare installed and required version
	IF(FMODEx_FIND_VERSION)
		# Major and minor version are required
		IF(NOT FMODEx_FIND_VERSION_MINOR)
			MESSAGE(SEND_ERROR "When requesting a specific version of FMODEx , you must provide at least the major and minor version numbers, e.g., 4.20")
		ENDIF(NOT FMODEx_FIND_VERSION_MINOR)

		IF ("${FMOD_VERSION}" VERSION_LESS "${FMODEx_FIND_VERSION}")
			SET(FMODEX_FOUND FALSE)
			MESSAGE(STATUS "Detected version of FMODEx ${FMODEX_VERSION} is too old. At least version ${FMODEx_FIND_VERSION} is required.")
		ENDIF("${FMOD_VERSION}" VERSION_LESS "${FMODEx_FIND_VERSION}")
	ENDIF(FMODEx_FIND_VERSION)
ENDIF(FMODEX_INCLUDE_DIR)

IF(FMODEX_FOUND)
	IF(NOT FMODEX_FIND_QUIETLY)
		MESSAGE(STATUS "  version: ${FMODEX_VERSION}")
	ENDIF(NOT FMODEX_FIND_QUIETLY)
ENDIF(FMODEX_FOUND)