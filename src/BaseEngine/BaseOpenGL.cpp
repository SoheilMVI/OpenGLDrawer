#include "BaseOpenGL.h"

BaseOpenGL::BaseOpenGL()
{
    // مقداردهی اولیه متغیرها
    m_objectPosition = QVector3D(0.0f, 0.0f, 0.0f);
}

BaseOpenGL::~BaseOpenGL()
{
    // آزادسازی منابع
    if (m_program)
    {
        delete m_program;
        m_program = nullptr;
    }
    if (m_texture)
    {
        delete m_texture;
        m_texture = nullptr;
    }
    m_vbo.destroy();
}

void BaseOpenGL::setSize(const QSize &size)
{
    m_size = size;
}

int BaseOpenGL::width() const
{
    return m_size.width();
}

int BaseOpenGL::height() const
{
    return m_size.height();
}

QVector3D BaseOpenGL::objectPosition() const
{
    return m_objectPosition;
}

void BaseOpenGL::setObjectPosition(const QVector3D &position)
{
    m_objectPosition = position;
}
