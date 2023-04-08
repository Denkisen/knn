
find_package(GTest REQUIRED)
find_package(OpenMP REQUIRED)

set(SOURCES
  "TestMatrix.cpp"
  "TestMetrics.cpp"
  "TestNormalizers.cpp"
)

include(GoogleTest)
enable_testing()
add_executable(Tests ${SOURCES})

target_include_directories(Tests PUBLIC "../inc")

set_target_properties(Tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(Tests PUBLIC
-march=native
-fopt-info-vec-optimized
-mavx
-ftree-vectorize
)
target_link_libraries(Tests PUBLIC OpenMP::OpenMP_CXX GTest::gtest_main)

gtest_discover_tests(Tests)