#
# project: sokol-test
#
cmake_minimum_required(VERSION 2.8)
project(sokol-test)
set(CMAKE_C_STANDARD 99)

# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

fips_setup()
fips_ide_group("")
add_definitions(-DSOKOL_GLCORE33)
set(slang "glsl330")

fips_project(sokol-test-project)
fips_ide_group("libs")
fips_add_subdirectory(libs)
fips_include_directories(libs)
fips_ide_group("src")
fips_add_subdirectory(src)
fips_ide_group("res")
fips_add_subdirectory(res)
fips_include_directories(res)

fips_finish()


