#include "RectangleRenderer.h"
#include <QOpenGLFramebufferObjectFormat>

RectangleRenderer::RectangleRenderer()
{
    qDebug() << "RectangleRenderer constructor called";
    initializeOpenGLFunctions(); // Initialize OpenGL function pointers
}

void RectangleRenderer::render()
{
    qDebug() << "Rendering in RectangleRenderer with size:" << m_window.width() << "x" << m_window.height();
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (!context)
    {
        qDebug() << "No OpenGL context available in render";
        return;
    }
    // Ensure context is current
    glClearColor(1.1f, 1.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_window.render();
}

QOpenGLFramebufferObject *RectangleRenderer::createFramebufferObject(const QSize &size)
{
    qDebug() << "Creating framebuffer with size:" << size;
    QSize validSize = size.isValid() ? size : QSize(800, 800);
    m_window.setSize(validSize);
    QOpenGLFramebufferObjectFormat format;
    format.setSamples(16);
    QOpenGLFramebufferObject *fbo = new QOpenGLFramebufferObject(validSize, format);
    // Initialize after FBO creation to ensure context
    qDebug() << "Start initializing RectangleWindow...";
    m_window.initialize();
    qDebug() << "Finished initializing RectangleWindow";
    return fbo;
}

void RectangleRenderer::synchronize(QQuickFramebufferObject *item)
{
    qDebug() << "Synchronizing with item:" << item;
    RectangleItem *windowItem = qobject_cast<RectangleItem *>(item);
    if (windowItem)
    {
        qDebug() << "Syncing properties: position=" << windowItem->objectPosition() << "rotate=" << windowItem->rotate() << "scale=" << windowItem->scale() << "useTexture=" << windowItem->useTexture();
        m_window.setObjectPosition(windowItem->objectPosition());
        m_window.setRotate(windowItem->rotate());
        m_window.setScale(windowItem->scale());
        m_window.setUseTexture(windowItem->useTexture());
    }
    else
    {
        qDebug() << "Failed to cast item to RectangleItem";
    }
}
