#include <stdio.h>
#include <stdarg.h>

#include "log.h"

FILE *log_Handle;

void Log_Open(const char *file_Name)
{
    log_Handle = fopen(file_Name, "w");
    
    return;
}

void Log_Redirect(const char *format, ...)
{
    if (NULL == log_Handle)
    {
        return;
    }

    static int log_Times;
    va_list parg;

    va_start(parg, format);
    vfprintf(log_Handle, format, parg);
    va_end(parg);

    ++log_Times;
    if (LOG_FREQUENCY == log_Times)
    {
        fclose(log_Handle);
        log_Handle = fopen(LOG_PATH, "a");
        log_Times = 0;
    }

    return;
}
