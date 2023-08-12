#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor); // Qt6中防止窗体二次缩放

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
