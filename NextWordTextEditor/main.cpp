//include necessary classes
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

//include header for class mainwindow
#include "mainwindow.h"

//main function
//set application, call main window
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(NextWordTextEditor);

    QApplication app(argc, argv);
    MainWindow window;

    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Next Word Text Editor");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    if (!parser.positionalArguments().isEmpty())
        window.loadFile(parser.positionalArguments().first());

    window.show();
    return app.exec();
}
