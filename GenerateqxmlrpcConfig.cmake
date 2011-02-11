###########################################################################
#
#  Library:   CTK
#
#  Copyright (c) Kitware Inc.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.commontk.org/LICENSE
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
###########################################################################

#-----------------------------------------------------------------------------
# Settings shared between the build tree and install tree.


#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The "use" file.
SET(qxmlrpc_USE_FILE ${qxmlrpc_BINARY_DIR}/Useqxmlrpc.cmake)

# Determine the include directories needed.
SET(qxmlrpc_INCLUDE_DIRS_CONFIG
  ${qxmlrpc_SOURCE_DIR}
)

# Library directory.
SET(qxmlrpc_LIBRARY_DIRS_CONFIG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Runtime library directory.
SET(qxmlrpc_RUNTIME_LIBRARY_DIRS_CONFIG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Build configuration information.
SET(qxmlrpc_CONFIGURATION_TYPES_CONFIG ${CMAKE_CONFIGURATION_TYPES})
SET(qxmlrpc_BUILD_TYPE_CONFIG ${CMAKE_BUILD_TYPE})

#-----------------------------------------------------------------------------
# Configure qxmlrpcConfig.cmake for the build tree.
CONFIGURE_FILE(${qxmlrpc_SOURCE_DIR}/qxmlrpcConfig.cmake.in
               ${qxmlrpc_BINARY_DIR}/qxmlrpcConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# TODO

#-----------------------------------------------------------------------------
# Configure qxmlrpcConfig.cmake for the install tree.

# TODO
