include_directories(${Hawk_SOURCE_DIR}/hawkc/include)
include_directories(${Hawk_SOURCE_DIR}/corelib/include)
link_directories(${Hawk_SOURCE_DIR/corelib/src})

set (HAWKC_SOURCES
  Main.cpp
  Compiler.cpp
  AST.cpp
  Parser.cpp
  Lexer.cpp
  Lexer/Token.cpp
  Project.cpp
  Project/ProjectFinder.cpp
  Project/ProjectBuilder.cpp
  Project/ProjectConf.cpp
)

set (HAWKC_HEADERS
  Compiler.h
  AST.h
  Parser.h
  Lexer.h
  Lexer/Token.h
  Project.h
  Project/ProjectFinder.h
  Project/ProjectBuilder.h
  Project/ProjectConf.h
)

PREPEND(HAWKC_SOURCES src ${HAWKC_SOURCES})
PREPEND(HAWKC_HEADERS include ${HAWKC_HEADERS})

add_executable(hkc ${HAWKC_SOURCES} ${HAWKC_HEADERS})
