#ifndef __LOG_H__
#define __LOG_H__

#define LOG_PATH "ux0:data/Monopoly.log"
#define LOG_FREQUENCY 2

#define LOG_LEVEL_INFO  (1 << 0)
#define LOG_LEVEL_DEBUG (1 << 1)

void Log_Open(const char *file_Name);
void Log_Redirect(int level, const char *format, ...);
void Set_Log_Level(int level);

#define Log_Info(FORMAT, ...)  Log_Redirect(LOG_LEVEL_INFO, FORMAT, ##__VA_ARGS__)
#define Log_Debug(FORMAT, ...) Log_Redirect(LOG_LEVEL_DEBUG, FORMAT, ##__VA_ARGS__)
 
#endif
