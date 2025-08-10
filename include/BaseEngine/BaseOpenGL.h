#ifndef BASEOPENGL_H
#define BASEOPENGL_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QSize>
#include <QVector3D>
#include <vector>

class BaseOpenGL : public QOpenGLFunctions
{
public:
    explicit BaseOpenGL();
    virtual ~BaseOpenGL();

    // متدهای مجازی خالص برای اطمینان از پیاده‌سازی در کلاس‌های مشتق‌شده
    virtual void initialize() = 0;
    virtual void render() = 0;

    // مدیریت اندازه پنجره
    void setSize(const QSize &size);
    int width() const;
    int height() const;

    // مدیریت موقعیت شیء (برای RectangleWindow)
    virtual QVector3D objectPosition() const;
    virtual void setObjectPosition(const QVector3D &position);

protected:
    // متغیرهای مشترک
    QOpenGLBuffer m_vbo;                       // بافر OpenGL
    QOpenGLShaderProgram *m_program = nullptr; // برنامه شیدر
    QOpenGLTexture *m_texture = nullptr;       // تکسچر
    QSize m_size = QSize(800, 800);            // اندازه پیش‌فرض پنجره
    QVector3D m_objectPosition;                // موقعیت شیء
};

#endif // BASEOPENGL_H
