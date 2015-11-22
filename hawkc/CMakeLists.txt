include_directories(${Hawk_SOURCE_DIR}/hawkc/include)
include_directories(${Hawk_SOURCE_DIR}/corelib/include)
link_directories(${Hawk_SOURCE_DIR/corelib/src})

set (HAWKC_SOURCES
  Main.cpp
  tokenizer.cpp
  lex.cpp
  lex/SourceFile.cpp
  lex/SourceLine.cpp
)

set (HAWKC_HEADERS
  tokenizer.h
  lex.h
  lex/SourceFile.h
  lex/SourceLine.h
)

PREPEND(HAWKC_SOURCES src ${HAWKC_SOURCES})
PREPEND(HAWKC_HEADERS include ${HAWKC_HEADERS})

add_executable(hkc ${HAWKC_SOURCES} ${HAWKC_HEADERS})

target_link_libraries(hkc corelib)
