add_library(flags STATIC flags.c strings.c parse.c)
target_link_libraries(flags)

c_verify_clang_format(flags)
c_verify_clang_tidy(flags)
