#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void log_output(log_level level, const char* file, int line, const char* fmt, ...) {
    const char* level_strings[] = { "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE" };

    // Start variable argument processing
    va_list args;
    va_start(args, fmt);

    // Format the message
    char message_buffer[1024];
    vsnprintf(message_buffer, sizeof(message_buffer), fmt, args);

    va_end(args);

    // Print the log message
    fprintf(stderr, "[%s] [%s:%d] %s\n", level_strings[level], file, line, message_buffer);

    // Handle fatal errors
    if (level == LOG_LEVEL_FATAL) {
        abort();  // Terminate the application
    }
}
