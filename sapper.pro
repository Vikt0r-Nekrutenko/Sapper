TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cells.cpp \
        chunks.cpp \
        endview.cpp \
        gamemodel.cpp \
        gameview.cpp \
        main.cpp \
        menuview.cpp \
        storyview.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../stf/release/ -lstf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../stf/debug/ -lstf

INCLUDEPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \

DEPENDPATH += $$PWD/../stf \
                $$PWD/../stf/smv \
                $$PWD/../stf/sdb \

HEADERS += \
    cells.hpp \
    chunks.hpp \
    endview.hpp \
    gamemodel.hpp \
    gameview.hpp \
    menuview.hpp \
    storyview.hpp

DISTFILES += \
    sprs/pause_menu.spr
    sprs/logo.bmp
