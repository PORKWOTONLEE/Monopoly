#ifndef __LOG_H__
#define __LOG_H__

#define LOG_PATH "app0:log"
#define LOG_FREQUENCY 5

void Log_Open(const char *file_Name);
void Log_Redirect(const char *format, ...);

#define Log_Info(FORMAT, ...) Log_Redirect(FORMAT, ##__VA_ARGS__)
 
#endif
