include_directories(${Hawk_SOURCE_DIR}/nest/include)
include_directories(${Hawk_SOURCE_DIR}/hkc/include)

set (NEST_SOURCES
  main.cpp
  nest.cpp
  config.cpp
  find.cpp
  load.cpp
  gen_build.cpp
)

set (NEST_HEADERS
  resolve_path.h
  constants.h
  nest.h
  config.h
  find.h
  load.h
  gen_build.h
)

set (EXTRA_SOURCES
  src/jsoncpp.cpp
  include/json/json.h
  include/json/json-forwards.h
)

PREPEND(NEST_SOURCES src/nest ${NEST_SOURCES})
PREPEND(NEST_HEADERS include/nest ${NEST_HEADERS})

add_executable(nest ${NEST_SOURCES} ${NEST_HEADERS} ${EXTRA_SOURCES})
target_link_libraries(nest hkclib)

if ( CMAKE_COMPILER_IS_GNUCC )
    # This is tested
    set_property( TARGET nest APPEND_STRING PROPERTY COMPILE_FLAGS -Wall )
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    # This is untested
    set_property( TARGET nest APPEND_STRING PROPERTY COMPILE_FLAGS /W3 )
endif ( MSVC )
