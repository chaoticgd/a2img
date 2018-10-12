#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T14:14:26
#
#-------------------------------------------------

QT       += core gui opengl testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = a2img
TEMPLATE = app
CONFIG += c++14
#INCLUDEPATH += /run/media/chaoticgd/usb/include
#LIBS += -L"/run/media/chaoticgd/usb/lib"
#LIBS += -larchive


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        mainwindow.cpp \
    application.cpp \
    canvas.cpp \
    layer.cpp \
    tools/viewtool.cpp \
    minimap.cpp \
    tools/brushtool.cpp \
    commands/brushcommand.cpp \
    colour/colourmanager.cpp \
    colour/rgbcolourpicker.cpp \
    sliderbox.cpp \
    colour/colourslider.cpp \
    colour/hsvcolourpicker.cpp \
    colour/colourpalette.cpp \
    tools/filltool.cpp \
    commands/globalfillcommand.cpp \
    commands/floodfillcommand.cpp \
    test/floodfilltest.cpp \
    effect.cpp \
    effectproperty.cpp \
    commands/texturestoragecommand.cpp \
    effectmanager.cpp \
    commands/effectcommand.cpp \
    tools/selectiontool.cpp \
    maskmanager.cpp \
    texture.cpp \
    glfunctions.cpp \
    versionnumber.cpp \
    layermodel.cpp \
    commands/newlayercommand.cpp \
    commands/deletelayercommand.cpp \
    commands/movelayercommand.cpp \
    glbuffer.cpp \
    commands/layercommand.cpp \
    util/type.cpp \
    canvasview.cpp \
    util/serialisation.cpp \
    document/document.cpp \
    document/documentmanager.cpp \
    document/documentwriter.cpp \
    document/documentreader.cpp \
    thememanager.cpp \
    settingsmanager.cpp \
    tools/tool.cpp \
    commands/fillcommand.cpp \
    dialogs/imagepreviewdialog.cpp \
    dialogs/errorreportdialog.cpp \
    dialogs/settingsdialog.cpp \
    tools/texttool.cpp \
    tools/selectionboxtool.cpp \
    commands/textcommand.cpp \
    dialogs/newimagedialog.cpp \
    dialogs/aboutdialog.cpp \
    tools/lassotool.cpp \
    commands/imageresizecommand.cpp \
    dialogs/imageresizedialog.cpp

HEADERS += \
        mainwindow.h \
    application.h \
    canvas.h \
    layer.h \
    tools/tool.h \
    tools/viewtool.h \
    minimap.h \
    tools/brushtool.h \
    commands/brushcommand.h \
    colour/colourmanager.h \
    colour/rgbcolourpicker.h \
    colour/colourpicker.h \
    sliderbox.h \
    colour/colourslider.h \
    colour/hsvcolourpicker.h \
    colour/colourpalette.h \
    tools/filltool.h \
    commands/globalfillcommand.h \
    commands/floodfillcommand.h \
    test/floodfilltest.h \
    effect.h \
    effectproperty.h \
    commands/texturestoragecommand.h \
    effectmanager.h \
    vectorutil.h \
    commands/effectcommand.h \
    tools/selectiontool.h \
    maskmanager.h \
    texture.h \
    glfunctions.h \
    versionnumber.h \
    treenode.h \
    layermodel.h \
    commands/newlayercommand.h \
    commands/deletelayercommand.h \
    commands/movelayercommand.h \
    glbuffer.h \
    commands/layercommand.h \
    commands/serialisablecommand.h \
    util/type.h \
    canvasview.h \
    util/serialisation.h \
    document/documentwriter.h \
    document/documentmanager.h \
    document/document.h \
    document/documentreader.h \
    thememanager.h \
    settingsmanager.h \
    commands/fillcommand.h \
    dialogs/imagepreviewdialog.h \
    dialogs/errorreportdialog.h \
    dialogs/settingsdialog.h \
    tools/texttool.h \
    tools/selectionboxtool.h \
    commands/textcommand.h \
    dialogs/newimagedialog.h \
    dialogs/aboutdialog.h \
    util/vec2.h \
    util/rectangle.h \
    buildconfig.h \
    tools/lassotool.h \
    commands/imageresizecommand.h \
    dialogs/imageresizedialog.h

FORMS += \
        mainwindow.ui \
    errorreportdialog.ui \
    newimagedialog.ui \
    aboutdialog.ui \
    settingsdialog.ui \
    imageresizedialog.ui

DISTFILES += \
    css/stylesheet.css \
    effects_include/gaussian.glsl \
    effects_include/colour/hsv.glsl \
    effects_include/constants.glsl \
    effects_include/0.1/constants.glsl \
    effects_include/0.1/gaussian.glsl \
    effects_include/0.1/colour/hsv.glsl \
    shaders/brush_endpoint_fragment.glsl \
    shaders/brush_endpoint_vertex.glsl \
    shaders/brush_segment_fragment.glsl \
    shaders/brush_segment_vertex.glsl \
    shaders/colour_mask_fragment.glsl \
    shaders/colour_mask_vertex.glsl \
    shaders/colour_mask_multiply_fragment.glsl \
    shaders/global_fill_fragment.glsl \
    shaders/global_fill_vertex.glsl \
    shaders/texture_fragment.glsl \
    shaders/texture_vertex.glsl \
    effects_include/0.1/complex.glsl \
    shaders/mask_texture_fragment.glsl \
    shaders/mask_texture_fragment.glsl \
    shaders/chequerboard_fragment.glsl \
    shaders/chequerboard_vertex.glsl \
    shaders/selection_fragment.glsl \
    shaders/mask_texture_vertex.glsl \
    css/stylesheetDark.css

RESOURCES += \
    data.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/release/ -larchive
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/debug/ -larchive
else:unix: LIBS += -L$$PWD/../../lib/ -larchive

INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
