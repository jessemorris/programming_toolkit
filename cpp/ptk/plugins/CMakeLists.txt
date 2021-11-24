add_library(plugins OBJECT)



target_sources(plugins PRIVATE
    naming.cpp
    details.cpp
    plugin.cpp
)

ptk_set_global_properties(plugins)
ptk_set_ptk_lib_properties(plugins)
