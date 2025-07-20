#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pix(":/images/images/splash.png");
    if (pix.isNull()) {
        qWarning() << "Splash image not found!";
    }

    QSplashScreen *splash = new QSplashScreen(pix);
    splash->show();

    splash->showMessage("Loaded modules");

    a.processEvents();

    MainWindow *w = new MainWindow;

    splash->showMessage("Established connections");

    a.processEvents();

    QTimer::singleShot(5500, splash, [splash]() {
    splash->close();
    });

    QTimer::singleShot(5500, w, [w]() {
    w->show();
    });

    return a.exec();
}
