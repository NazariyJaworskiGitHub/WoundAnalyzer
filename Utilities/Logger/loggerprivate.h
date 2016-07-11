#ifndef LOGGERPRIVATE_H
#define LOGGERPRIVATE_H

#include <fstream>
#include <ctime>
#include <iomanip>
#include <QObject>
//#include <QMutex>

namespace Log
{
    /// It should be hidden! Don't use directly
    class _HiddenWriter : public QObject
    {
        Q_OBJECT

        private: std::ofstream &_logFileStream;
//        private: QMutex _myMutex;

        public : Q_SLOT void _writeMsgToLog(const QString &msg)
        {
//            _myMutex.lock();

            std::time_t _time = std::time(NULL);
            std::tm _timeInfo(*std::localtime(&_time));
            _logFileStream << "["
                           << std::setfill('0') << std::setw(2) << _timeInfo.tm_hour
                           << ":"
                           << std::setfill('0') << std::setw(2) << _timeInfo.tm_min
                           << ":"
                           << std::setfill('0') << std::setw(2) << _timeInfo.tm_sec
                           << "] ";
            // for asynchron GUI messages: '\r' + msg + '>'
            if(*msg.begin() != '\r') _logFileStream << msg.toStdString();
            else
            {
                QString _copy(msg);
                _copy.remove(_copy.length()-1,1);   // '>'
                _copy.remove(0,1);                  // '\r'
                _logFileStream << _copy.toStdString();
            }
//            _myMutex.unlock();
        }

        public : _HiddenWriter(std::ofstream &logFileStream) :
            _logFileStream(logFileStream){}
        public : ~_HiddenWriter(){}
    };
}

#endif // LOGGERPRIVATE_H

