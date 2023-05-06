#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtGlobal>

#include <QLocale>
#include <QTranslator>

#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    // const char *file = context.file ? context.file : "";
    // const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        PLOGD << std::format("{}", localMsg.constData());
        break;
    case QtInfoMsg:
        PLOGI << std::format("{}", localMsg.constData());
        break;
    case QtWarningMsg:
        PLOGW << std::format("{}", localMsg.constData());
        break;
    case QtCriticalMsg:
        PLOGE << std::format("{}", localMsg.constData());
        break;
    case QtFatalMsg:
        PLOGF << std::format("{}", localMsg.constData());
        break;
    }

}

int main(int argc, char *argv[])
{
#ifdef CI
    plog::init(plog::info, "docspeak.log");
    qInstallMessageHandler(messageHandler);
    PLOGI << "Starting DocSpeak GUI Release";
#else
    plog::init(plog::verbose, "docspeak.log");
    PLOGI << "Starting DocSpeak GUI Debug";
#endif

    QGuiApplication app(argc, argv);

    app.setApplicationName("DocSpeak");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Jospeph Bisso");
    app.setOrganizationDomain("github.com/JosephBisso");


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "doc_speak-gui_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
