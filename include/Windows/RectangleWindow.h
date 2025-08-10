#ifndef RECTANGLEWINDOW_H
#define RECTANGLEWINDOW_H

#include "BaseOpenGLWindow.h"

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QScreen>
#include <QWindow>
#include <QtMath>

class RectangleWindow : public BaseOpenGLWindow
{
    Q_OBJECT

public:
    using BaseOpenGLWindow::BaseOpenGLWindow;
    ~RectangleWindow();

    void initialize() override;
    void render() override;

    void createRectangleBuffer(std::vector<GLfloat> *buffer);
    void createRectangleBuffer_multithreaded(std::vector<GLfloat> *buffer);
    void setAttributs();
    void bindAttributs();
    void initializeShaders();
    void loadTexture();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyHandler();
    void renderScene();
    void renderMainScene();
    void renderZoomScene();

    void printBuffer(std::vector<GLfloat> buffer);

signals:
    void objectPositionChanged();
    void rotateChanged();
    void scaleChanged();
    void useTextureChanged();

private:
    GLint m_matrixUniform = 0;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLTexture *m_texture = nullptr;

    QSet<int> m_keysPressed;
    QVector3D m_objectPosition;

    float m_moveSpeed = 0.05f;
    float m_scale = 1.0f;
    float m_rotate = 0.0f;
    float m_scaleSpeed = 0.05f;
    int m_frame = 0;
    bool isReset = true;
    bool m_useTexture = false;

    const int minimumSpacesColorsVertex = 3;
    const int minimumSpacesPointsVertex = 2;
    const int minimumSpacesTextureVertex = 2;
    const int allVertexs = minimumSpacesPointsVertex + minimumSpacesColorsVertex + minimumSpacesTextureVertex; // x and y -> (x,y) + RGB values + 2D Texture
    const int pointsCountForOneTriangle = 3;
    const int triangleCounts = 2;
    const int vertexCountForOneRectangle = allVertexs * pointsCountForOneTriangle * triangleCounts;
    const int wholeRectangle = 200000;
    const int wholeVertexesCount = vertexCountForOneRectangle * wholeRectangle; // Total number of vertices you want to store
    const size_t allocatedSize = wholeVertexesCount * sizeof(GLfloat);

    const GLfloat recWidth = 2.0;
    const GLfloat recHeight = 2.0;
    const GLfloat xSpacing = 0.0f;
    const GLfloat ySpacing = 0.0f;
    const GLfloat xOffset = +0; //-(wholeRectangle * recWidth) / 2;
    const GLfloat yOffset = +0; //-recHeight / 2;
};

#endif // RECTANGLEWINDOW_H
