configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/installer.nsis.in"
    "${CMAKE_CURRENT_BINARY_DIR}/installer.nsis.tmp"
)

file(GENERATE
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/installer.nsis"
    INPUT "${CMAKE_CURRENT_BINARY_DIR}/installer.nsis.tmp"
)

add_custom_target(installer.nsis.in SOURCES)
