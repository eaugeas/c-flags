add_library(flags STATIC flags.c strings.c parse.c)
target_link_libraries(flags)
set_target_properties(flags PROPERTIES PUBLIC_HEADER "flags.h;parse.h;strings.h")

c_verify_clang_format(flags)
c_verify_clang_tidy(flags)

install(TARGETS flags
   LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
   PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include/flags)
