include_directories(${Hawk_SOURCE_DIR}/hawkc/include)
include_directories(${Hawk_SOURCE_DIR}/corelib/include)
link_directories(${Hawk_SOURCE_DIR/corelib/src})

set (HAWKC_SOURCES
  Main.cpp
  Parser.cpp
  Lexer.cpp
  Lexer/Token.cpp
)

set (HAWKC_HEADERS
  Parser.h
  Lexer.h
  Lexer/Token.h
)

PREPEND(HAWKC_SOURCES src ${HAWKC_SOURCES})
PREPEND(HAWKC_HEADERS include ${HAWKC_HEADERS})

add_executable(hkc ${HAWKC_SOURCES} ${HAWKC_HEADERS})

target_link_libraries(hkc corelib)
