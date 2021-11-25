add_library(plugins OBJECT)



target_sources(plugins PRIVATE
    LibraryLoader.cpp
    naming.cpp
    details.cpp
    plugin.cpp
)

target_link_libraries(plugins ${CMAKE_DL_LIBS})

ptk_set_global_properties(plugins)
ptk_set_ptk_lib_properties(plugins)
