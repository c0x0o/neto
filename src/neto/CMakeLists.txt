include_directories(${neto_SOURCE_DIR}/include)

file(GLOB BASE_SRC base/*.cc)

set(ALL_SRC ${BASE_SRC})

add_library(neto SHARED ${ALL_SRC})
target_link_libraries(neto pthread)
set_target_properties(neto
  PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

