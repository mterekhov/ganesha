#include <stdio.h>
#include <time.h>
#include <chrono>
#include <iomanip>

#include "glog.h"

namespace spcGaneshaEngine {

GLog::GLog(const std::string& title) : title(title) {
    
}

GLog::~GLog() {
    
}

void GLog::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(GLOG_TYPE_INFO, format, args);
    va_end(args);
}

void GLog::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(GLOG_TYPE_ERROR, format, args);
    va_end(args);
}

void GLog::warning(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(GLOG_TYPE_WARNING, format, args);
    va_end(args);
}

void GLog::layer(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(GLOG_TYPE_LAYER, format, args);
    va_end(args);
}

void GLog::log(const GLogType type, const char* format, va_list args) {
#ifndef DEBUG
    return;
#endif
    
    std::chrono::system_clock::time_point nowTimePoint = std::chrono::system_clock::now();
    std::chrono::system_clock::duration epochTime = nowTimePoint.time_since_epoch();
    epochTime -= duration_cast<std::chrono::seconds>(epochTime);
    time_t nowTimeStamp = std::chrono::system_clock::to_time_t(nowTimePoint);
    tm localTime = *localtime(&nowTimeStamp);
    char const *timeFormat = "%02u-%02u-%04u %02u:%02u:%02u.%03u";
    char timeBuffer[BUFSIZ] = {0};
    snprintf(timeBuffer, BUFSIZ, timeFormat, 
             localTime.tm_mday,
             localTime.tm_mon + 1,
             localTime.tm_year + 1900,
             localTime.tm_hour,
             localTime.tm_min,
             localTime.tm_sec,
             static_cast<unsigned>(epochTime / std::chrono::milliseconds(1)));

    std::string newFormat = timeBuffer;
    switch (type) {
        case GLOG_TYPE_INFO:
            newFormat += " ";
            newFormat += title;
            newFormat += ": ";
            break;
        case GLOG_TYPE_WARNING:
            newFormat += " ";
            newFormat += title;
            newFormat += "-WARNING: ";
            break;
        case GLOG_TYPE_ERROR:
            newFormat += " ";
            newFormat += title;
            newFormat += "-ERROR: ";
            break;
        case GLOG_TYPE_LAYER:
            newFormat += " ";
            newFormat += title;
            newFormat += "-LAYER: ";
            break;
    }
    newFormat += format;

    vprintf(newFormat.c_str(), args);
}

}

