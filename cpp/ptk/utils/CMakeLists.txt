add_library(utils OBJECT)



target_sources(utils PRIVATE
   statistics.cpp
)

ptk_set_global_properties(utils)
ptk_set_ptk_lib_properties(utils)
