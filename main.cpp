#include "precompiled.hpp"
#include "meawindow.hpp"

void myLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logMessage = qFormatLogMessage(type, context, msg);
    static QMutex mutex;
    QMutexLocker lock(&mutex);
    static std::ofstream logFile("logfile.txt");
    if (logFile)
    {
        logFile << qPrintable(logMessage) << std::endl;
    }
    fprintf(stderr, "%s\n", logMessage.toLocal8Bit().constData());
    fflush(stderr);
}

int main(int argc, char *argv[])
{
//    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
//    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

    QApplication app(argc, argv);

    qInstallMessageHandler(myLogger);

    MEAWindow win;
    win.show();

    return app.exec();
}
