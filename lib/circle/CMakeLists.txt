
set(MAIN_TARGET_NAME circle)

file(GLOB_RECURSE C_SRCS "*.c")
file(GLOB_RECURSE CC_SRCS "*.cc")
set(PROJECT_SRCS ${C_SRCS} ${CC_SRCS})

message("[SOURCES]for ${MAIN_TARGET_NAME}")
sources_print("${PROJECT_SRCS}")
add_library(${MAIN_TARGET_NAME} ${PROJECT_SRCS}) # ${ELEMENTS_SRC})

default_config(${MAIN_TARGET_NAME})
