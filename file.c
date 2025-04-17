#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"

int maxCharError = 128;
char erreur[128];

FILE * OpenFile(char * name, const char * mode) {
        FILE *file = fopen(name, mode);
        if (file == NULL) {
                SetError("erreur lors de l'ouverture du fichier.");
                return NULL;
        }
        return file;
}

void CloseFile(FILE * file) {
        fclose(file);
}

unsigned char Ecrire(FILE * file, void * data, unsigned char size, unsigned char nbElements, long CursorPos) {
        fseek(file, CursorPos, SEEK_SET);
        int nbSuccesse = fwrite(data, size, nbElements, file);
        if (nbSuccesse == nbElements) {
                return 0;
        }
        return nbElements - nbSuccesse;
}

unsigned char EcrireLast(FILE * file, void * data, unsigned char size, unsigned char nbElements) {
        fseek(file, 0, SEEK_END);
        int nbSuccesse = fwrite(data, size, nbElements, file);
        if (nbSuccesse == nbElements) {
                return 0;
        }
        return nbElements - nbSuccesse;
}

unsigned char Lire(FILE *file, void *data, unsigned char size, unsigned char nbElements, long CursorPos) {
        fseek(file, CursorPos, SEEK_SET);
        int nbSuccesse = fread(data, size, nbElements, file);
        if (nbSuccesse == nbElements) {
                return 0;
        }
        return nbElements - nbSuccesse;
}

void SetError(const char *text) {
        strncpy(erreur, text, maxCharError - 1);
        erreur[maxCharError - 1] = '\0';
}

char *sqlLikeGetErreur() {
        return erreur;
}
