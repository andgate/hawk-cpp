cmake_minimum_required (VERSION 2.8)
project (Hawk)

#version number
set (Hawk_VERSION_MAJOR 1)
set (Hawk_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# boost
#set(Boost_NO_SYSTEM_PATHS true)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED
  COMPONENTS
  system
  date_time
  thread
  regex
  filesystem
  )

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_libraries(${Boost_LIBRARIES})
endif()

# include the subdirectory containing core library and programs
subdirs(hawkc)

# Prepend is used to append the directory to a list of source/header files.
FUNCTION(PREPEND var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)
