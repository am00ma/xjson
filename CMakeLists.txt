cmake_minimum_required(VERSION 3.15)

# =========================================================================
#                       Project
# =========================================================================
project(
    "xjson"
    VERSION 0.0.1
    LANGUAGES C)

# =========================================================================
#                      Binaries
# =========================================================================
set(name xjson)

add_executable(${name} main.c)

target_link_libraries(${name} PRIVATE stz)
set_property(TARGET ${name} PROPERTY LANGUAGES  C)
set_property(TARGET ${name} PROPERTY C_STANDARD 23)

target_compile_options(${name}
    PRIVATE
        -O0
        -g
        -Wall
        -Wextra
        -fno-exceptions
        -fdiagnostics-color=always
)

# =========================================================================
#                      Installation
# =========================================================================
include(GNUInstallDirs)

install(TARGETS ${name})
