#ifndef BASEOPENGLWINDOW_H
#define BASEOPENGLWINDOW_H

#include "BaseOpenGL.h"
#include <QOpenGLPaintDevice>
#include <QWindow>

class BaseOpenGLWindow : public QWindow, protected BaseOpenGL
{
public:
    explicit BaseOpenGLWindow(QWindow *parent = nullptr);
    ~BaseOpenGLWindow();

    virtual void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating = false;

    QOpenGLContext *m_context = nullptr;
    QOpenGLPaintDevice *m_device = nullptr;
};

#endif // BASEOPENGLWINDOW_H
