# ptk_set_ptk_lib_properties() sets properties for the PTK lib itself.
# This should be used for all object libraries that make up the PTK lib.
#
# In comparison, ptk_set_global_properties() sets properties for the PTK
# lib, and targets that links to the PTK lib, e.g pybind, unit tests, etc.
function(ptk_set_ptk_lib_properties target)
    # cmake_parse_arguments(arg "HIDDEN" "" "" ${ARGN})
    # if(NOT BUILD_SHARED_LIBS)
    #     target_compile_definitions(${target} PUBLIC PTK_STATIC)
    # endif()
    if (arg_HIDDEN)
        set_target_properties(${target} PROPERTIES
                        CXX_VISIBILITY_PRESET hidden
                        VISIBILITY_INLINES_HIDDEN ON
                        )
    else ()
        target_compile_definitions(${target} PRIVATE PTK_ENABLE_DLL_EXPORTS)
    endif()

endfunction()

# Configure a header file to pass the version settings to the source code
configure_file("${PROJECT_SOURCE_DIR}/cpp/ptk/ptk.h.in"
               "${PROJECT_SOURCE_DIR}/cpp/ptk/ptk.h")
configure_file("${PROJECT_SOURCE_DIR}/cpp/ptk/ptkConfig.h.in"
               "${PROJECT_SOURCE_DIR}/cpp/ptk/ptkConfig.h")


add_library(ptk SHARED)

add_subdirectory(utilities)
add_subdirectory(plugins)
add_subdirectory(core)
# add_subdirectory(common)


# note: adding at least one real source file to any target that references
# reference: https://cmake.org/cmake/help/v3.12/command/add_library.html#object-libraries
target_sources(ptk PRIVATE
    $<TARGET_OBJECTS:utilities>
    $<TARGET_OBJECTS:plugins>
    $<TARGET_OBJECTS:core>
    # $<TARGET_OBJECTS:common>

)

# Source group for Visual Studio
add_source_group(utils)
add_source_group(plugins)
add_source_group(core)
# add_source_group(common)
# add_source_group(geometry)
# add_source_group(tgeometry)
# add_source_group(io)
# add_source_group(tio)
# add_source_group(ml)
# add_source_group(pipelines)
# add_source_group(tpipelines)
# add_source_group(utility)
# add_source_group(visualization)

# open3d_show_and_abort_on_warning(Open3D)
ptk_set_global_properties(ptk)
ptk_set_ptk_lib_properties(ptk)
# open3d_link_3rdparty_libraries(Open3D)

# If we are building a STATIC_LIBRARY, hide symbols coming from 3rd party static
# libraries that are not hidden during compilation. Don't propagate beyond
# direct consumers of libOpen3D.a
target_link_options(ptk INTERFACE
    $<$<STREQUAL:$<TARGET_PROPERTY:ptk,TYPE>,STATIC_LIBRARY>:$<LINK_ONLY:${PTK_HIDDEN_3RDPARTY_LINK_OPTIONS}>>)

add_library(ptk::ptk ALIAS ptk)

target_link_libraries(ptk ${CMAKE_DL_LIBS})

include(CMakePackageConfigHelpers)

# find_package ptk
configure_package_config_file(ptkConfig.cmake.in
                              "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ptkConfig.cmake"
                              INSTALL_DESTINATION ${PTK_INSTALL_CMAKE_DIR}
                              PATH_VARS PTK_INSTALL_INCLUDE_DIR
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# find_package PTK Version
write_basic_package_version_file("${PROJECT_BINARY_DIR}/ptkConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY ExactVersion)

# Installation
install(TARGETS ptk EXPORT ptkTargets
        RUNTIME DESTINATION ${PTK_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PTK_INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${PTK_INSTALL_LIB_DIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION ${PTK_INSTALL_INCLUDE_DIR}
    FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.cuh"
)

# Install the ptk.cmake and ptkConfigVersion.cmake
install(FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ptkConfig.cmake"
        "${PROJECT_BINARY_DIR}/ptkConfigVersion.cmake"
        "${PROJECT_SOURCE_DIR}/cmake/ptkPlugin.cmake"
        DESTINATION "${PTK_INSTALL_CMAKE_DIR}" COMPONENT dev)

# uninstall target
# if(NOT TARGET uninstall)
#     configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#                    IMMEDIATE @ONLY)

#     add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P
#                       ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
# endif()

# # Export GUI_RESOURCE_FILES to parent CMake context (cpp/open3d/)
# set(GUI_RESOURCE_FILES ${GUI_RESOURCE_FILES} PARENT_SCOPE)
# set(GUI_RESOURCE_DIR ${GUI_RESOURCE_DIR} PARENT_SCOPE)