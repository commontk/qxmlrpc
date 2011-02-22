
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2)

PROJECT(qxmlrpc)

OPTION(BUILD_TESTING "Enable testing" OFF)
OPTION(qxmlrpc_BUILD_EXAMPLES "Build examples" OFF)


#-----------------------------------------------------------------------------
# Output directories.
#
SET(output_dir ${qxmlrpc_BINARY_DIR}/bin)
FOREACH(type LIBRARY RUNTIME ARCHIVE)
  IF(NOT DEFINED CMAKE_${type}_OUTPUT_DIRECTORY)
    SET(CMAKE_${type}_OUTPUT_DIRECTORY ${output_dir} CACHE INTERNAL "Single output directory for building all libraries.")
  ENDIF()
ENDFOREACH()

#-----------------------------------------------------------------------------
# Qt
#
FIND_PACKAGE(Qt4 4.6 REQUIRED)
SET(QT_USE_QTNETWORK ON)
SET(QT_USE_QTXML ON)
INCLUDE(${QT_USE_FILE})


#-----------------------------------------------------------------------------
# Subdirectories
#

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )
ADD_SUBDIRECTORY(xmlrpc)



IF(BUILD_TESTING)
#  ADD_SUBDIRECTORY(test)
ENDIF()

IF(qxmlrpc_BUILD_EXAMPLES)
#  ADD_SUBDIRECTORY(examples)
ENDIF()

#-----------------------------------------------------------------------------
# Generate qxmlrpc file
#
CONFIGURE_FILE(Useqxmlrpc.cmake.in
               ${qxmlrpc_BINARY_DIR}/Useqxmlrpc.cmake COPYONLY)

INCLUDE(GenerateqxmlrpcConfig.cmake)

