#ifndef RECTANGLECOMPONENT_H
#define RECTANGLECOMPONENT_H

#include "BaseOpenGL.h"

#include <QKeyEvent>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QScreen>
#include <QWindow>
#include <QtMath>

class RectangleComponent : public QObject, public BaseOpenGL
{
    Q_OBJECT
    Q_PROPERTY(QVector3D objectPosition READ objectPosition WRITE setObjectPosition NOTIFY objectPositionChanged)
    Q_PROPERTY(float rotate READ rotate WRITE setRotate NOTIFY rotateChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(bool useTexture READ useTexture WRITE setUseTexture NOTIFY useTextureChanged)

public:
    explicit RectangleComponent(QObject *parent = nullptr);
    ~RectangleComponent();

    void initialize() override;
    void render() override;

    QVector3D objectPosition() const;
    void setObjectPosition(const QVector3D &position);

    float rotate() const;
    void setRotate(float angle);

    float scale() const;
    void setScale(float scale);

    bool useTexture() const;
    void setUseTexture(bool use);

    void createRectangleBuffer(std::vector<GLfloat> *buffer);
    void createRectangleBuffer_multithreaded(std::vector<GLfloat> *buffer);
    void setAttributs();
    void bindAttributs();
    void initializeShaders();
    void loadTexture();
    // void keyPressEvent(QKeyEvent *event) override;
    // void keyReleaseEvent(QKeyEvent *event) override;
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
    QSize m_size = QSize(800, 800);
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

#endif // RECTANGLECOMPONENT_H
