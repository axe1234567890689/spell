#ifndef MAIN_H
#define MAIN_H

#include "window.h"

typedef struct Pos Pos;
typedef struct Text Text;
typedef struct Bouton Bouton;
typedef struct Animation Animation;
typedef struct AnimModule AnimModule;
typedef struct MouseCollideBox MouseCollideBox;
typedef enum VarType VarType;

typedef struct IPos IPos;
typedef struct noeud noeud;
typedef struct category category;
typedef struct Player Player;
typedef struct team team;
typedef struct SortLinked SortLinked;
typedef struct PlayerLinked PlayerLinked;
typedef struct Sort Sort;
typedef struct noeudModule noeudModule;
typedef struct noeudModuleIn noeudModuleIn;
typedef struct noeudModuleOut noeudModuleOut;

typedef struct Pos {
        float x;
        float y;
} Pos;

typedef struct IPos {
        int x;
        int y;
} IPos;

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

void draw();
void freeNoeud(noeud * noeuds);
void freeCategory(category * categories);
void printCategorie(category * categories);
void printNoeud(noeud * current);
void creatCategorie();

void Overed(uint64_t ID);
void NotOvered(uint64_t ID);
void Clicked(uint64_t ID);
void Release(uint64_t ID);
void OveredNoeud(uint64_t ID);
void NotOveredNoeud(uint64_t ID);
void ClickedNoeud(uint64_t ID);
void ReleaseNoeud(uint64_t ID);
void ClickedNoeudModule(uint64_t ID);
void ReleaseNoeudModule(uint64_t ID);
void OveredNoeudModule(uint64_t ID);
void NotOveredNoeudModule(uint64_t ID);
void clickedBoutonCategorie(uint64_t ID);
void ReleaseBoutonCategorie(uint64_t ID);
void OveredBoutonCategorie(uint64_t ID);
void NotOveredBoutonCategorie(uint64_t ID);

void printff(const char * text);
void logstr(const char * text);

#endif