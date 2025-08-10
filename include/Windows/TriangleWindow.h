#ifndef TRIANGLEWINDOW_H
#define TRIANGLEWINDOW_H

#include "BaseOpenGLWindow.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QWindow>
#include <QtMath>

static const char *vertexShaderSource = "attribute highp vec4 posAttr;\n"
                                        "attribute lowp vec4 colAttr;\n"
                                        "varying lowp vec4 col;\n"
                                        "uniform highp mat4 matrix;\n"
                                        "void main() {\n"
                                        "   col = colAttr;\n"
                                        "   gl_Position = matrix * posAttr;\n"
                                        "}\n";

static const char *fragmentShaderSource = "varying lowp vec4 col;\n"
                                          "void main() {\n"
                                          "   gl_FragColor = col;\n"
                                          "}\n";

class TriangleWindow : public BaseOpenGLWindow
{
public:
    using BaseOpenGLWindow::BaseOpenGLWindow;
    ~TriangleWindow();

    void initialize() override;
    void render() override;

private:
    GLint m_matrixUniform = 0;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};

#endif // TRIANGLEWINDOW_H
