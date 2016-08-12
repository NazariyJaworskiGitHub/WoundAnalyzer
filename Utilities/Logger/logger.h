#ifndef LOGGER_H
#define LOGGER_H

/// \warning make shure that you are using compiler native libstdc...dll,
/// but not QT libstdc...dll, or it causes errors
/// see http://forum.qt.io/topic/45306/solved-runtime-error-with-mingw-ifstream-qt-creator/11

#include <fstream>
#include <ctime>
#include <iomanip>

namespace Log
{
    class StaticLogger
    {
        private: std::ofstream _logFileStream;
        private: StaticLogger(const std::string &logFileName = "LogFile.log")
        {
            _logFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            _logFileStream.open(logFileName);
            *this << "Logging is started.\n";
        }
        public : void operator << (const std::string &msg) noexcept
        {
            std::time_t _time = std::time(NULL);
            std::tm _timeInfo(*std::localtime(&_time));
            _logFileStream << "["
                           << std::setfill('0') << std::setw(2) << _timeInfo.tm_hour
                           << ":"
                           << std::setfill('0') << std::setw(2) << _timeInfo.tm_min
                           << ":"
                           << std::setfill('0') << std::setw(2) << _timeInfo.tm_sec
                           << "] ";
            _logFileStream << msg;
            _logFileStream.flush();
        }
        private: ~StaticLogger()
        {
            *this << "Logging is finished.\n";
            _logFileStream.flush();
            _logFileStream.close();
        }
        public : static StaticLogger& instance(){
            static StaticLogger _instanceOfStaticLogger;
            return _instanceOfStaticLogger;}
    };
}

#endif // LOGGER_H
