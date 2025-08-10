#ifndef RECTANGLERENDERER_H
#define RECTANGLERENDERER_H

#include "RectangleComponent.h"
#include "RectangleItem.h" // Add this include
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQuickFramebufferObject>

class RectangleRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    RectangleRenderer();

protected:
    void render() override;

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

    void synchronize(QQuickFramebufferObject *item) override;

    RectangleComponent m_window;
};

#endif // RECTANGLERENDERER_H
