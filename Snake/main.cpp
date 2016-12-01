#include <QGuiApplication>
#include <QQuickView>
#include "SnakeGlobal.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Snake>("com.gmail.ddkv587", 1, 0, "Snake");

    QQuickView viewer;
    viewer.setSource(QUrl(("qrc:/main.qml")));
    viewer.show();

    return app.exec();
}

