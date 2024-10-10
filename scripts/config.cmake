
function(default_config main_target_name)
  message("[default include]: ${DEFAULT_INCLUDE}")
  target_include_directories(${main_target_name} PRIVATE ${DEFAULT_INCLUDE})
  target_compile_definitions(${main_target_name} PRIVATE FS_BASE_PATH="${CMAKE_SOURCE_DIR}")
  message("[${main_target_name}]filesystem base path [${FS_BASE_PATH}]")

  target_link_libraries(${main_target_name} PUBLIC OpenGL::GL)
  # local glad and glfw
  target_link_libraries(${main_target_name} PUBLIC glad glfw)

  # cross platform configuration
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # config
    target_compile_options(${main_target_name} PRIVATE ${MSVC_CXX_FLAGS})
    target_compile_definitions(${main_target_name} PRIVATE BUILD_PLATFORM_WINDOWS)
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #config
    target_compile_options(${main_target_name} PRIVATE ${GCC_CLANG_CXX_FLAGS})
    target_compile_definitions(${main_target_name} PRIVATE BUILD_PLATFORM_LINUX)
  else()
    # Unsupported
  endif()
  message("-- end")

endfunction()
