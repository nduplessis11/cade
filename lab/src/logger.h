#pragma once
typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TRACE
} log_level;

void log_output(log_level level, const char* file, int line, const char* fmt, ...);

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1

#ifdef DEBUG
  #define LOG_DEBUG_ENABLED 1
  #define LOG_TRACE_ENABLED 1
#else
  #define LOG_DEBUG_ENABLED 0
  #define LOG_TRACE_ENABLED 0
#endif

#define CADE_FATAL(fmt, ...) log_output(LOG_LEVEL_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define CADE_ERROR(fmt, ...) log_output(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#if LOG_WARN_ENABLED
  #define CADE_WARN(fmt, ...) log_output(LOG_LEVEL_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define CADE_WARN(fmt, ...) // Do nothing
#endif

#if LOG_INFO_ENABLED
  #define CADE_INFO(fmt, ...) log_output(LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define CADE_INFO(fmt, ...) // Do nothing
#endif

#if LOG_DEBUG_ENABLED
  #define CADE_DEBUG(fmt, ...) log_output(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define CADE_DEBUG(fmt, ...) // Do nothing
#endif

#if LOG_TRACE_ENABLED
  #define CADE_TRACE(fmt, ...) log_output(LOG_LEVEL_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define CADE_TRACE(fmt, ...) // Do nothing
#endif
