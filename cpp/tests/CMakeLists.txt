add_executable(tests)

add_subdirectory(common)

find_package(GTest REQUIRED)

target_sources(tests PRIVATE
    Main.cpp
)


target_include_directories(tests PRIVATE ".")

target_compile_definitions(tests PRIVATE TEST_DATA_DIR="${PROJECT_SOURCE_DIR}/examples/test_data")

target_link_libraries(tests PRIVATE
    ptk::ptk
    GTest::GTest 
    GTest::Main
)

ptk_set_global_properties(tests)