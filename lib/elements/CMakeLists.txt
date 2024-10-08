file(GLOB_RECURSE ELEMENTS_SRC "*.c" "*.cc")

message("[SOURCES]for elements")
sources_print(${ELEMENTS_SRC})
add_library(elements STATIC ${ELEMENTS_SRC})

default_config(elements)
