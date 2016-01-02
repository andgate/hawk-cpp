include_directories(${Hawk_SOURCE_DIR}/hkc/include)

set (HKC_LIB_SOURCES
  build.cpp
  compile.cpp
  symbol.cpp
  parser/Parser.cpp
  parser/Lexer.cpp
)

set (HKC_LIB_HEADERS
  build.h
  compile.h
  symbol.h
  parser/Parser.h
  parser/Lexer.h
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

add_library (hkclib STATIC ${HKC_LIB_SOURCES} ${HKC_LIB_HEADERS})
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
