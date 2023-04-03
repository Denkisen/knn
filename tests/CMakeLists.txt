
find_package(GTest REQUIRED)
find_package(OpenMP REQUIRED)

set(SOURCES
  "ut.cpp"
  "TestMatrix.cpp")

add_executable(Tests ${SOURCES})

set_target_properties(Tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

target_compile_options(Tests PUBLIC
  -march=native
  -fopt-info-vec-optimized
  -mavx
  -ftree-vectorize
)

set_target_properties(Tests
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(Tests PUBLIC "../inc")

target_link_libraries(Tests PUBLIC ${GTEST_LIBRARIES} OpenMP::OpenMP_CXX)