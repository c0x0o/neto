project(neto LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.0.0)

# Option               |  Description
# -DCMAKE_BUILD_TYPE   |  could be 'release', 'debug'
# -DNETO_EXCLUDE_TEST  |  set to prevent compiling test code

# TODO check required libs

# enable c++11 standard
if(CMAKE_VERSION VERSION_LESS "3.1")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
  endif()
else()
  set(CMAKE_CXX_STANDARD 11)
endif()

# default build type: release
if( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE release)
endif()

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-format-security -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG             "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE           "-O3 -fno-strict-aliasing")
set(CMAKE_CXX_FLAGS_MINSIZEREL        "-Os")
set(CMAKE_CXX_RELWITHDEBINFO_RELEASE  "-O2")

add_subdirectory(src)

if( NOT NETO_EXCLUDE_TEST )
  enable_testing()
  add_subdirectory(test)
endif()
