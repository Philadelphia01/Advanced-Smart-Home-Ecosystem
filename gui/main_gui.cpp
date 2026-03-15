#include <QApplication>
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setApplicationName("Smart Home Ecosystem");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("APM11A1");

    // Set a clean application font
    QFont font("Segoe UI", 10);
    app.setFont(font);

    MainWindow window;
    window.show();

    return app.exec();
}
