#ifndef BOUTON_H
#define BOUTON_H

#include <stdbool.h>

#include "window.h"
#include "main.h"

typedef struct Pos Pos;
typedef struct Text Text;
typedef struct Bouton Bouton;
typedef struct MouseCollideBox MouseCollideBox;

typedef struct Bouton {
        MouseCollideBox * collider;
        Text * text;
        color * normal;
        color * over;
        color * clicked;
        bool Draw;
        int ID;
} Bouton;

typedef struct Text {
        char * text;
        int size;
        color * color;
        Pos pos;
        float angle;
} Text;

typedef struct MouseCollideBox {
        Pos start;
        Pos end;
        bool wasOver;
        void (*whenOver)(int ID);
        void (*whenNotOver)(int ID);
        void (*whenRelease)(int ID);
        void (*whenClick)(int ID);
        int ID;
} MouseCollideBox;

MouseCollideBox * newCollideBox(MouseCollideBox * ColliderArray[], int nbColliderinArray, float x, float y, float w, float h, void (*whenOver)(int ID), void (*whenNotOver)(int ID), void (*whenClick)(int ID), void (*whenRelease)(int ID), int ID);
void Overed(int ID);
void NotOvered(int ID);
void Clicked(int ID);
void Release(int ID);
void clickedBoutonCategorie(int ID);
void initBouton();

#endif