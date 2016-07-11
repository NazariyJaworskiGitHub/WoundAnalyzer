#include "logger.h"
#include "loggerprivate.h"

using namespace Log;

void Logger::operator << (const std::string &msg) noexcept
{
    /// \warning it can lock entire system
    while(!_isEventLoopRunning)
        QThread::currentThread()->sleep(1);
    Q_EMIT signal_sendMsgToLogger(QString(msg.data()));
}

void Logger::run()
{
    _HiddenWriter _writer(_logFileStream);

    connect(this, SIGNAL(signal_sendMsgToLogger(const QString &)),
            &_writer, SLOT(_writeMsgToLog(const QString &)));

    _writer._writeMsgToLog("Logging is started.\n");
    _isEventLoopRunning = true;
    exec();
    _writer._writeMsgToLog("Logging is finished.\n");
}

Logger::Logger(const std::string &logFileName, QObject *parent):
    QThread(parent)
{
    _logFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    _logFileStream.open(logFileName);
    this->start();
}

Logger::~Logger()
{
    quit();
    wait();
    _logFileStream.flush();
    _logFileStream.close();
}
