#ifndef WINDOW_H
#define WINDOW_H
#include <SDL.h>
#include <stdbool.h>

typedef enum typ {
        WIN_COLOR,
        GRADIANT,
        WINDOW
} typ;

typedef struct typeCast {
        typ type;
        void *value;
        struct typeCast * suivant;
} typeCast;

typedef struct window {
        SDL_Window * window;
        SDL_Renderer * renderer;
        Uint32 id;
        int w;
        int h;
} window;

typedef struct color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
} color;

typedef struct gradiantUC {
        unsigned char start;
        unsigned char end;
} gradiantUC;

typedef struct gradiantI {
        int start;
        int end;
} gradiantI;

typedef struct gradiantF {
        float start;
        float end;
} gradiantF;

typedef struct gradiantColor {
        gradiantUC r;
        gradiantUC g;
        gradiantUC b;
        gradiantUC a;
        gradiantF encadrement;
} gradiantColor;

void printfASuppr();

void InitWindow();
window * NewWindow(char *titre, int x, int  y, int w, int  h, int windowFlag, int rendererFlag);

void RenderPresent(window * windowe);
struct color *NewColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void ChangeColor(window * windowe, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void ChangeColorC(window * windowe, struct color *color);
void DrawPoint(window * windowe, int x, int y);
void DrawString(window * windowe, int x, int y, const char *s);
void DrawLine(window * windowe, int x1, int y1, int x2, int y2);
void DrawRectangle(window * windowe, int x1, int y1, int x2, int y2, bool fill, int arondissement);
void DrawCircle(window * windowe, int centerx, int centery, int r, int diametreInterne);
void DrawArcCircle(window * windowe, int centerx, int centery, int r, int diametreInterne, float angleA, float angleB, gradiantColor *gradiant);
void DrawLine(window * windowe, int x1, int y1, int x2, int y2);
void DrawLineHole(window * windowe, int x1, int y, int x2, int distance, int xCerclr, int yCercle);
void DrawLineHoleArc(window * windowe, int x1, int y, int x2, int distance, int xCercle, int yCercle, float angleA, float angleB, bool chiant, gradiantColor *gradiant);
void Pause(int delay);

void ChangeColorGradiant(window * windowe, float value, gradiantColor *gradiant, bool chiant);
gradiantColor *NewGradiant(color *couleura, color *couleurb, float start, float end);

float interpolationF(float a, float b, float i);
unsigned char interpolationUC(unsigned char a, unsigned char b, float i);
int interpolationI(int a, int b, float i);

void NewTypeCast(void *value, typ type);

window * CloseWindow(window * windowe);
void CloseWindows();

#endif