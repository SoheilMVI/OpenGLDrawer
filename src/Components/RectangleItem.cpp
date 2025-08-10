#include "RectangleItem.h"
#include "RectangleRenderer.h"
#include <QQuickFramebufferObject>
#include <QTimer>

RectangleItem::RectangleItem()
{
    setFlag(ItemHasContents, true);
    connect(this, &QQuickItem::widthChanged, this, &RectangleItem::update);
    connect(this, &QQuickItem::heightChanged, this, &RectangleItem::update);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RectangleItem::update);
    // timer->start(100); // ~60 FPS
}

QQuickFramebufferObject::Renderer *RectangleItem::createRenderer() const
{
    return new RectangleRenderer();
}

QVector3D RectangleItem::objectPosition() const
{
    return m_objectPosition;
}
void RectangleItem::setObjectPosition(const QVector3D &position)
{
    if (m_objectPosition != position)
    {
        m_objectPosition = position;
        emit objectPositionChanged();
        update();
    }
}

float RectangleItem::rotate() const
{
    return m_rotate;
}
void RectangleItem::setRotate(float angle)
{
    if (m_rotate != angle)
    {
        m_rotate = angle;
        emit rotateChanged();
        update();
    }
}

float RectangleItem::scale() const
{
    return m_scale;
}
void RectangleItem::setScale(float scale)
{
    if (m_scale != scale && scale >= 0)
    {
        m_scale = scale;
        emit scaleChanged();
        update();
    }
}

bool RectangleItem::useTexture() const
{
    return m_useTexture;
}
void RectangleItem::setUseTexture(bool use)
{
    if (m_useTexture != use)
    {
        m_useTexture = use;
        emit useTextureChanged();
        update();
    }
}

void RectangleItem::setWindowSize()
{
    setWidth(400);
    setHeight(400);
}
