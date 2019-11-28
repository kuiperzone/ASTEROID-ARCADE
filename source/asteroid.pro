#-------------------------------------------------
# APPLICATION META
#-------------------------------------------------
TARGET_MAJOR=2
TARGET_MINOR=0
TARGET_PATCH=0
VERSION = $${TARGET_MAJOR}.$${TARGET_MINOR}.$${TARGET_PATCH}
DEFINES += "APP_VERSION=\"\\\"$${VERSION}\\\"\""

QMAKE_TARGET_PRODUCT = "ASTEROID ARCADE"
QMAKE_TARGET_COMPANY = "Andy Thomas"
QMAKE_TARGET_COPYRIGHT = "Andy Thomas 2019"
QMAKE_TARGET_DESCRIPTION = "Open Source Arcade Game"

#-------------------------------------------------
# CONFIGURATION
#-------------------------------------------------
TARGET = "ASTEROID"
TEMPLATE = app
QT *= core gui widgets multimedia
CONFIG *= c++11 stl exceptions_off

# RELEASE vs DEBUG
Debug:DEFINES *= DEBUG

# FIXES
# We want M_PI and other math defines.
# Best to define it here rather than in files.
DEFINES *= _USE_MATH_DEFINES

# MS headers interfere with std::min() and std::max()
DEFINES *= NOMINMAX

# PATHS
INCLUDEPATH +=

#-------------------------------------------------
# EXTERNAL LIBS
#-------------------------------------------------
# none

#-------------------------------------------------
# POST PROCESSING
#-------------------------------------------------
# none

#-------------------------------------------------
# SOURCE FILES & RESOURCES
#-------------------------------------------------

win32:RC_ICONS += res/app_icon.ico
RESOURCES += res/resources.qrc

HEADERS += \
    game/internal/big_rock.h \
    game/internal/bullet.h \
    game/internal/debris.h \
    game/internal/entity_kind.h \
    game/internal/exploder.h \
    game/internal/game_entity.h \
    game/internal/label.h \
    game/internal/medium_rock.h \
    game/internal/rotator.h \
    game/internal/scaled_canvas.h \
    game/internal/ship.h \
    game/internal/small_rock.h \
    game/internal/spark.h \
    game/internal/ufo.h \
    game/internal/universe.h \
    game/canvas_interface.h \
    game/key_id.h \
    game/pair_xy.h \
    game/player.h \
    game/sound_id.h \
    main/about_dialog.h \
    main/device_canvas.h \
    main/main_window.h

SOURCES += \
    game/internal/big_rock.cpp \
    game/internal/bullet.cpp \
    game/internal/debris.cpp \
    game/internal/exploder.cpp \
    game/internal/game_entity.cpp \
    game/internal/label.cpp \
    game/internal/medium_rock.cpp \
    game/internal/rotator.cpp \
    game/internal/scaled_canvas.cpp \
    game/internal/ship.cpp \
    game/internal/ufo.cpp \
    game/internal/universe.cpp \
    game/internal/small_rock.cpp \
    game/internal/spark.cpp \
    game/pair_xy.cpp \
    game/player.cpp \
    main/about_dialog.cpp \
    main/device_canvas.cpp \
    main/main.cpp \
    main/main_window.cpp

FORMS += \
    ui/main_window.ui \
    ui/about_dialog.ui

