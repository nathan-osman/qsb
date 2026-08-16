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

add_executable(QSoundBoard ${SRC})

target_link_libraries(QSoundBoard PRIVATE Qt6::Multimedia Qt6::Widgets)

install(TARGETS QSoundBoard RUNTIME DESTINATION bin)
