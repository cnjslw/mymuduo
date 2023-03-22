#pragma once

#include "noncopyable.h"

#include <string>

#define LOG_INFO(logmsgformat, ...)                       \
    do {                                                  \
        Logger& logger = Logger::instance();              \
        logger.setLogLevel(INFO);                         \
        char buf[1024] = { 0 };                           \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_ERROR(logmsgformat, ...)                      \
    do {                                                  \
        Logger& logger = Logger::instance();              \
        logger.setLogLevel(INFO);                         \
        char buf[1024] = { 0 };                           \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)

#define LOG_FATAL(logmsgformat, ...)                      \
    do {                                                  \
        Logger& logger = Logger::instance();              \
        logger.setLogLevel(INFO);                         \
        char buf[1024] = { 0 };                           \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
        exit(-1);                                         \
    } while (0)

#ifdef MUDEBUG
#define LOG_DEBUG(logmsgformat, ...)                      \
    do {                                                  \
        Logger& logger = Logger::instance();              \
        logger.setLogLevel(INFO);                         \
        char buf[1024] = { 0 };                           \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(buf);                                  \
    } while (0)
#else
#define LOG_DEBUG(logmsgformat, ...)
#endif

// 日志级别 INFO ERROR FATAL DEBUG
enum LogLevel {
    INFO,
    ERROR,
    FATAL,
    DEBUG,
};

class Logger : noncopyable {
public:
    static Logger& instance();
    void setLogLevel(int level);
    void log(std::string msg);

private:
    int LogLevel_;
};
