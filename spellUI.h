#ifndef SPELLUI_H
#define SPELLUI_H

#include "bouton.h"

typedef enum CategoryType{
        CONSTANT,
        DECLENCHEUR,
        CONDITION,
        INPUT,
        TRANSFORM,
        EFFET,
        AUCUN
} CategoryType;

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

typedef struct noeudModule {
        Text name;
        MouseCollideBox collider;
        unsigned char nbIn;
        noeudModuleIn * inputs;
        unsigned char nbOut;
        noeudModuleOut * outputs;
        noeudModule * suivant;
} noeudModule;

typedef struct noeudModuleIn{
        Text name;
        VarType type;
        void * value;
        unsigned char nbout;
} noeudModuleIn;

typedef struct noeudModuleOut{
        Text name;
        noeudModuleIn * linked;
} noeudModuleOut;

noeud * NewNoeud(const char name[16], unsigned char nbIn, unsigned char nbout, ...);
category * NewCategory(const char name[16], Uint32 nbElement, noeud * Elements[]);

#endif