#include <QApplication>
#include <QStyleFactory>
#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Job Service Client");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("JobService");

    // Use Fusion style for consistent cross-platform look
    app.setStyle(QStyleFactory::create("Fusion"));

    MainWindow window;
    window.show();

    return app.exec();
}
