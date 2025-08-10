
#include "RectangleWindow.h"

#include "QOpenGLPaintDevice"
#include "QPainter"
#include <QFuture>
#include <QFutureWatcher>
#include <QRandomGenerator>
#include <QtConcurrent>

static const char *vertexShaderSource = "attribute highp vec4 pointsAttr;\n"
                                        "attribute lowp vec4 colorAttr;\n"
                                        "attribute highp vec2 textureCoordAttr;\n"
                                        "varying lowp vec4 vertexShaderColor;\n"
                                        "varying highp vec2 vertexShaderTextureCoord;\n"
                                        "uniform highp mat4 matrix;\n"
                                        "void main() {\n"
                                        "   vertexShaderColor = colorAttr;\n"
                                        "   vertexShaderTextureCoord = textureCoordAttr;\n"
                                        "   gl_Position = matrix * pointsAttr;\n"
                                        "}\n";

static const char *fragmentShaderSource = "varying lowp vec4 vertexShaderColor;\n"
                                          "varying highp vec2 vertexShaderTextureCoord;\n"
                                          "uniform sampler2D textureSampler;\n"
                                          "void main() {\n"
                                          "   //gl_FragColor = texture2D(textureSampler, vertexShaderTextureCoord);\n"
                                          "   //gl_FragColor = vertexShaderColor;\n"
                                          "   vec4 texColor = texture2D(textureSampler, vertexShaderTextureCoord);\n"
                                          "   gl_FragColor = mix(texColor, vertexShaderColor, 0.5);\n"
                                          "}\n";

//***************************************************************************************************************

RectangleWindow::~RectangleWindow()
{
    delete m_program;
    delete m_texture;
    m_vbo.destroy();
}

//***************************************************************************************************************

void RectangleWindow::initialize()
{
    //    static const GLfloat vertexs[]{
    //                                    +0.5f, +0.5f,
    //                                    +0.5f, -0.5f,
    //                                    -0.5f, -0.5f,
    //                                    -0.5f, +0.5f,
    //                                    +0.5f, +0.5f,
    //                                    -0.5f, -0.5f
    //                                };

    qDebug() << __FUNCTION__;
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (!context)
    {
        qDebug() << "No OpenGL context is available!";
    }
    else
    {
        qDebug() << "OpenGL Version:" << reinterpret_cast<const char *>(glGetString(GL_VERSION));
        qDebug() << "GLSL Version:" << reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        qDebug() << "Vendor:" << reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        qDebug() << "Renderer:" << reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    }

    std::vector<GLfloat> vertexs(wholeVertexesCount);

    // createRectangleBuffer(&vertexs);
    createRectangleBuffer_multithreaded(&vertexs);

    // printBuffer(vertexs);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertexs.data(), allocatedSize);

    m_program = new QOpenGLShaderProgram(this);

    initializeShaders();

    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

    if (!m_program->link())
    {
        qDebug() << "Shader Program Link Error:" << m_program->log();
    }

    loadTexture();
}

//***************************************************************************************************************

void RectangleWindow::createRectangleBuffer(std::vector<GLfloat> *buffer)
{
    /*
    Points  Colors      Texture
    0  1    2  3  4     5  6
    7  8    9  10 11    12 13
    14 15   16 17 18    19 20
    21 22   23 24 25    26 27
    28 29   30 31 32    33 34
    35 36   37 38 39    40 41
    */
    qDebug() << "Creating rectangle buffer...";
    for (int recIndex = 0; recIndex < wholeRectangle; recIndex++)
    {
        const int indexAdder = ((recIndex)*vertexCountForOneRectangle);
        const int rowNum = recIndex / 400;
        // if (rowNum != 0)
        // qDebug() << "rowNum -> " << rowNum << "| recIndex -> " << recIndex << "| width() -> " << width();

        const GLfloat startX = (recIndex * (recWidth + xSpacing)) - (xSpacing * wholeRectangle) / 2 - ((rowNum * 400 * recWidth) /** (recWidth + xSpacing)*/);
        // qDebug() << startX << startX + xOffset;
        const GLfloat startY = (rowNum * recHeight) + (ySpacing * recIndex);

        QRandomGenerator *generator = QRandomGenerator::global();
        const GLfloat r = generator->bounded(255) / 255.0f;
        const GLfloat g = generator->bounded(255) / 255.0f;
        const GLfloat b = generator->bounded(255) / 255.0f;

        { // left bottom
            // Box corner
            buffer->at(0 + indexAdder) = startX + xOffset;
            buffer->at(1 + indexAdder) = startY + yOffset;
            // texture
            buffer->at(5 + indexAdder) = 0.0f;
            buffer->at(6 + indexAdder) = 0.0f;
        }
        { // left top
            // Box corner
            buffer->at(7 + indexAdder) = startX + xOffset;
            buffer->at(8 + indexAdder) = startY + yOffset + recHeight;
            // texture
            buffer->at(12 + indexAdder) = 0.0f;
            buffer->at(13 + indexAdder) = 1.0f;
        }
        { // right top
            // Box corner
            buffer->at(14 + indexAdder) = startX + xOffset + recWidth;
            buffer->at(15 + indexAdder) = startY + yOffset + recHeight;
            // texture
            buffer->at(19 + indexAdder) = 1.0f;
            buffer->at(20 + indexAdder) = 1.0f;
        }
        { // left bottom
            // Box corner
            buffer->at(21 + indexAdder) = startX + xOffset;
            buffer->at(22 + indexAdder) = startY + yOffset;
            // texture
            buffer->at(26 + indexAdder) = 0.0f;
            buffer->at(27 + indexAdder) = 0.0f;
        }
        { // right bottom
            // Box corner
            buffer->at(28 + indexAdder) = startX + xOffset + recWidth;
            buffer->at(29 + indexAdder) = startY + yOffset;
            // texture
            buffer->at(33 + indexAdder) = 1.0f;
            buffer->at(34 + indexAdder) = 0.0f;
        }
        { // right top
            // Box corner
            buffer->at(35 + indexAdder) = startX + xOffset + recWidth;
            buffer->at(36 + indexAdder) = startY + yOffset + recHeight;
            // texture
            buffer->at(40 + indexAdder) = 1.0f;
            buffer->at(41 + indexAdder) = 1.0f;
        }

        for (int j = 0; j < triangleCounts * pointsCountForOneTriangle; j++)
        {
            const int colorIndexOfsset = (minimumSpacesColorsVertex * j) + (minimumSpacesPointsVertex * j) + (minimumSpacesTextureVertex * j) + indexAdder;
            buffer->at(2 + colorIndexOfsset) = r;
            buffer->at(3 + colorIndexOfsset) = g;
            buffer->at(4 + colorIndexOfsset) = b;
        }
    }
}

void RectangleWindow::createRectangleBuffer_multithreaded(std::vector<GLfloat> *buffer)
{
    qDebug() << "Creating rectangle buffer...";

    // تابع کمکی برای پردازش یک محدوده از مستطیل‌ها
    auto processRectangleRange = [=](int startIndex, int endIndex)
    {
        for (int recIndex = startIndex; recIndex < endIndex; recIndex++)
        {
            const int indexAdder = ((recIndex)*vertexCountForOneRectangle);
            const int rowNum = recIndex / 400;

            const GLfloat startX = (recIndex * (recWidth + xSpacing)) - (xSpacing * wholeRectangle) / 2 - ((rowNum * 400 * recWidth));
            const GLfloat startY = (rowNum * recHeight) + (ySpacing * recIndex);

            QRandomGenerator *generator = QRandomGenerator::global();
            const GLfloat r = generator->bounded(255) / 255.0f;
            const GLfloat g = generator->bounded(255) / 255.0f;
            const GLfloat b = generator->bounded(255) / 255.0f;

            { // left bottom
                buffer->at(0 + indexAdder) = startX + xOffset;
                buffer->at(1 + indexAdder) = startY + yOffset;
                buffer->at(5 + indexAdder) = 0.0f;
                buffer->at(6 + indexAdder) = 0.0f;
            }
            { // left top
                buffer->at(7 + indexAdder) = startX + xOffset;
                buffer->at(8 + indexAdder) = startY + yOffset + recHeight;
                buffer->at(12 + indexAdder) = 0.0f;
                buffer->at(13 + indexAdder) = 1.0f;
            }
            { // right top
                buffer->at(14 + indexAdder) = startX + xOffset + recWidth;
                buffer->at(15 + indexAdder) = startY + yOffset + recHeight;
                buffer->at(19 + indexAdder) = 1.0f;
                buffer->at(20 + indexAdder) = 1.0f;
            }
            { // left bottom
                buffer->at(21 + indexAdder) = startX + xOffset;
                buffer->at(22 + indexAdder) = startY + yOffset;
                buffer->at(26 + indexAdder) = 0.0f;
                buffer->at(27 + indexAdder) = 0.0f;
            }
            { // right bottom
                buffer->at(28 + indexAdder) = startX + xOffset + recWidth;
                buffer->at(29 + indexAdder) = startY + yOffset;
                buffer->at(33 + indexAdder) = 1.0f;
                buffer->at(34 + indexAdder) = 0.0f;
            }
            { // right top
                buffer->at(35 + indexAdder) = startX + xOffset + recWidth;
                buffer->at(36 + indexAdder) = startY + yOffset + recHeight;
                buffer->at(40 + indexAdder) = 1.0f;
                buffer->at(41 + indexAdder) = 1.0f;
            }

            for (int j = 0; j < triangleCounts * pointsCountForOneTriangle; j++)
            {
                const int colorIndexOffset = (minimumSpacesColorsVertex * j) + (minimumSpacesPointsVertex * j) + (minimumSpacesTextureVertex * j) + indexAdder;
                buffer->at(2 + colorIndexOffset) = r;
                buffer->at(3 + colorIndexOffset) = g;
                buffer->at(4 + colorIndexOffset) = b;
            }
        }
    };

    // تعداد نخ‌ها (می‌توانید بر اساس تعداد هسته‌های CPU تنظیم کنید)
    const int threadCount = QThread::idealThreadCount();
    const int rectanglesPerThread = wholeRectangle / threadCount;
    QVector<QFuture<void>> futures;

    // تقسیم کار بین نخ‌ها
    for (int i = 0; i < threadCount; ++i)
    {
        int startIndex = i * rectanglesPerThread;
        int endIndex = (i == threadCount - 1) ? wholeRectangle : startIndex + rectanglesPerThread;
        futures.append(QtConcurrent::run([=]() { processRectangleRange(startIndex, endIndex); }));
    }

    // منتظر اتمام تمام نخ‌ها
    for (auto &future : futures)
    {
        future.waitForFinished();
    }

    qDebug() << "Rectangle buffer creation completed.";
}

//***************************************************************************************************************

void RectangleWindow::setAttributs()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, minimumSpacesPointsVertex, GL_FLOAT, GL_FALSE /*GL_TRUE*/, allVertexs * sizeof(GL_FLOAT), reinterpret_cast<void *>(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, minimumSpacesColorsVertex, GL_FLOAT, GL_FALSE /*GL_TRUE*/, allVertexs * sizeof(GL_FLOAT), reinterpret_cast<void *>(minimumSpacesPointsVertex * sizeof(GLfloat)));
    glVertexAttribPointer(2, minimumSpacesTextureVertex, GL_FLOAT, GL_FALSE /*GL_TRUE*/, allVertexs * sizeof(GL_FLOAT), reinterpret_cast<void *>((minimumSpacesPointsVertex + minimumSpacesColorsVertex) * sizeof(GLfloat)));
}

//***************************************************************************************************************

void RectangleWindow::bindAttributs()
{
    m_program->bindAttributeLocation("pointsAttr", 0);
    m_program->bindAttributeLocation("colorAttr", 1);
    m_program->bindAttributeLocation("textureCoordAttr", 2);
}

//***************************************************************************************************************

void RectangleWindow::initializeShaders()
{
    setAttributs();

    QString shaderDir = QString(PROJECT_SOURCE_DIR) + "/Shaders/v440";
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, shaderDir + "/VertexShader.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, shaderDir + "/FragmentShader.frag");

    // m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    // m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    bindAttributs();

    // m_program->link();
    // m_program->bind();
    if (!m_program->link())
    {
        qDebug() << "Shader Program Link Error:" << m_program->log();
        return;
    }
    if (!m_program->bind())
    {
        qDebug() << "Shader Program Bind Error:" << m_program->log();
        return;
    }
}

//***************************************************************************************************************

void RectangleWindow::loadTexture()
{
    QImage image("D:/1276/openglwindow/image.png");
    if (image.isNull())
        qDebug() << "Image not loaded";

    m_texture = new QOpenGLTexture(image.mirrored());

    if (!m_texture->isCreated())
    {
        qDebug() << "Texture creation failed";
        return;
    }

    // Set texture parameters
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

//***************************************************************************************************************

void RectangleWindow::keyPressEvent(QKeyEvent *event)
{
    m_keysPressed.insert(event->key()); // Add key to the set
}

// // //***************************************************************************************************************

void RectangleWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_keysPressed.remove(event->key()); // Remove key from the set
}

//***************************************************************************************************************

void RectangleWindow::keyHandler()
{
    if (m_keysPressed.contains(Qt::Key_W))
    {
        m_objectPosition.setY(m_objectPosition.y() + m_moveSpeed); // Move up
        isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_S))
    {
        m_objectPosition.setY(m_objectPosition.y() - m_moveSpeed); // Move down
        isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_A))
    {
        m_objectPosition.setX(m_objectPosition.x() - m_moveSpeed); // Move left
        isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_D))
    {
        m_objectPosition.setX(m_objectPosition.x() + m_moveSpeed); // Move right
        isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_6))
    {
        m_rotate += 0.5f; // Rotate right
        isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_4))
    {
        m_rotate -= 0.5f; // Rotate right
        isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_Up))
    {
        m_scale += m_moveSpeed; // Zoom in
        // isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_Down))
    {
        m_scale -= m_moveSpeed; // Zoom out
        if (m_scale < 0)
        {
            m_scale = 0;
        }
        // isReset = false;
    }
    if (m_keysPressed.contains(Qt::Key_Escape))
    {
        m_scale = 1.0f;
        m_rotate = 0.0f;
        m_objectPosition.setX(0.0f);
        m_objectPosition.setY(0.0f);
        isReset = true;
    }
    if (m_keysPressed.contains(Qt::Key_C))
    {
        m_useTexture = false;
    }
    if (m_keysPressed.contains(Qt::Key_C) && m_keysPressed.contains(Qt::Key_Shift))
    {
        m_useTexture = true;
    }

    // if (isReset)
    // {
    //     m_objectPosition.setX(std::cos(m_frame / screen()->refreshRate()) / 2.0f + 0.5f);
    //     m_objectPosition.setY(std::sin(m_frame / screen()->refreshRate()) / 2.0f + 0.5f);
    // }
}

//***************************************************************************************************************

void RectangleWindow::renderScene()
{
    // glEnable(GL_TEXTURE_2D);
    if (m_useTexture)
        m_texture->bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glDrawArrays(GL_TRIANGLES, 0, triangleCounts * pointsCountForOneTriangle * wholeVertexesCount);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    if (m_useTexture)
        m_texture->release();
    // glDisable(GL_TEXTURE_2D);
}

//***************************************************************************************************************

void RectangleWindow::renderMainScene()
{
    glViewport(0, 0, QWindow::width(), QWindow::height());
    glClearColor(1.0f, 0.01f, 0.01f, 0.0f); // Dark gray background for main view
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_program->bind();
    initialize();

    QMatrix4x4 matrix;
    matrix.translate(m_objectPosition);
    matrix.ortho(0.0f, QWindow::width(), QWindow::height(), 0.0f, -1.0f, 1.0f);
    matrix.rotate(m_rotate, 0.0f, 0.0f, 1.0f);
    matrix.scale(1.0f);
    m_program->setUniformValue(m_matrixUniform, matrix);
    renderScene();
    m_program->release();
}

//***************************************************************************************************************

void RectangleWindow::renderZoomScene()
{
    int zoomViewportWidth = QWindow::width() / 4;                  // 1/4th of the screen width
    int zoomViewportHeight = QWindow::height() / 4;                // 1/4th of the screen height
    int zoomViewportX = QWindow::width() - zoomViewportWidth - 10; // Bottom-right corner (with padding)
    int zoomViewportY = 10;                                        // Padding from bottom

    glViewport(zoomViewportX, zoomViewportY, zoomViewportWidth, zoomViewportHeight);
    m_program->bind();

    QMatrix4x4 zoomMatrix;
    zoomMatrix.scale(m_scale);              // Larger scale for zoom-in
    zoomMatrix.translate(m_objectPosition); // Use the same object position

    m_program->setUniformValue(m_matrixUniform, zoomMatrix);
    renderScene();
    m_program->release();
}

//***************************************************************************************************************

void RectangleWindow::render()
{
    keyHandler();
    // ++m_frame;

    qDebug() << "Rendering with size:" << QWindow::width() << "x" << QWindow::height();
    renderMainScene();
    renderZoomScene();

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        qDebug() << "OpenGL error during render:" << err;
    }
}

//***************************************************************************************************************

void RectangleWindow::printBuffer(std::vector<GLfloat> buffer)
{
    // qDebug() <<buffer;

    for (int i = 0; i < buffer.size() / allVertexs; i++)
    {
        QString tmp = "";
        for (int j = 0; j < allVertexs; j++)
        {
            const int index = j + (i * allVertexs);
            // qDebug () << "index: " << index << "value: " << buffer[index];
            tmp += (QString::number(buffer[index])) + "       ";
        }
        qDebug() << tmp;
    }

    // for(int j=0,i=0; i<buffer.size() && j<buffer.size(); j++ ,i++){
    // //        qDebug() << "buffer \t\t" << "Index " << i++ << buffer[i] << "Index " << i << buffer[i];
    //     qDebug() << "buffer \t" << QString("Index %1: %2").arg(j).arg(buffer[j++]) << "\t" <<  QString("Index %1: %2").arg(j).arg(vertexs[j]);
    // }
}

//***************************************************************************************************************
