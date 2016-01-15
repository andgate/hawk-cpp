include_directories(${Hawk_SOURCE_DIR}/hkc/include)

BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp HEADER ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp [COMPILE_FLAGS -v])
FLEX_TARGET(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set (HKC_LIB_SOURCES
  compile.cpp
  parser/driver.cpp
  codegen.cpp
)

set (HKC_LIB_HEADERS
  build.h
  compile.h
  codegen.h
  parser/driver.h
  parser/node.h
  utils/ReadFile.h
)

set (HKC_BIN_SOURCES
  main.cpp
)

set (HKC_BIN_HEADERS
)

PREPEND(HKC_LIB_SOURCES src/hkc ${HKC_LIB_SOURCES})
PREPEND(HKC_LIB_HEADERS include/hkc ${HKC_LIB_HEADERS})

PREPEND(HKC_BIN_SOURCES src/hkc ${HKC_BIN_SOURCES})
PREPEND(HKC_BIN_HEADERS include/hkc ${HKC_BIN_HEADERS})

add_library (hkclib STATIC ${HKC_LIB_SOURCES} ${HKC_LIB_HEADERS} ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS})
add_executable(hkc ${HKC_BIN_SOURCES} ${HKC_BIN_HEADERS})
target_link_libraries(hkc hkclib)


if ( CMAKE_COMPILER_IS_GNUCC )
    # This is tested
    set_property( TARGET hkclib APPEND_STRING PROPERTY COMPILE_FLAGS -Wall )
    set_property( TARGET hkc APPEND_STRING PROPERTY COMPILE_FLAGS -Wall )
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    # This is untested
    set_property( TARGET hkclib APPEND_STRING PROPERTY COMPILE_FLAGS /W3 )
    set_property( TARGET hkc APPEND_STRING PROPERTY COMPILE_FLAGS /W3 )
endif ( MSVC )
