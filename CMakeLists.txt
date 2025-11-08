cmake_minimum_required(VERSION 3.24...4.1)
project(
  ${SKBUILD_PROJECT_NAME}
  DESCRIPTION "A Python Package for Analyzing Lattice Polytopes"
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES C CXX)

set(POLY_Dmax
    11
    CACHE STRING "Maximum polytope dimension")

include(FetchContent)
FetchContent_Declare(
  palp
  GIT_REPOSITORY https://github.com/ariostas/PALP.git
  GIT_TAG main)
FetchContent_MakeAvailable(palp)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(pypalp src/pypalp/pypalp.cpp)
target_link_libraries(pypalp PRIVATE objects class_objects nef_objects
                                     mori_objects)
target_compile_definitions(pypalp PRIVATE POLY_Dmax=${POLY_Dmax})
target_include_directories(pypalp PRIVATE ${CMAKE_SOURCE_DIR}/extern/palp
                                          ${CMAKE_SOURCE_DIR}/include)
target_compile_features(pypalp PRIVATE cxx_std_17)

install(TARGETS pypalp DESTINATION .)
