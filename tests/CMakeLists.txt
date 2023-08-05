add_executable(string_test string_test.c)

target_link_libraries(string_test PUBLIC vstring check pthread)

target_include_directories(string_test PUBLIC "${PROJECT_BINARY_DIR}")

add_test(NAME string_test COMMAND string_test WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Testing)
set_tests_properties(string_test PROPERTIES TIMEOUT 30)
