include_directories(${Hawk_SOURCE_DIR}/corelib/include)

set (CORE_SOURCES
  Core.cpp
)

set (CORE_HEADERS
  Core.h
)

PREPEND(CORE_SOURCES src ${CORE_SOURCES})
PREPEND(CORE_HEADERS include ${CORE_HEADERS})

add_library(corelib ${CORE_SOURCES} ${CORE_HEADERS})
