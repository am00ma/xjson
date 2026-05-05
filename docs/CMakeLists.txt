file(GLOB docs_subset   "*.c")

list(APPEND doc_paths ${docs_subset})

foreach(doc_path ${doc_paths})

    get_filename_component(name ${doc_path} NAME_WLE)
    add_executable(${name} ${doc_path})
    target_include_directories(${name} PUBLIC ../include)
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

endforeach()
