add_executable(testc64)
target_sources(testc64 PUBLIC
        c64main.m
        )
target_include_directories(testc64 PRIVATE
        ../public
        ../
        ../../..
        )
target_link_libraries(testc64 oex64sc)

target_compile_definitions(testc64 PRIVATE -DSOURCE_ROOT="${CMAKE_SOURCE_DIR}")

add_executable(testvic20)
target_sources(testvic20 PUBLIC
        vic20main.m
        )
target_include_directories(testvic20 PRIVATE
        ../public
        ../
        ../../..
        )
target_link_libraries(testvic20 oexvic20)

target_compile_definitions(testvic20 PRIVATE -DSOURCE_ROOT="${CMAKE_SOURCE_DIR}")

add_executable(testc128)
target_sources(testc128 PUBLIC
        c128main.m
        )
target_include_directories(testc128 PRIVATE
        ../public
        ../
        ../../..
        )
target_link_libraries(testc128 oex128)

target_compile_definitions(testc128 PRIVATE -DSOURCE_ROOT="${CMAKE_SOURCE_DIR}")
