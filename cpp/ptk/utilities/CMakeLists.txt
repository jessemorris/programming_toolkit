add_library(utilities OBJECT)



target_sources(utilities PRIVATE
   fileSystem.cpp
   impl/outputHandler.cpp
   impl/loggingHandler.cpp
   impl/logging.cpp
   impl/fileHandler.cpp
   impl/logging-common.cpp
)

ptk_set_global_properties(utilities)
ptk_set_ptk_lib_properties(utilities)
