#ifndef LOGGER_H
#define LOGGER_H

/// \warning make shure that you are using compiler native libstdc...dll,
/// but not QT libstdc...dll, or it causes errors
/// see http://forum.qt.io/topic/45306/solved-runtime-error-with-mingw-ifstream-qt-creator/11
#include <fstream>

#include <QThread>

namespace Log
{
    /// Logger is running in its oun thread and is independed from other;
    /// Use '<<' operator to put the message into logfile;
    class Logger : public QThread
    {
        Q_OBJECT

        private: bool _isEventLoopRunning = false;
        private: std::ofstream _logFileStream;

        /// Output format is
        /// [hh.mm.ss] msg std::endl
        public : void operator << (const std::string &msg) noexcept;
        private: Q_SIGNAL void signal_sendMsgToLogger(const QString &msg);

        public : void run() override;

        /// Define parent to destroy logger on parent destruction
        public : Logger(const std::string &logFileName, QObject *parent = 0);
        public : ~Logger();
    };
}

#endif // LOGGER_H
