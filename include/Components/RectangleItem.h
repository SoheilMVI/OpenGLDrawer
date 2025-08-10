#ifndef RECTANGLEITEM_H
#define RECTANGLEITEM_H

#include "RectangleWindow.h"
#include <QQuickFramebufferObject>

class RectangleItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D objectPosition READ objectPosition WRITE setObjectPosition NOTIFY objectPositionChanged)
    Q_PROPERTY(float rotate READ rotate WRITE setRotate NOTIFY rotateChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(bool useTexture READ useTexture WRITE setUseTexture NOTIFY useTextureChanged)

public:
    RectangleItem();

    Renderer *createRenderer() const override;

    QVector3D objectPosition() const;

    void setObjectPosition(const QVector3D &position);

    float rotate() const;
    void setRotate(float angle);
    float scale() const;
    void setScale(float scale);

    bool useTexture() const;
    void setUseTexture(bool use);

signals:
    void objectPositionChanged();
    void rotateChanged();
    void scaleChanged();
    void useTextureChanged();

public slots:
    void setWindowSize();

private:
    QVector3D m_objectPosition = QVector3D(0, 0, 0);
    float m_rotate = 0.0f;
    float m_scale = 1.0f;
    bool m_useTexture = false;
};

#endif // RECTANGLEITEM_H
