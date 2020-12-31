#
# CMakeLists.txt
#

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(bucket)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(bucket bucket.cc ${sources})

# Setting ROOT
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
target_link_libraries(bucket PRIVATE ${Geant4_LIBRARIES})

target_include_directories(bucket PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_features(bucket PRIVATE cxx_std_11)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS bucket DESTINATION bin)
