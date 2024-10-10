

set(MAIN_TARGET_NAME solar_system)

file(GLOB_RECURSE C_SRCS "*.c")
file(GLOB_RECURSE CC_SRCS "*.cc")
set(PROJECT_SRCS ${C_SRCS} ${CC_SRCS})

message("[SOURCES]for ${MAIN_TARGET_NAME}")
sources_print("${PROJECT_SRCS}")
add_executable(${MAIN_TARGET_NAME} ${PROJECT_SRCS}) # ${ELEMENTS_SRC})

# link to lib
add_dependencies(${MAIN_TARGET_NAME} elements circle)
target_link_libraries(${MAIN_TARGET_NAME} PRIVATE elements circle)

default_config(${MAIN_TARGET_NAME})
