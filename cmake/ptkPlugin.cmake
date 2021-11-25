function(add_plugin libName)
    add_library(${libName} SHARED
        ${ARGN}
    )
    target_link_libraries(${libName} ptk::ptk ${CMAKE_DL_LIBS})

endfunction()

#TODO: maybe need to use the export line here - need to understand CMake better
#not sure if CMAKE_BIANRY_DIR is always going to be right... need to check this
function(install_plugin libName)
    SET(libNamePath "${libName}")
    find_plugin_libs(${libName} libNamePath)
        message("Installing ${libName} to ${PTK_INSTALL_LIB_DIR}/ptk_plugins")
    install(FILES ${libNamePath}
        DESTINATION ${PTK_INSTALL_LIB_DIR}/ptk_plugins
    )
endfunction()


function(find_plugin_libs libName libPath)
    set(OF_LIB_LIST ${libName})

    foreach(of_libname IN LISTS OF_LIB_LIST)
        message("Trying to find ${of_libname} library")
        # message("CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}")
        find_library(of_lib_path ${of_libname} HINTS ${CMAKE_BINARY_DIR}/lib)
        if(NOT of_lib_path)
            message(WARNING "${of_libname} NOT FOUND in ${CMAKE_BINARY_DIR}/lib.")
            continue()
        else()
            message("${of_libname} lib is FOUND as ${of_lib_path}.")
            SET(${libPath} ${of_lib_path} PARENT_SCOPE)
            break()
        endif()
    endforeach()
endfunction()
