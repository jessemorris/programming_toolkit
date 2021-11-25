set(EXAMPLE_BIN_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/examples")

macro(ptk_add_example EXAMPLE_CPP_NAME)
    add_executable(${EXAMPLE_CPP_NAME})

    target_sources(${EXAMPLE_CPP_NAME} PRIVATE "${EXAMPLE_CPP_NAME}.cpp")
    target_link_libraries(${EXAMPLE_CPP_NAME} PRIVATE ptk::ptk ${ARGN})

    # Don't use ptk_link_3rdparty_libraries(${EXAMPLE_CPP_NAME}).
    # Unlike benchmarks, examples are intended as standalone apps that link
    # the main Open3D library, while the benchmarks executable needs 3rd-party
    # libraries since it measures the internal componets of Open3D.
    # open3d_show_and_abort_on_warning(${EXAMPLE_CPP_NAME})
    ptk_set_global_properties(${EXAMPLE_CPP_NAME})

    set_target_properties(${EXAMPLE_CPP_NAME} PROPERTIES
        FOLDER "examples/cpp/"
        RUNTIME_OUTPUT_DIRECTORY "${EXAMPLE_BIN_DIR}"
    )

    if (NOT BUILD_EXAMPLES)
        set_target_properties(${EXAMPLE_CPP_NAME} PROPERTIES
            EXCLUDE_FROM_ALL TRUE
        )
    endif()

    list(APPEND EXAMPLE_TARGETS ${EXAMPLE_CPP_NAME})
endmacro()


add_subdirectory(plugins)


ptk_add_example(plugin_main)
ptk_add_example(statistics)