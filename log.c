#include <time.h>
#include <stdbool.h>
#include <stddef.h>

#include "log.h"
#include "file.h"

FILE * logFile;
char Time[30];
char logFileName[] = "logSpell.txt";
char lastChar[] = "\n";
char startText[] = "------START LOG------\0";
char endText[] =   "-------END LOG-------\0";
bool isLogged = false;
bool NewLine = true;

char * get_formatted_time(char * buffer, size_t buffer_size) {
    time_t raw_time = time(NULL);
    struct tm *time_info = localtime(&raw_time);

    strftime(buffer, buffer_size, "[%d, %b %H:%M:%S] : ", time_info);
    return buffer;
}

void StartLog() {
        logFile = OpenFile(logFileName, "a");
        if (logFile == NULL) {
                printf("Erreur : %s\n - le fichier log.txt n'existe pas.\n", sqlLikeGetErreur());
                return;
        }
        isLogged = true;
        WriteLogln(startText);
}

void WriteLog(const char *text) {
        if (isLogged) {
                int tailleText = getStrSize(text);
                if (NewLine) {
                        get_formatted_time(Time, 30);
                        EcrireLast(logFile, (void *) Time, 1, 2);
                        for (int i = 2; Time[i] != '\0'; i++) {
                                EcrireLast(logFile, (void *) &Time[i], 1, 1);
                                if (Time[i - 2] == ':' && Time[i - 1] == ' ') break;
                        }
                        NewLine = false;
                }
                EcrireLast(logFile, (void *) text, tailleText - 1, 1);
        }
}

void WriteLogln(const char *text) {
        if (isLogged) {
                int tailleText = getStrSize(text);
                if (NewLine) {
                        get_formatted_time(Time, 30);
                        EcrireLast(logFile, (void *) Time, 1, 2);
                        for (int i = 2; Time[i] != '\0'; i++) {
                                EcrireLast(logFile, (void *) &Time[i], 1, 1);
                                if (Time[i - 2] == ':' && Time[i - 1] == ' ') break;
                        }
                }
                NewLine = true;
                EcrireLast(logFile, (void *) text, tailleText - 1, 1);
                EcrireLast(logFile, (void *) lastChar, 1, 1);
        }
}

int getStrSize(const char * text) {
        int i = 0;
        while (text[i] != '\0') {
                i++;
        }
        return i;
}

void EndLog() {
        WriteLogln(endText);
        CloseFile(logFile);
}
