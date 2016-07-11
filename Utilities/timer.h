#ifndef TIMER
#define TIMER

#include <chrono>
#include <string>
#include <sstream>

class Timer
{
    private: std::chrono::steady_clock::time_point _timePointBegin;
    private: std::chrono::steady_clock::time_point _timePointEnd;

    public : Timer(){}

    public : void start()
    {
        _timePointBegin = std::chrono::steady_clock::now();
    }
    public : void stop()
    {
        _timePointEnd = std::chrono::steady_clock::now();
    }
    public : std::chrono::duration<double> getTimeSpan() const
    {
        return std::chrono::duration_cast<
                std::chrono::duration<double>>(_timePointEnd - _timePointBegin);
    }
    public : std::string getTimeSpanAsString() const
    {
        std::stringstream _str;
        _str << getTimeSpan().count();
        return  _str.str();
    }

    public :~Timer(){}
};

#endif // TIMER

