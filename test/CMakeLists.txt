include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${Hawk_SOURCE_DIR}/hkc/include)

set (SOURCE_FILES
  
)

set (HEADER_FILES
)

add_executable(runTests ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(runTests hkclib ${GTEST_LIBRARIES} pthread)