function(add_plugin libName)
    add_library(${libName} SHARED
        ${ARGN}
    )
    target_link_libraries(${libName} ptk)

endfunction()
