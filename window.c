#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <math.h>
#include <SDL_test_font.h>

#include "window.h"

typeCast * asuppr = NULL;

float interpolationF(float a, float b, float i) {
        return a * -(i - 1) + b * i;
}

unsigned char interpolationUC(unsigned char a, unsigned char b, float i) {
        return (unsigned char) (a * -(i - 1) + b * i);
}

int interpolationI(int a, int b, float i) {
        return (int) (a * -(i - 1) + b * i);
}

void InitWindow() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                SDL_Log("Erreur : %s\n dans %s fichier\n à la ligne %d\n", SDL_GetError(), __FILE__, __LINE__);
                exit(EXIT_FAILURE);
        }
}

void RenderPresent(window * windowe) {
        SDL_RenderPresent(windowe->renderer);
        SDL_RenderClear(windowe->renderer);
}

window * NewWindow(char *titre, int x, int  y, int w, int  h, int windowFlag, int rendererFlag) {
        if (x == -1) {
                x = SDL_WINDOWPOS_CENTERED;
        }
        if (y == -1) {
                y = SDL_WINDOWPOS_CENTERED;
        }

        window * windowe = (window *) malloc(sizeof(window));
        windowe->window = SDL_CreateWindow(titre, x, y, w, h, windowFlag);
        windowe->renderer = SDL_CreateRenderer(windowe->window, -1, rendererFlag);
        windowe->id = SDL_GetWindowID(windowe->window);

        if (windowe->window == NULL || windowe->renderer == NULL) {
                SDL_Log("Erreur : %s\n dans le fichier %s\nà la ligne %d\n", SDL_GetError(), __FILE__, __LINE__);
                CloseWindows();
                exit(EXIT_FAILURE);
        }

        windowe->h = h;
        windowe->w = w;

        NewTypeCast((void *) windowe, WINDOW);

        return windowe;
}

void DrawString(window * windowe, int x, int y, const char *s) {
        SDLTest_DrawString(windowe->renderer, x, y, s);
}

void ChangeColor(window * windowe, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        SDL_SetRenderDrawColor(windowe->renderer, r, g, b, a);
}

void ChangeColorC(window * windowe, struct color *color) {
        SDL_SetRenderDrawColor(windowe->renderer, color->r, color->g, color->b, color->a);
}

void DrawPoint(window * windowe, int x, int y) {
        SDL_RenderDrawPoint(windowe->renderer, x, y);
}

void DrawLine(window * windowe, int x1, int y1, int x2, int y2) {
        SDL_RenderDrawLine(windowe->renderer, x1, y1, x2, y2);
}

void DrawRectangle(window * windowe, int x1, int y1, int x2, int y2, bool fill, int arondissement) {
        SDL_Rect rectangle;
        rectangle.x = x1;
        rectangle.y = y1;
        rectangle.w = x2 - x1;
        rectangle.h = y2 - y1;
        if (fill) {
                SDL_RenderFillRect(windowe->renderer, &rectangle);
        } else {
                SDL_RenderDrawRect(windowe->renderer, &rectangle);
        }
}

void Pause(int delay) {
        SDL_Delay(delay);
}

color *NewColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        color *result = (color *) malloc(sizeof(color));
        result->r = r;
        result->g = g;
        result->b = b;
        result->a = a;
        NewTypeCast((void *) result, WIN_COLOR);
        return result;
}

void NewTypeCast(void * value, typ type) {
        typeCast *result = (typeCast *) malloc(sizeof(typeCast));
        result->type = type;
        result->suivant = NULL;
        result->value = value;
        if (asuppr == NULL) {
                asuppr = result;
                return;
        }
        typeCast *current = asuppr;
        while (current->suivant != NULL) {
                current = current->suivant;
        }
        current->suivant = result;
}

void printfASuppr() {
        typeCast * current = asuppr;
        printf("____________________\n");
        while (current != NULL)
        {
                switch (current->type)
                {
                case WIN_COLOR:
                        printf("        COLOR       \n");
                        printf(" - red = %d\n", ((color * ) current->value)->r);
                        printf(" - green = %d\n", ((color * ) current->value)->g);
                        printf(" - bleu = %d\n", ((color * ) current->value)->b);
                        printf(" - alpha = %d\n", ((color * ) current->value)->a);
                        printf(" - current = %x\n", (void *) &current->value);
                        printf("--------------------\n");
                        (current->suivant == NULL) ? printf(" - suivant = NULL\n") : printf(" - suivant = %x\n", ((void *) current->suivant) + 8);
                        printf("____________________\n");
                        break;
                        
                case GRADIANT:
                        printf("       GRADIANT     \n");
                        printf(" - current = %x\n", (void *) &current->value);
                        printf("--------------------\n");
                        (current->suivant == NULL) ? printf(" - suivant = NULL\n") : printf(" - suivant = %x\n", ((void *) current->suivant) + 8);
                        printf("____________________\n");
                        break;
                
                case WINDOW:
                        printf("        WINDOW      \n");
                        printf(" - current = %x\n", (void *) &current->value);
                        printf("--------------------\n");
                        (current->suivant == NULL) ? printf(" - suivant = NULL\n") : printf(" - suivant = %x\n", ((void *) current->suivant) + 8);
                        printf("____________________\n");
                        break;
                
                default:
                        printf("        UNKNOW      \n");
                        printf("____________________\n");
                        break;
                }
                current = current->suivant;
        }
        
}

gradiantColor *NewGradiant(color *couleura, color *couleurb, float start, float end) {
        gradiantColor *result = (gradiantColor *) malloc(sizeof(gradiantColor));
        if (start == end) {printf("Erreur :\n essaie pour créer un gradiant NULL\n"); return NULL;}
        result->a.start = couleura->a;
        result->a.end   = couleurb->a;
        result->r.start = couleura->r;
        result->r.end   = couleurb->r;
        result->g.start = couleura->g;
        result->g.end   = couleurb->g;
        result->b.start = couleura->b;
        result->b.end   = couleurb->b;
        if (start > end) {
                result->encadrement.start = end;
                result->encadrement.end = start;
        } else {
                result->encadrement.start = start;
                result->encadrement.end   = end;
        }
        NewTypeCast((void *) result, GRADIANT);
        return result;
}

window * CloseWindow(window * windowe) {
        SDL_DestroyWindow(windowe->window);
        SDL_DestroyRenderer(windowe->renderer);

        typeCast *current = asuppr;
        typeCast *precedent = NULL;
        while (current != NULL) {
                if (((window *) current->value) == windowe) {
                        precedent->suivant = current->suivant;
                        free(current->value);
                        return NULL;
                }
                precedent = current;
                current = current->suivant;
        }
        return NULL;
}

void CloseWindows() {
        typeCast *current = asuppr;
        typeCast *asupprimer = NULL;
        while (current != NULL) {
                switch (current->type)
                {
                case WINDOW:
                        SDL_DestroyWindow(((window * )current->value)->window);
                        SDL_DestroyRenderer(((window * )current->value)->renderer);
                        break;
                
                default:
                        break;
                }
                free(current->value);
                asupprimer = current;
                current = current->suivant;
                free(asupprimer);
        }

        SDL_Quit();
}

void DrawCircle(window * windowe, int centerx, int centery, int r, int diametreInterne) {
        int x = 0;
        int y = -r;
        if (r != diametreInterne) {
                while (x <= -y) {
                        DrawLineHole(windowe, centerx - x, centery + y, centerx + x, diametreInterne, centerx, centery);
                        DrawLineHole(windowe, centerx - x, centery - y, centerx + x, diametreInterne, centerx, centery);
                        DrawLineHole(windowe, centerx - y, centery + x, centerx + y, diametreInterne, centerx, centery);
                        DrawLineHole(windowe, centerx - y, centery - x, centerx + y, diametreInterne, centerx, centery);
                        x ++;
                        if ((x * x + (y + 1) * (y + 1)) > r * r) {
                                y ++;
                        }
                }
        } else {
                while (x <= -y) {
                        DrawPoint(windowe, centerx - x, centery + y);
                        DrawPoint(windowe, centerx + x, centery + y);
                        DrawPoint(windowe, centerx - x, centery - y);
                        DrawPoint(windowe, centerx + x, centery - y);
                        DrawPoint(windowe, centerx - y, centery + x);
                        DrawPoint(windowe, centerx + y, centery + x);
                        DrawPoint(windowe, centerx - y, centery - x);
                        DrawPoint(windowe, centerx + y, centery - x);
                        x ++;
                        if ((x * x + (y + 1) * (y + 1)) > r * r) {
                                y ++;
                        }
                }
        }
}

void DrawArcCircle(window * windowe, int centerx, int centery, int r, int diametreInterne, float angleA, float angleB, gradiantColor *gradiant) {
        bool chiant = false;
        while (angleA < 0.) {
                angleA += M_PI * 2;
        }
        while (angleB < 0.) {
                angleB += M_PI * 2;
        }
        while (angleA > M_PI * 2) {
                angleA -= M_PI * 2;
        }
        while (angleB > M_PI * 2) {
                angleB -= M_PI * 2;
        }
        if (angleA == angleB) {
                DrawCircle(windowe, centerx, centery, r, diametreInterne);
                return;
        }
        if (angleB > angleA) {
                chiant = true;
        }
        int x = 0;
        int y = -r;
        while (x <= -y) {
                DrawLineHoleArc(windowe, centerx - x, centery + y, centerx + x, diametreInterne, centerx, centery, angleA, angleB, chiant, gradiant);
                DrawLineHoleArc(windowe, centerx - x, centery - y, centerx + x, diametreInterne, centerx, centery, angleA, angleB, chiant, gradiant);
                DrawLineHoleArc(windowe, centerx - y, centery + x, centerx + y, diametreInterne, centerx, centery, angleA, angleB, chiant, gradiant);
                DrawLineHoleArc(windowe, centerx - y, centery - x, centerx + y, diametreInterne, centerx, centery, angleA, angleB, chiant, gradiant);
                x ++;
                if ((x * x + (y + 1) * (y + 1)) > r * r) {
                        y ++;
                }
        }
}

void DrawLineHoleArc(window * windowe, int x1, int y, int x2, int distance, int xCercle, int yCercle, float angleA, float angleB, bool chiant, gradiantColor *gradiant) {
        int x = x1;
        if (y != yCercle) {
                while (x != x2) {
                        if (x1 > x2) {
                                x--;
                        } else {
                                x++;
                        }
                        float angle = SDL_atan2f((float) (yCercle - y), (float) (xCercle - x));
                        while (angle < 0.) {
                                angle += M_PI * 2;
                        }
                        while (angle > M_PI * 2) {
                                angle -= M_PI * 2;
                        }
                        if ((!chiant && angle < angleA && angle > angleB) || (chiant && ((angle < angleA && angle >= 0.) || (angle > angleB && angle <= M_PI * 2)))) {
                                if (SDL_abs((int) ((float) (yCercle - y) / SDL_sinf(angle))) > distance) {
                                        if (gradiant != NULL) {ChangeColorGradiant(windowe, angle, gradiant, chiant);}
                                        DrawPoint(windowe, x, y);
                                }
                        }
                }
        } else {
                if ((!chiant && 0. < angleA && 0. > angleB) || (chiant && (0. < angleA || 0. > angleB))) {
                        if (gradiant) {ChangeColorGradiant(windowe, 0, gradiant, chiant);}
                        DrawLine(windowe, x2, y, xCercle - distance - 1, y);
                }
                if ((!chiant && M_PI < angleA && M_PI > angleB) || (chiant && (M_PI < angleA || M_PI > angleB))) {
                        if (gradiant) {ChangeColorGradiant(windowe, M_PI, gradiant, chiant);}
                        DrawLine(windowe, xCercle + distance + 1, y, x1, y);
                }
        }
}

void DrawLineHole(window * windowe, int x1, int y, int x2, int distance, int xCercle, int yCercle) {
        int x = x1;
        if (y != yCercle) {
                while (x != x2) {
                        if (x1 > x2) {
                                x--;
                        } else {
                                x++;
                        }
                        float angle = SDL_atan2f((float) (yCercle - y), (float) (xCercle - x));
                        if (SDL_abs((int) ((float) (yCercle - y) / SDL_sinf(angle))) > distance) {
                                DrawPoint(windowe, x, y);
                        }
                }
        } else {
                DrawLine(windowe, x2, y, xCercle - distance - 1, y);
                DrawLine(windowe, xCercle + distance + 1, y, x1, y);
        }
}

void ChangeColorGradiant(window * windowe, float value, gradiantColor *gradiant, bool chiant) {
        float i;
        if (chiant) {
                if (value < gradiant->encadrement.end) {
                        value += M_PI * 2;
                }
                i = (value - gradiant->encadrement.start - M_PI * 2) / (gradiant->encadrement.end - gradiant->encadrement.start - M_PI * 2);
                i = -(i - 1);
        } else {
                i = (value - gradiant->encadrement.start) / (gradiant->encadrement.end - gradiant->encadrement.start);
        }
        if (i < 0.) {
                i = 0.;
        } else if (i > 1.) {
                i = 1.;
        }
        ChangeColor(windowe, interpolationUC(gradiant->r.start, gradiant->r.end, i), interpolationUC(gradiant->g.start, gradiant->g.end, i), interpolationUC(gradiant->b.start, gradiant->b.end, i), interpolationUC(gradiant->a.start, gradiant->a.end, i));
}
