function(sources_print src_list )
  message("=====================================")
  foreach(item IN LISTS src_list)
    message("${item}")
  endforeach()

  message("=====================================")

endfunction()
