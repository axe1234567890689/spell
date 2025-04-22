#ifndef MAIN_H
#define MAIN_H

#include "window.h"

typedef struct Pos Pos;
typedef struct IPos IPos;
typedef struct noeud noeud;
typedef struct category category;
typedef struct Player Player;
typedef struct team team;
typedef struct SortLinked SortLinked;
typedef struct PlayerLinked PlayerLinked;
typedef struct Sort Sort;

typedef enum {
        CONSTANT,
        DECLENCHEUR,
        CONDITION,
        INPUT,
        TRANSFORM,
        EFFET,
        AUCUN
} CategoryType;

typedef struct Pos {
        float x;
        float y;
} Pos;

typedef struct IPos {
        int x;
        int y;
} IPos;

typedef struct noeud{
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

typedef struct SortLinked {
        Sort * sort;
        struct SortLinked * suivant;
} SortLinked;

typedef struct PlayerLinked {
        Player * player;
        struct PlayerLinked * suivant;
} PlayerLinked;

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
        Pos * target;
        Player * proprio;
        int ID;
        SortLinked ** spellAround;
        PlayerLinked ** playerAround;
} Sort;

noeud * NewNoeud(const char name[16], unsigned char nbIn, unsigned char nbout, ...);
category * NewCategory(const char name[16], Uint32 nbElement, noeud * Elements[]);

void draw();
void freeNoeud(noeud * noeuds);
void freeCategory(category * categories);
void printCategorie(category * categories);
void printNoeud(noeud * current);

void Overed(int ID);
void NotOvered(int ID);
void Clicked(int ID);
void Release(int ID);
void clickedBoutonCategorie(int ID);
void ReleaseBoutonCategorie(int ID);
void OveredBoutonCategorie(int ID);
void NotOveredBoutonCategorie(int ID);

void printff(const char * text);
void logstr(const char * text);

#endif