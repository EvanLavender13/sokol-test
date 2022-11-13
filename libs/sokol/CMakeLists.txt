# the sokol implementations library 
fips_begin_lib(sokol)
    fips_vs_warning_level(4)
    add_definitions(-DSOKOL_GLCORE33)
    set(slang "glsl330")    
    fips_files(sokol.c)
    fips_libs(X11 Xi Xcursor GL m dl asound)
fips_end_lib()