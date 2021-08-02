cmake_minimum_required(VERSION 3.19)
# If you're using Ubuntu 18.04, we suggest you install the latest CMake from the
# official repository https://apt.kitware.com/.  CMake 3.19+ is required to
# allow linking with OBJECT libraries, to prevent erroneous -gencode option
# deduplication with CUDA, to simplify generator expressions for selecting
# compile flags and setting global hardened link flags, and to parse JSON files
# for downloading files from the open3d_downloads repository.

# if (APPLE)
# set (CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING
#     "Minimum OS X deployment version" FORCE)
# endif()

# if(UNIX AND NOT APPLE)
#     # CMAKE_HOST_SYSTEM_PROCESSOR is only available after calling project(),
#     # which depends on ${OPEN3D_VERSION}, which depends on ${DEVELOPER_BUILD}.
#     execute_process(COMMAND uname -p
#         OUTPUT_VARIABLE PROCESSOR_ARCH
#         OUTPUT_STRIP_TRAILING_WHITESPACE
#     )
#     if(PROCESSOR_ARCH STREQUAL "aarch64")
#         set(LINUX_AARCH64 TRUE)
#     endif()
# endif()

include(CMakeDependentOption)

# # Open3D build options
# option(BUILD_SHARED_LIBS          "Build shared libraries"                   OFF)
option(BUILD_EXAMPLES             "Build PTK examples programs"           ON )
option(BUILD_UNIT_TESTS           "Build PTK unit tests"                  OFF)
# option(BUILD_BENCHMARKS           "Build the micro benchmarks"               OFF)
# option(BUILD_PYTHON_MODULE        "Build the python module"                  ON )
# option(BUILD_CUDA_MODULE          "Build the CUDA module"                    OFF)
# option(BUILD_COMMON_CUDA_ARCHS    "Build for common CUDA GPUs (for release)" OFF)
# option(BUILD_CACHED_CUDA_MANAGER  "Build the cached CUDA memory manager"     ON )
# option(BUILD_GUI                  "Builds new GUI"                           ON )
# option(WITH_OPENMP                "Use OpenMP multi-threading"               ON )
# option(WITH_IPPICV                "Use Intel Performance Primitives"         ON )
# option(ENABLE_HEADLESS_RENDERING  "Use OSMesa for headless rendering"        OFF)
# cmake_dependent_option(
#        STATIC_WINDOWS_RUNTIME     "Use static (MT/MTd) Windows runtime"      ON
#        "NOT BUILD_SHARED_LIBS"                                               OFF)
# option(GLIBCXX_USE_CXX11_ABI      "Set -D_GLIBCXX_USE_CXX11_ABI=1"           OFF)
# if(WIN32 OR UNIX AND NOT LINUX_AARCH64)
#     cmake_dependent_option(BUILD_WEBRTC "Build WebRTC visualizer" ON "BUILD_GUI" OFF)
# else()
#     option(BUILD_WEBRTC           "Build WebRTC visualizer"                  OFF)
# endif()
# option(BUILD_JUPYTER_EXTENSION    "Build Jupyter, requires BUILD_WEBRTC=ON"  OFF)


# Parse Open3D version number
file(STRINGS "cpp/ptk/version.txt" PTK_VERSION_READ)
foreach(ver ${PTK_VERSION_READ})
    if (ver MATCHES "PTK_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(PTK_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(PTK_VERSION_DEVHASH "")
# if(DEVELOPER_BUILD)
#     execute_process(COMMAND git -C "${CMAKE_SOURCE_DIR}" log --pretty=format:%h -n 1
#         OUTPUT_VARIABLE GIT_REV)
#     if (GIT_REV)
#         set(OPEN3D_VERSION_DEVHASH "+${GIT_REV}")
#     endif()
# endif()
string(CONCAT PTK_VERSION
    "${PTK_VERSION_MAJOR}"
)
set(PTK_VERSION_FULL "${PTK_VERSION}${PTK_VERSION_DEVHASH}")

project(ptk VERSION ${PTK_VERSION} LANGUAGES C CXX)
message(STATUS "PTK ${PTK_VERSION_FULL}")


# CMake modules
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/CMake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# # Setup Python executable
# if(NOT DEFINED Python3_FIND_REGISTRY)
#     # Only consider PATH variable on Windows by default
#     set(Python3_FIND_REGISTRY NEVER)
# endif()
# find_package(Python3 3.6...<3.10
#              COMPONENTS Interpreter Development
# )
# if (Python3_FOUND)
#     # Setup PYTHON_EXECUTABLE for 3rdparty modules
#     # which still use the deprecated find_package(PythonInterp)
#     set(PYTHON_EXECUTABLE ${Python3_EXECUTABLE} CACHE STRING
#         "Deprecated path to the Python executable (for 3rdparty only)" FORCE)
# else()
#     if (BUILD_PYTHON_MODULE)
#         message(FATAL_ERROR "BUILD_PYTHON_MODULE=ON requires Python 3.6-3.9. Please ensure it is in PATH.")
#     endif()
# endif()

# # npm version has to be MAJOR.MINOR.PATCH
# string(CONCAT PROJECT_VERSION_THREE_NUMBER "${OPEN3D_VERSION_MAJOR}"
#                                            ".${OPEN3D_VERSION_MINOR}"
#                                            ".${OPEN3D_VERSION_PATCH}")

# PyPI package name controls specifies the repository name on PyPI. The default
# name is "open3d". In the past, for historical reasons, we've used the
# following names for PyPI, while they are now deprecated:
# - open3d-python
# - py3d
# - open3d-original
# - open3d-official
# - open-3d
# if(NOT DEFINED PYPI_PACKAGE_NAME)
#     set(PYPI_PACKAGE_NAME "open3d")
# endif()

# Set additional info
set(PROJECT_EMAIL   "")
set(PROJECT_HOME    "")
set(PROJECT_DOCS    "")
set(PROJECT_CODE    "")
set(PROJECT_ISSUES  "")

# Set installation paths
if(UNIX OR CYGWIN)
    include(GNUInstallDirs)
    set(PTK_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(PTK_INSTALL_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
    set(PTK_INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(PTK_INSTALL_CMAKE_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
else()
    set(PTK_INSTALL_INCLUDE_DIR include)
    set(PTK_INSTALL_BIN_DIR bin)
    set(PTK_INSTALL_LIB_DIR lib)
    set(PTK_INSTALL_CMAKE_DIR CMake)
endif()

# Put build results in some predictable places
# The $<CONFIG> generator expression makes sure that XCode or Visual Studio do not
# append additional path components, as we need to know *exactly* where the build results
# end up.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Global Security options (including 3rd party code)
# Add -fPIC for library and -fPIE for executable to compiler and linker. Does not add -pie !
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# include(Open3DMakeHardeningFlags)
# open3d_make_hardening_flags(HARDENING_CFLAGS HARDENING_LDFLAGS)
# open3d_make_hardening_definitions(HARDENING_DEFINITIONS)

# message(STATUS "Using security hardening compiler flags: ${HARDENING_CFLAGS}")
# message(STATUS "Using security hardening linker flags: ${HARDENING_LDFLAGS}")
# message(STATUS "Using security hardening compiler definitions: ${HARDENING_DEFINITIONS}")

# add_compile_options("${HARDENING_CFLAGS}")
# add_link_options("${HARDENING_LDFLAGS}")
# add_compile_definitions("${HARDENING_DEFINITIONS}")

# Build CUDA module by default if CUDA is available
# if(BUILD_CUDA_MODULE)
#     include(Open3DMakeCudaArchitectures)
#     open3d_make_cuda_architectures(CUDA_ARCHS)
#     set(CMAKE_CUDA_ARCHITECTURES ${CUDA_ARCHS})

#     message(STATUS "Using CUDA architectures: ${CMAKE_CUDA_ARCHITECTURES}")
#     enable_language(CUDA)

#     if (CMAKE_CUDA_COMPILER_ID STREQUAL "NVIDIA" AND CMAKE_CUDA_COMPILER_VERSION VERSION_LESS "10.1")
#         message(FATAL_ERROR "CUDA 10.0 and older are not supported. Please upgrade to CUDA 10.1 or newer.")
#     endif()
# endif ()

# OS specific settings
# if(WIN32)
#     # Windows defaults to hidden symbol visibility, override that
#     # TODO: It would be better to explictly export symbols.
#     #       Then, we could use -fvisibility=hidden for Linux as well
#     SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#     if(MSVC)
#         # Make sure we don't hit the 65535 object member limit with MSVC
#         #
#         # /bigobj allows object files with more than 65535 members
#         # /Ob2 enables function inlining, because MSVC is particularly
#         # verbose with inline members
#         #
#         # See: https://github.com/tensorflow/tensorflow/pull/10962
#         add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:/bigobj;/Ob2>")
#     endif()
#     if (STATIC_WINDOWS_RUNTIME)
#         set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#     else()
#         set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
#     endif()
# endif()

# Downloads files from https://github.com/isl-org/open3d_downloads to
# Open3D/examples/test_data/open3d_downloads.
# add_custom_target(open3d_downloads
#     COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/examples/test_data/download_test_data.cmake
#     COMMENT ""
# )

# Folder view for project files
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check if the compiler defines the _GLIBCXX_USE_CXX11_ABI macro
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("#include <cxxabi.h>
int main() { return _GLIBCXX_USE_CXX11_ABI; }" HAS_GLIBCXX_USE_CXX11_ABI)

macro(add_source_group module_name)
    file(GLOB MODULE_HEADER_FILES "${module_name}/*.h")
    source_group("Header Files\\${module_name}" FILES ${MODULE_HEADER_FILES})
    file(GLOB MODULE_SOURCE_FILES "${module_name}/*.cpp")
    source_group("Source Files\\${module_name}" FILES ${MODULE_SOURCE_FILES})
    file(GLOB MODULE_SHADER_FILES "${module_name}/*.glsl")
    source_group("Source Files\\Shader\\GLSL" FILES ${MODULE_SHADER_FILES})
    file(GLOB MODULE_MATERIAL_FILES "${module_name}/*.mat")
    source_group("Source Files\\Material" FILES ${MODULE_MATERIAL_FILES})
endmacro()

# Include convenience functions
# include(Open3DLink3rdpartyLibraries)
include(PtkSetGlobalProperties)
# include(Open3DShowAndAbortOnWarning)

# Enumerate all third-party libraries which we need later
# This creates the necessary targets and sets the
# Open3D_3RDPARTY_*_TARGETS variables we use in open3d_link_3rdparty_libraries
# include(3rdparty/find_dependencies.cmake)

# Open3D library
add_subdirectory(cpp)

# Examples
add_subdirectory(examples)

# Documentation
# add_subdirectory(docs)

# Install CMake configuration files
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE ${PROJECT_NAME}:: DESTINATION ${PTK_INSTALL_CMAKE_DIR})
export(EXPORT ${PROJECT_NAME}Targets NAMESPACE ${PROJECT_NAME}::)

# if (Python3_EXECUTABLE)
#     # `make check-style` checks style for c++/cuda/python/ipynb files
#     add_custom_target(check-style
#         COMMAND ${Python3_EXECUTABLE}
#         ${CMAKE_CURRENT_SOURCE_DIR}/util/check_style.py
#         COMMENT "Python executable used for style check: ${Python3_EXECUTABLE}."
#     )

#     # `make apply-style` applies style for c++/cuda/python/ipynb files
#     add_custom_target(apply-style
#         COMMAND ${Python3_EXECUTABLE}
#         ${CMAKE_CURRENT_SOURCE_DIR}/util/check_style.py --do_apply_style
#         COMMENT "Python executable used for style check: ${Python3_EXECUTABLE}."
#     )
# endif()

# `make check-cpp-style` checks style for c++/cuda files.
# This works outside of python virtualenv.
# add_custom_target(check-cpp-style
#     COMMAND ${CMAKE_COMMAND}
#     -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
#     -DDO_APPLY_STYLE=OFF
#     -P ${CMAKE_CURRENT_SOURCE_DIR}/util/check_cpp_style.cmake
# )

# `make apply-cpp-style` applies style for c++/cuda files.
# This works outside of python virtualenv.
# add_custom_target(apply-cpp-style
#     COMMAND ${CMAKE_COMMAND}
#     -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
#     -DDO_APPLY_STYLE=ON
#     -P ${CMAKE_CURRENT_SOURCE_DIR}/util/check_cpp_style.cmake
# )

# include(Open3DPrintConfigurationSummary)
# open3d_print_configuration_summary()