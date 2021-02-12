project(rename-cpp)

#**************************************************************************************************
# General cMake settings **************************************************************************
cmake_minimum_required(VERSION 3.5)

#**************************************************************************************************
# Find Package ************************************************************************************
find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)


#**************************************************************************************************
# Include *****************************************************************************************
include_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

#**************************************************************************************************
# Set compiler ************************************************************************************
set(CMAKE_CXX_FLAGS "-fPIC -fpermissive -std=c++2a")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#**************************************************************************************************
# Include *****************************************************************************************



#**************************************************************************************************
# Set variable ************************************************************************************
SET(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

#**************************************************************************************************
# Make configuration ******************************************************************************
add_executable(rename ${SOURCES})
set_property(TARGET rename PROPERTY CXX_STANDARD 20)
target_link_libraries(rename -pthread -lstdc++fs fmt::fmt spdlog::spdlog)
