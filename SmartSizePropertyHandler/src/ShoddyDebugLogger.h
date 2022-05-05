/*
 * ShoddyDebugLogger.h
 */
#ifndef __ShoddyDebugLogger_H__
#define __ShoddyDebugLogger_H__

#include <ctime>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>

//#define DEBUG_FLAG
#ifdef DEBUG_FLAG

// TODO: ちゃんとする
#define DEBUGLOG_FILEPATH "C:\\Users\\mitsu\\works\\SmartSizePropertyHandler\\debug.log"
inline void DebugLog(std::string &log)
{
    // build log string
    std::ostringstream os;
    os << "echo ";

    // get time
    const std::time_t t = std::time(nullptr);
    struct std::tm tm;
    if (localtime_s(&tm, &t) == 0) {
		char s_time[20];
		std::strftime(s_time, ARRAYSIZE(s_time), "%Y/%m/%d-%H:%M:%S", &tm);
        os << "[" << s_time << "] ";
    }

    os << log << " >> " << DEBUGLOG_FILEPATH;
    std::system(os.str().c_str());
}

template <typename ...Args>
inline void DebugLog(const std::string &fmt, Args ...args)
{
    const size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args...);
    std::vector<char> buff(len + 1);
    std::snprintf(&buff[0], len + 1, fmt.c_str(), args...);

    std::string str = std::string(&buff[0], &buff[0] + len);
    DebugLog(str);
}

#else

template <typename ...Args>
inline void DebugLog(const std::string &s, Args ...args) {}

#endif /* DEBUG_FLAG */

#endif /* __ShoddyDebugLogger_H__ */
