QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets gui

QMAKE_CXXFLAGS=-finline-functions

CONFIG += c++17 c11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON = Resources/photosurface.icns

SOURCES += \
    View_module/recorder.cc \
    View_module/texturing.cc \
    main.cc \
    Model_module/model_obj_model.cc \
    Controller_module/viewer_controller.cc \
    View_module/mainwindow.cc \
    View_module/ptransform.cc \
    View_module/viewsetup.cc \
    View_module/screencap.cc \
    View_module/lighting.cc \
    View_module/oglview.cc \
    View_module/object3d.cc \
    giflib/dgif_lib.c \
    giflib/egif_lib.c \
    giflib/gif_err.c \
    giflib/gif_hash.c \
    giflib/gifalloc.c \
    giflib/quantize.c \
    gifimage/qgifimage.cpp

HEADERS += \
    Model_module/model_obj_model.h \
    Controller_module/viewer_controller.h \
    View_module/global_settings.h \
    View_module/panelfuncs.h \
    View_module/lighting.h \
    View_module/mainwindow.h \
    View_module/ptransform.h \
    View_module/recorder.h \
    View_module/texturing.h \
    View_module/viewsetup.h \
    View_module/screencap.h \
    View_module/enum_parameters.h \
    View_module/oglview.h \
    View_module/object3d.h \
    gifimage/qgifglobal.h \
    gifimage/qgifimage.h \
    gifimage/qgifimage_p.h

FORMS += \
    View_module/lighting.ui \
    View_module/mainwindow.ui \
    View_module/ptransform.ui \
    View_module/texturing.ui \
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

DISTFILES +=
