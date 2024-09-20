
file(GLOB_RECURSE GLAD_SRCS "*.c" "*.cc")

message("[SOURCES]for glad")
sources_print(${GLAD_SRCS})
add_library(glad STATIC ${GLAD_SRCS})

target_include_directories(glad PRIVATE "../../include/")

