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
    GLog(const std::string& title);
    ~GLog();
    
    void info(const char* format, ...);
    void error(const char* format, ...);
    void warning(const char* format, ...);
    void layer(const char* format, ...);

private:
    const std::string title;
    
    void log(const GLogType type, const char* format, va_list args);
};

};

#endif    //    SPCGANESHAENGINE_GLOG_H
