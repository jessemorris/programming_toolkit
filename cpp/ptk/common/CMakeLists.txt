add_library(common OBJECT)



target_sources(common PRIVATE
   Algorithm.cpp
)

ptk_set_global_properties(common)
ptk_set_ptk_lib_properties(common)
