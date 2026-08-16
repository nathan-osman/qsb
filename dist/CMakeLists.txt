configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/installer.nsi.in"
    "${CMAKE_CURRENT_BINARY_DIR}/installer.nsi.tmp"
)

file(GENERATE
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/installer.nsi"
    INPUT "${CMAKE_CURRENT_BINARY_DIR}/installer.nsi.tmp"
)

add_custom_target(installer.nsi.in SOURCES)
