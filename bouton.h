#ifndef BOUTON_H
#define BOUTON_H

#include <stdbool.h>

#include "window.h"
#include "main.h"

typedef struct Pos Pos;
typedef struct Bouton Bouton;
typedef struct MouseCollideBox MouseCollideBox;

typedef struct Bouton {
        MouseCollideBox * collider;
        //Text * text;
} Bouton;

typedef struct MouseCollideBox {
        Pos start;
        Pos end;
        bool wasOver;
        void (*whenOver)();
        void (*whenNotOver)();
        void (*whenCollide)();
        int ID;
} MouseCollideBox;

MouseCollideBox * newCollideBox(MouseCollideBox * ColliderArray[], int nbCollider, float x, float y, float w, float h, void (*whenOver)(), void (*whenNotOver)(), void (*whenCollide)());

void Overed();
void NotOvered();
void Clicked();

#endif