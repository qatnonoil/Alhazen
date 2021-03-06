﻿#include "logging.hpp"
#include "util.hpp"

/*
-------------------------------------------------
-------------------------------------------------
*/
void loggingCore(LoggingLevel level, const char* format, ...)
{
    //
    const char* levelStr = (level == LoggingLevel::LOG_INFO)
                               ? "[INFO] "
                               : (level == LoggingLevel::LOG_TRACE)
                                     ? "[TRACE]"
                                     : (level == LoggingLevel::LOG_WARNING)
                                           ? "[WARN] "
                                           : (level == LoggingLevel::LOG_ERROR)
                                                 ? "[ERR]  "
                                                 : "[???]  ";
    const time_t timer = time(nullptr);
#if defined(WINDOWS)
    tm date;
    localtime_s(&date, &timer);
#else
    const tm date = *localtime(&timer);
#endif
    printf("[%04d/%02d/%02d %02d:%02d:%02d] %s",
           date.tm_year + 1900,
           date.tm_mon + 1,
           date.tm_mday,
           date.tm_hour,
           date.tm_min,
           date.tm_sec,
           levelStr);

    // 本体の出力
    va_list valist;
    va_start(valist, format);
    vprintf(format, valist);
    va_end(valist);
    //
    printf("\n");
    // errorの場合はここで終了させてしまう
    if (level == LoggingLevel::LOG_ERROR)
    {
        fflush(stdout);
        exit(-1);
    }
}
