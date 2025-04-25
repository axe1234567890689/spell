#ifndef BOUTON_H
#define BOUTON_H

#include <stdbool.h>

#include "window.h"
#include "main.h"

typedef enum VarType{
        INT,
        FLOAT,
        POSITION,
        COLOR,
        BOOL
} VarType;

typedef struct MouseCollideBox {
        Pos start;
        Pos end;
        bool wasOver;
        bool enabled;
        void (*whenOver)(uint64_t ID);
        void (*whenNotOver)(uint64_t ID);
        void (*whenRelease)(uint64_t ID);
        void (*whenClick)(uint64_t ID);
        uint64_t ID;
} MouseCollideBox;

typedef struct AnimModule {
        float min;
        float max;
        VarType type;
        void * target;
} AnimModule;

typedef struct Animation {
        float i;
        float min;
        float max;
        float animated;
        float speed;
        int nbModule;
        AnimModule * modules;
} Animation;

typedef struct Text {
        char * text;
        int size;
        color * color;
        Pos pos;
        float angle;
} Text;

typedef struct Bouton {
        MouseCollideBox collider;
        Text text;
        color * normal;
        color * over;
        color * clicked;
        color colorUse;
        bool Draw;
        Animation anim;
} Bouton;

MouseCollideBox * newCollideBox(MouseCollideBox * ColliderArray[], int nbColliderinArray, float x, float y, float w, float h, void (*whenOver)(uint64_t ID), void (*whenNotOver)(uint64_t ID), void (*whenClick)(uint64_t ID), void (*whenRelease)(uint64_t ID), uint64_t ID);
Bouton * NewBouton(Bouton * BoutonArray[], int nbBoutoninArray, MouseCollideBox * ColliderArray[], int nbColliderinArray, char * text, int TextSize, color * TextColor, color * normalColor, color * overColor, color * clickedColor, float x, float y, float w, float h, void (*whenOver)(uint64_t ID), void (*whenNotOver)(uint64_t ID), void (*whenClick)(uint64_t ID), void (*whenRelease)(uint64_t ID), uint64_t
 ID, float animMin, float animMax);
void initBouton();

#endif