# Determine where QT's bin/ directory is located
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

# Look for windeployqt in this path
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

set(SRC
    category.hpp
    category.cpp
    categorydialog.hpp
    categorydialog.cpp
    clip.hpp
    clip.cpp
    clipdialog.hpp
    clipdialog.cpp
    main.cpp
    mainwindow.hpp
    mainwindow.cpp
    manager.hpp
    manager.cpp
    pixmaps.hpp
    pixmaps.cpp
    player.hpp
    player.cpp
    resource.qrc
    util.hpp
    util.cpp
)

add_executable(QSoundBoard WIN32 ${SRC})

target_link_libraries(QSoundBoard PRIVATE Qt6::Multimedia Qt6::Widgets)

install(TARGETS QSoundBoard RUNTIME DESTINATION bin)

if(WIN32)
    target_sources(QSoundBoard PRIVATE resource.rc)

    add_custom_command(TARGET QSoundBoard POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                --verbose 0
                --no-compiler-runtime
                --no-opengl-sw
                \"$<TARGET_FILE:QSoundBoard>\"
        COMMENT "Deploying Qt..."
    )

    include(InstallRequiredSystemLibraries)
    foreach(lib ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
        get_filename_component(filename "${lib}" NAME)
        add_custom_command(TARGET QSoundBoard POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                copy_if_different "${lib}" \"$<TARGET_FILE_DIR:QSoundBoard>\"
            COMMENT "Copying ${filename}..."
        )
    endforeach()
endif()
