
set(MAIN_TARGET_NAME neon_cube)

file(GLOB_RECURSE C_SRCS "*.c")
file(GLOB_RECURSE CC_SRCS "*.cc")
set(PROJECT_SRCS ${C_SRCS} ${CC_SRCS})

message("[SOURCES]for ${MAIN_TARGET_NAME}")
sources_print("${PROJECT_SRCS}")
add_executable(${MAIN_TARGET_NAME} ${PROJECT_SRCS} )# ${ELEMENTS_SRC})

# link to lib
add_dependencies(${MAIN_TARGET_NAME} elements)
target_link_libraries(${MAIN_TARGET_NAME} PRIVATE elements)

default_config(${MAIN_TARGET_NAME})
