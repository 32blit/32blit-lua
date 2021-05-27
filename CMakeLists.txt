cmake_minimum_required(VERSION 3.8)

project(32blit_lua)
add_subdirectory("lua")

set(PROJECT_SOURCE
  main.cpp
  luablitlib.cpp
  lua-bindings/pen.cpp
  lua-bindings/point.cpp
  lua-bindings/size.cpp
  lua-bindings/vec2.cpp
  lua-bindings/rect.cpp
  lua-bindings/sprites.cpp
  lua-bindings/timer.cpp)
set(PROJECT_DISTRIBS LICENSE README.md)

# Build configuration; approach this with caution!
if(MSVC)
  add_compile_options("/W4" "/wd4244" "/wd4324")
else()
  add_compile_options("-Wall" "-Wextra" "-Wdouble-promotion")
endif()
find_package(32BLIT CONFIG REQUIRED PATHS ../ /opt)

blit_executable (${PROJECT_NAME} ${PROJECT_SOURCE})
blit_assets_yaml (${PROJECT_NAME} assets.yml)
blit_metadata (${PROJECT_NAME} metadata.yml)

target_include_directories(${PROJECT_NAME} PRIVATE lua)

target_link_libraries(${PROJECT_NAME} lua)
add_custom_target (flash DEPENDS ${PROJECT_NAME}.flash)

# setup release packages
install (FILES ${PROJECT_DISTRIBS} DESTINATION .)
set (CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set (CPACK_GENERATOR "ZIP" "TGZ")
include (CPack)
