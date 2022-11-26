QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets gui

CONFIG += c++17 c11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cc \
    Model_module/model_frame.cc \
    Controller_module/viewer_controller.cc \
    View_module/mainwindow.cc \
    View_module/ptransform.cc \
    View_module/viewsetup.cc \
    View_module/screencap.cc \
    View_module/oglview.cc \
    giflib/dgif_lib.c \
    giflib/egif_lib.c \
    giflib/gif_err.c \
    giflib/gif_hash.c \
    giflib/gifalloc.c \
    giflib/quantize.c \
    gifimage/qgifimage.cpp

HEADERS += \
    Model_module/model_frame.h \
    Controller_module/viewer_controller.h \
    View_module/mainwindow.h \
    View_module/ptransform.h \
    View_module/viewsetup.h \
    View_module/screencap.h \
    View_module/enum_parameters.h \
    View_module/oglview.h \
    gifimage/qgifglobal.h \
    gifimage/qgifimage.h \
    gifimage/qgifimage_p.h

FORMS += \
    View_module/mainwindow.ui \
    View_module/ptransform.ui \
    View_module/viewsetup.ui \
    View_module/screencap.ui \
    View_module/oglview.ui

TRANSLATIONS += \
    3DViewer_v2_1_en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc