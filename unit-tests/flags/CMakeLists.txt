add_executable(flags_test flags_test.c prints.c)
target_link_libraries(flags_test flags)
target_include_directories(flags_test 
  PRIVATE ${CMAKE_SOURCE_DIR}/source)
add_test(flags_test flags_test)

c_verify_clang_format(flags-unit-tests)
c_verify_clang_tidy(flags-unit-tests)
