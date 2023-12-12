//
//  GLog.cpp
//  yajnavalkya
//
//  Created by cipher on 08.09.2023.
//

#include <stdio.h>
#include <time.h>
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

void GLog::log(const GLogType type, const char* format, va_list args) {
#ifndef DEBUG
    return;
#endif
    
    time_t rawtime;
    time(&rawtime);
    char timeBuffer[BUFSIZ] = {0};
    char const *timeFormat = "%d-%m-%Y %H:%M:%S";
    strftime(timeBuffer, BUFSIZ, timeFormat, localtime(&rawtime));

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

        default:
            break;
    }
    newFormat += format;

    vprintf(newFormat.c_str(), args);
}

}

