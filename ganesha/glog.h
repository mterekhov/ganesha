#ifndef SPCGANESHAENGINE_GLOG_H
#define SPCGANESHAENGINE_GLOG_H

#include <stdarg.h>
#include <string>

namespace spcGaneshaEngine {

enum GLogType {
    GLOG_TYPE_INFO,
    GLOG_TYPE_WARNING,
    GLOG_TYPE_ERROR,
    GLOG_TYPE_LAYER
};

class GLog {
public:
    static void info(const char* format, ...);
    static void error(const char* format, ...);
    static void warning(const char* format, ...);
    static void layer(const char* format, ...);
    
private:    
    static const std::string title;
    
    static void log(const GLogType type, const char* format, va_list args);
};

#define GLOG_INFO(...) GLog::info(__VA_ARGS__)
#define GLOG_ERROR(...) GLog::error(__VA_ARGS__)
#define GLOG_WARNING(...) GLog::warning(__VA_ARGS__)
#define GLOG_LAYER(...) GLog::layer(__VA_ARGS__)

};


#endif    //    SPCGANESHAENGINE_GLOG_H
