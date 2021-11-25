add_library(core OBJECT)



target_sources(core PRIVATE
    program.cpp
    package.cpp
)

ptk_set_global_properties(core)
ptk_set_ptk_lib_properties(core)
