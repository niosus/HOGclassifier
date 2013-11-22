# This file is part of HOGclassifier.

# HOGclassifier is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# HOGclassifier is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with HOGclassifier.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(hog_classifier)

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x -O ${CMAKE_CXX_FLAGS}")

# The library prefix
SET(LIB_PREFIX _hog_classifier)

SET(hog_classifier_C_FLAGS)
SET(hog_classifier_CXX_FLAGS)

# Set the output directory for the build executables and libraries
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${hog_classifier_SOURCE_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hog_classifier_SOURCE_DIR}/bin)

# Find OpenCV
FIND_PACKAGE(OpenCV 2 REQUIRED)

# Set up the top-level include directories
INCLUDE_DIRECTORIES(${hog_classifier_SOURCE_DIR} ${hog_classifier_INCLUDE} ${OPENCV_INCLUDE_DIR})

# Include the subdirectories
ADD_SUBDIRECTORY(source)

set(CMAKE_BUILD_TYPE Release)