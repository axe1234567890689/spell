#ifndef FILE_H
#define FILE_H

#include <stdio.h>

FILE * OpenFile(char * name, const char * mode);
void CloseFile(FILE * file);
unsigned char Ecrire(FILE * file, void * data, unsigned char size, unsigned char nbElements, long CursorPos);
unsigned char EcrireLast(FILE * file, void * data, unsigned char size, unsigned char nbElements);
unsigned char Lire(FILE * file, void * data, unsigned char size, unsigned char nbElements, long CursorPos);
void SetError(const char * text);
char * sqlLikeGetErreur();

#endif
