#
# project: sokol-test
#
project(sokol-test-project)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_STANDARD 99)

# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

fips_setup()
fips_ide_group("")
if (FIPS_EMSCRIPTEN)
    set(sokol_backend SOKOL_GLES3)
    set(slang "glsl300es:glsl100")
    add_link_options("SHELL:-s USE_WEBGL2=1")
else()
    set(sokol_backend SOKOL_GLCORE33)
    set(slang "glsl330")
endif()

fips_project(sokol-test-project C)
fips_ide_group("libs")
fips_add_subdirectory(libs)
fips_include_directories(libs)
fips_ide_group("src")
fips_add_subdirectory(src)
fips_ide_group("res")
fips_add_subdirectory(res)
fips_include_directories(res)

fips_finish()


