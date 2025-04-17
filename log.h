#ifndef LOG_H
#define LOG_H

#include <stddef.h>

char * get_formatted_time(char * buffer, size_t buffer_size);
void StartLog();
int getStrSize(const char * text);
void WriteLog(const char * text);
void WriteLogln(const char * text);
void EndLog();

#endif
