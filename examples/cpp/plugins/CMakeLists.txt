cmake_minimum_required(VERSION 3.10)
project(plugin_example)


set(CMAKE_CXX_STANDARD 17)

find_package(ptk)


add_plugin(plugin_example
     testPlugin.cpp    
)


# Include directories
# target_include_directories(cpp_plugins PUBLIC
#         include
#         )



