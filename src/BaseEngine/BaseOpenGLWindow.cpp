#include "BaseOpenGLWindow.h"
#include <qopenglpaintdevice.h>

BaseOpenGLWindow::BaseOpenGLWindow(QWindow *parent) : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

BaseOpenGLWindow::~BaseOpenGLWindow()
{
    delete m_device;
}

void BaseOpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void BaseOpenGLWindow::renderLater()
{
    requestUpdate();
}

void BaseOpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context)
    {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize)
    {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

bool BaseOpenGLWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void BaseOpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}
