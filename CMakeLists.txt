cmake_minimum_required(VERSION 3.20)

project(FormationCpp)
set (CMAKE_CXX_STANDARD 17)

if(MSVC)
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
 endif()
 
file(GLOB sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB headers
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(${PROJECT_NAME}
  ${sources}
  ${headers}
)

target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ${my_lib}
)
