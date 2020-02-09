if(USE_OEHEADLESS)
    add_subdirectory(oeheadless)
else()
    add_subdirectory(shared)
    add_subdirectory(sdl)
endif()

