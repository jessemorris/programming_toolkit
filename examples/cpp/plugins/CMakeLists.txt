cmake_minimum_required(VERSION 3.10)
project(plugin_example)


set(CMAKE_CXX_STANDARD 17)

find_package(ptk)


add_plugin(plugin_example
     testPlugin.cpp    
)

install_plugin(plugin_example)





