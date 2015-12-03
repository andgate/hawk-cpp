include_directories(${Hawk_SOURCE_DIR}/hawkc/include)
include_directories(${Hawk_SOURCE_DIR}/corelib/include)
link_directories(${Hawk_SOURCE_DIR/corelib/src})

set (HAWKC_SOURCES
  Main.cpp
  Compiler.cpp
  AST.cpp
  AST/Module.cpp
  AST/Expr.cpp
  AST/UnparsedExpr.cpp
  Parser.cpp
  Symbol.cpp
  Lexer.cpp
  Lexer/Token.cpp
  Nest.cpp
  Nest/NestConf.cpp
  jsoncpp.cpp
  Util.cpp
)

set (HAWKC_HEADERS
  Compiler.h
  AST.h
  AST/Module.h
  AST/Expr.h
  AST/UnparsedExpr.h
  Parser.h
  Symbol.h
  Lexer.h
  Lexer/Token.h
  Nest.h
  Nest/NestConf.h
  json/json.h
  json/json-forwards.h
  Util.h
)

PREPEND(HAWKC_SOURCES src ${HAWKC_SOURCES})
PREPEND(HAWKC_HEADERS include ${HAWKC_HEADERS})

add_executable(hkc ${HAWKC_SOURCES} ${HAWKC_HEADERS})


if ( CMAKE_COMPILER_IS_GNUCC )
    # This is tested
    set_property( TARGET hkc APPEND_STRING PROPERTY COMPILE_FLAGS -Wall )
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    # This is untested
    set_property( TARGET hkc APPEND_STRING PROPERTY COMPILE_FLAGS /W3 )
endif ( MSVC )