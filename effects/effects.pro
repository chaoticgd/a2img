TEMPLATE = app
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES +=

DISTFILES += \
    a2img/boxBlur/manifest0.json \
    a2img/boxBlur/horizontalPass.glsl \
    a2img/boxBlur/vertex.glsl \
    a2img/boxBlur/verticalPass.glsl \
    a2img/boxBlurSingle/manifest0.json \
    a2img/boxBlurSingle/fragment.glsl \
    a2img/boxBlurSingle/vertex.glsl \
    a2img/brightnessContrast/manifest0.json \
    a2img/brightnessContrast/fragment.glsl \
    a2img/brightnessContrast/vertex.glsl \
    a2img/gaussianBlur/manifest0.json \
    a2img/gaussianBlur/horizontalPass.glsl \
    a2img/gaussianBlur/vertex.glsl \
    a2img/gaussianBlur/verticalPass.glsl \
    a2img/hueSaturation/manifest0.json \
    a2img/hueSaturation/fragment.glsl \
    a2img/hueSaturation/vertex.glsl \
    a2img/mandelbrotSet/manifest0.json \
    a2img/mandelbrotSet/colour_basic.glsl \
    a2img/mandelbrotSet/fractal.glsl \
    a2img/mandelbrotSet/vertex.glsl \
    a2img/motionBlur/manifest0.json \
    a2img/motionBlur/fragment.glsl \
    a2img/motionBlur/vertex.glsl \
    a2img/motionBlur/manifest0.json \
    a2img/motionBlur/fragment.glsl \
    a2img/motionBlur/vertex.glsl \
    a2img/motionBlur/manifest0.json \
    a2img/motionBlur/fragment.glsl \
    a2img/motionBlur/vertex.glsl \
    a2img/pixelate/manifest0.json \
    a2img/pixelate/fragment.glsl \
    a2img/pixelate/vertex.glsl \
    a2img/radialBlur/manifest0.json \
    a2img/radialBlur/fragment.glsl \
    a2img/radialBlur/vertex.glsl \
    a2img/distort/manifest0.json \
    a2img/distort/fragment.glsl \
    a2img/vertex.glsl \
    a2img/sinusoidalDistortion/manifest0.json \
    a2img/tileDistortion/manifest0.json \
    a2img/tileDistortion/fragment.glsl
