add_library(utilities OBJECT)



target_sources(utilities PRIVATE
   statistics.cpp
   program.cpp
   fileSystem.cpp
)

ptk_set_global_properties(utilities)
ptk_set_ptk_lib_properties(utilities)
