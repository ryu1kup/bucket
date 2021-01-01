#
# CMakeLists.txt
#

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(bucket VERSION 1.0.0 LANGUAGES CXX)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
add_executable(bucket bucket.cc ${sources})

# Setting headers and C++ features
target_include_directories(bucket PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(bucket PRIVATE cxx_std_11)

# Setting Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
target_link_libraries(bucket PRIVATE ${Geant4_LIBRARIES})

# Setting ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT CONFIG REQUIRED)
target_link_libraries(bucket PRIVATE ROOT::Tree)
target_include_directories(bucket PRIVATE ${ROOT_INCLUDE_DIRS})

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS bucket DESTINATION bin)
