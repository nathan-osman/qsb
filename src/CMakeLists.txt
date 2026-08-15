set(SRC
    main.cpp
    mainwindow.hpp
    mainwindow.cpp
)

add_executable(QSoundBoard ${SRC})

target_link_libraries(QSoundBoard PRIVATE Qt6::Widgets)

install(TARGETS QSoundBoard RUNTIME DESTINATION bin)
