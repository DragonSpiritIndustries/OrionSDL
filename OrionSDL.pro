# This file is part of Orion.
#
# Orion is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Orion is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Orion.  If not, see <http:#www.gnu.org/licenses/>

TEMPLATE = app
CONFIG += thread
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11
win32:QMAKE_LFLAGS += -mwindows

DEFINES += \
    ATHENA_NO_ZQUEST \
    ATHENA_NO_SAVES \
    ATHENA_ENABLE_PHYSFS

win32:DEFINES += \
    SDL_MAIN_NEEDED

include(Athena/Athena.pri)

win32:LIBS += \
    -lmingw32

LIBS += \
    -lSDL2 \
    -lSDL2_ttf \
    -lSDL2_image \
    -lSDL2_gfx \
    -lz \
    -lphysfs \
    -ltinyxml \

unix:LIBS += \
    -lGL \
    -lGLU
win32:LIBS += \
    -lSDL2main

INCLUDEPATH += \
    include

SOURCES += \
    src/SDLWindow.cpp \
    src/SDLRenderer.cpp \
    src/SDLApplication.cpp \
    src/SDLKeyboardManager.cpp \
    src/SDLJoystickManager.cpp \
    src/SDLMouseManager.cpp \
    src/ByteCodeStream.cpp \
    src/SDLTextureResource.cpp \
    src/SDLFontResource.cpp

HEADERS += \
    include/SDLWindow.hpp \
    include/SDLRenderer.hpp \
    include/SDLApplication.hpp \
    include/SDLKeyboardManager.hpp \
    include/SDLJoystickManager.hpp \
    include/SDLMouseManager.hpp \
    include/SDLTextureResource.hpp \
    include/SDLFontResource.hpp


