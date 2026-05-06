file(GLOB cpps_subset   "*.cpp")

list(APPEND cpp_paths ${cpps_subset})

foreach(cpp_path ${cpp_paths})

    get_filename_component(name ${cpp_path} NAME_WLE)
    set(name cpp_${name})
    add_executable(${name} ${cpp_path})
    set_property(TARGET ${name} PROPERTY LANGUAGES    CXX)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 26)
    target_compile_options(${name}
        PRIVATE
            -O0
            -g
            -Wall
            -Wextra
            # -freflection # Not working in clangd yet
            -fno-exceptions
            -fdiagnostics-color=always
    )

endforeach()
