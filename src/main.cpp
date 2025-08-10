#include "RectangleItem.h"
#include "RectangleWindow.h"
#include "TriangleWindow.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s \t\t\t(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s \t\t\t(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s \t\t\t(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s \t\t\t(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s \t\t\t(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        abort();
    }
}

int main(int argc, char **argv)
{
    qInstallMessageHandler(myMessageHandler);
    QGuiApplication app(argc, argv);

    // Set OpenGL format
    QSurfaceFormat format;
    format.setSamples(16);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    //[Rectangle Configure]
    RectangleWindow rectangleWindow;
    rectangleWindow.setFormat(format);
    rectangleWindow.resize(800, 800);
    rectangleWindow.show();
    rectangleWindow.setAnimating(true);
    //![Rectangle Configure]

    //[Triangle Configure]
    TriangleWindow triangleWindow;
    triangleWindow.setFormat(format);
    triangleWindow.resize(640, 480);
    triangleWindow.show();
    //![Triangle Configure]

    // Register the QML type
    qmlRegisterType<RectangleItem>("RectangleItem", 1, 0, "RectangleItem");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

/*
//! [2]
int main(int argc, char **argv)
{
    qInstallMessageHandler(myMessageHandler);
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    //[Triangle Configure]
    // TriangleWindow window;
    // window.setFormat(format);
    // window.resize(640, 480);
    // window.show();
    //![Triangle Configure]

    //[Rectangle Configure]
    RectangleWindow window;
    window.setFormat(format);
    window.resize(800, 800);
    window.show();
    window.setAnimating(true);
    //![Rectangle Configure]

    //[Rectangle as RegisterType]
    // qmlRegisterType<RectangleWindowWrapper>("RectangleWindowWrapper", 1, 0, "RectangleWindowWrapper");
    // QQuickView view;
    // view.setResizeMode(QQuickView::SizeRootObjectToView);
    // view.setSource(QUrl(QStringLiteral("qrc:/Main.qml")));
    // view.show();
    //![Rectangle as RegisterType]

    //[Rectangle in qml Configure]
    // QQuickView view;
    // view.setResizeMode(QQuickView::SizeRootObjectToView);
    // view.setSource(QUrl(QStringLiteral("qrc:/Main.qml")));
    // view.show();
    // RectangleWindow window(&view);
    // window.setFormat(format);
    // window.resize(500, 500);
    // window.setPosition(QPoint(50,50));
    // window.show();
    // window.setAnimating(true);
    //![Rectangle in qml Configure]

    return app.exec();
}
*/
