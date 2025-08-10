// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "OpenGLWindow.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>

//! [1]
OpenGLWindow::OpenGLWindow(QWindow *parent) : QWindow(parent)
{
    // qDebug() << __FUNCTION__ ;
    setSurfaceType(QWindow::OpenGLSurface);
}
//! [1]

OpenGLWindow::~OpenGLWindow()
{
    // qDebug() << __FUNCTION__ ;
    delete m_device;
}
//! [2]
void OpenGLWindow::render(QPainter *painter)
{
    // qDebug() << __FUNCTION__ ;
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{
    // qDebug() << __FUNCTION__ ;
}

void OpenGLWindow::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}
//! [2]

//! [3]
void OpenGLWindow::renderLater()
{
    requestUpdate();
}

bool OpenGLWindow::event(QEvent *event)
{
    // qDebug() << __FUNCTION__ ;
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    // qDebug() << __FUNCTION__ ;
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}
//! [3]

//! [4]
void OpenGLWindow::renderNow()
{
    // qDebug() << __FUNCTION__ ;
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
//! [4]

//! [5]
void OpenGLWindow::setAnimating(bool animating)
{
    // qDebug() << __FUNCTION__ ;
    m_animating = animating;

    if (animating)
        renderLater();
}
//! [5]
