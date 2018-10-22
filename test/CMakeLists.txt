include(GoogleTest)

add_subdirectory(googletest
                 ${CMAKE_BINARY_DIR}/test/neto/googletest
                 )

include_directories(${neto_SOURCE_DIR}/include)
include_directories(${neto_SOURCE_DIR}/test/googletest/googletest/include)

link_directories(${CMAKE_BINARY_DIR}/lib)

file(GLOB BASE_TEST_SRC base/*.cc)

set(ALL_TEST_SRC ./main.cc ${BASE_TEST_SRC})

add_executable(neto-test ${ALL_TEST_SRC})
target_link_libraries(neto-test neto gtest)
set_target_properties(neto-test
                      PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/neto)

if(${CMAKE_VERSION} VERSION_LESS "3.10.0")
  gtest_add_tests(TARGET neto-test)
else()
  gtest_discover_tests(neto-test)
endif()
