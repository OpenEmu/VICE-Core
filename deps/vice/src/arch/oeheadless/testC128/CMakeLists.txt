add_executable(testc64)
target_sources(testc64 PUBLIC
        main.m
        )
target_include_directories(testc64 PRIVATE
        ../public
        ../
        ../../..
        )
target_link_libraries(testc64 oex64sc)

target_compile_definitions(testc64 PRIVATE -DSOURCE_ROOT="${CMAKE_SOURCE_DIR}")
