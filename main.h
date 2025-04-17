#ifndef MAIN_H
#define MAIN_H

#include "window.h"

typedef struct Pos {
        float x;
        float y;
} Pos;

typedef struct IPos {
        int x;
        int y;
} IPos;

typedef struct noeud
{
        char name[16];
        unsigned char nbIn;
        unsigned char nbout;
        char ** input;
        char ** output;
} noeud;

typedef struct category {
        char name[16];
        uint32_t nbElement;
        struct category * suivant;
        noeud * elements[];
} category;

typedef struct Player {
        int vie;
        int vieMax;
        int mana;
        int manaMax;
        color * color;
        Pos pos;
        int ID;
        int teamID;
} Player;

typedef struct team {
        
} team;

typedef struct Sort {
        Pos pos;
        float rayon;
        float density;
        Pos targetPos;
        float vitesse;
        float vitesseMax;
        float direction;
        float rayonSpell;
        float rayonPlayer;
        color * color;
        int targetID;
        int proprioID;
        int ID;
        int * spellLinkedID;
        struct Sort ** spellAround;
        Player ** playerAround;
} Sort;

noeud * NewNoeud(const char name[16], unsigned char nbIn, unsigned char nbout, ...);
category * NewCategory(const char name[16], Uint32 nbElement, noeud * Elements[]);
void draw();
void freeNoeud(noeud * noeuds);
void freeCategory(category * categories);
void printCategorie(category * categories);
void printNoeud(noeud * current);

void printff(const char * text);
void logstr(const char * text);

#endif