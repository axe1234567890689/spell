#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "main.h"
#include "window.h"
#include "file.h"
#include "bouton.h"
#include "log.h"

#define log 0
#define debug 1

#if log
#define LOG(fmt, ...) \
	do { snprintf(text, 256, fmt, ##__VA_ARGS__); WriteLogln(text); } while (0)
#else
#define LOG(fmt, ...) \
	do { } while (0)
#endif

//cd C:\Users\axel\Desktop\c
//chcp 65001
//gcc src\test\spell\*.c src\test\spell\*.h -o bin\spell -lws2_32 -I include -L lib -lmingw32 -lSDL2main -lSDL2_test -lSDL2
//printf("ligne : %d\ndans le fichier : %s\n", __LINE__, __FILE__);

window * windoweGame;
window * windoweSpell;

bool game = false;
bool moveScreen = false;

Pos camera;
Pos mousePos;
Pos exMousePose;
IPos exScreenPose;

int nbPrintf = 0;
int nbCollider = 0;

char text[256];

CategoryType menuSelect = AUCUN;
category * categories;

color * noir;
color * blanc;
color * bleu;
color * vert;
color * rouge;

MouseCollideBox * collideBoxSpells[10];
Bouton * boutonSpellsCategory[6];

int main(int argc, char ** argv) {
	StartLog();
	initBouton();

	for (int i = 0; i < 10; i++) {
		collideBoxSpells[i] = NULL;
		boutonSpellsCategory[i] = NULL;
	}

        noir = NewColor(0, 0, 0, 255);
        blanc = NewColor(200, 200, 200, 255);
        bleu = NewColor(0, 0, 200, 255);
        vert = NewColor(0, 200, 0, 255);
        rouge = NewColor(200, 0, 0, 255);

	NewBouton(boutonSpellsCategory, 10, collideBoxSpells, 10, "Constante      \0", 10, noir, blanc, vert, rouge, 10, 10, 110, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, 0, 0., 1.);
	NewBouton(boutonSpellsCategory, 10, collideBoxSpells, 10, "Déclencheur    \0", 10, noir, blanc, vert, rouge, 10, 40, 110, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, 1, 0., 1.);
	NewBouton(boutonSpellsCategory, 10, collideBoxSpells, 10, "Condition      \0", 10, noir, blanc, vert, rouge, 10, 70, 110, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, 2, 0., 1.);
	NewBouton(boutonSpellsCategory, 10, collideBoxSpells, 10, "Input          \0", 10, noir, blanc, vert, rouge, 10, 100, 110, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, 3, 0., 1.);
	NewBouton(boutonSpellsCategory, 10, collideBoxSpells, 10, "Transform      \0", 10, noir, blanc, vert, rouge, 10, 130, 110, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, 4, 0., 1.);
	NewBouton(boutonSpellsCategory, 10, collideBoxSpells, 10, "Effet          \0", 10, noir, blanc, vert, rouge, 10, 160, 110, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, 5, 0., 1.);

	//FILE * spellSGL = OpenFile("spells/spell.json", "a"); if (spellSGL == NULL) { printf("%s\n", sqlLikeGetErreur()); }
	//listConst[] = NewNoeud("               \0", 0b000 000 0 0, 0b000 000 0 0, "               \0", "               \0", "               \0", "               \0", "               \0");

	noeud * listConst[16];
	listConst[0]  = NewNoeud("Color          \0", 0b10000000, 0b00000001, "Red            \0", "Green          \0", "Blue           \0", "Alpha          \0", "Color          \0");
	listConst[1]  = NewNoeud("Int            \0", 0b00000100, 0b00100000, "Float          \0", "Int            \0");
	listConst[2]  = NewNoeud("Float          \0", 0b00100000, 0b00000100, "Int            \0", "Float          \0");
	listConst[3]  = NewNoeud("Position       \0", 0b00001000, 0b00000010, "X              \0", "Y              \0", "Position       \0");
	listConst[4]  = NewNoeud("Random         \0", 0b00000000, 0b00000100, "Float          \0");
	categories = NewCategory("Constante      \0", 5, listConst);
	listConst[0]  = NewNoeud("Touche Player  \0", 0b00000000, 0b10000011, "PV             \0", "Mana           \0", "ID             \0", "ID Team        \0", "Position       \0", "Color          \0");
	listConst[1]  = NewNoeud("0Touche         \0", 0b00000000, 0b00100000, "Material       \0");
	listConst[2]  = NewNoeud("Position       \0", 0b00000000, 0b00000000);
	listConst[3]  = NewNoeud("Distance       \0", 0b00100000, 0b00000000, "Distance Max   \0");
	listConst[4]  = NewNoeud("Mana Lost      \0", 0b00000100, 0b00000100, "Mana Limit     \0", "Mana Remaning  \0");
	listConst[5]  = NewNoeud("mana Stored    \0", 0b00000100, 0b00000100, "Mana Déclencher\0", "Mana Stored    \0");
	listConst[6]  = NewNoeud("New Color      \0", 0b00000000, 0b00000001, "Color          \0");
	categories->suivant = NewCategory("Déclencheur    \0", 7, listConst);
	listConst[0]  = NewNoeud("==             \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[1]  = NewNoeud("<              \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[2]  = NewNoeud(">              \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[3]  = NewNoeud("<=             \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[4]  = NewNoeud(">=             \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[5]  = NewNoeud("inverte        \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[6]  = NewNoeud("et             \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[7]  = NewNoeud("ou             \0", 0b01000000, 0b00100000, "valeur1        \0", "valeur2        \0", "resultat       \0");
	listConst[8]  = NewNoeud("wait n    tick \0", 0b00100000, 0b00000000, "nb ticks       \0");
	categories->suivant->suivant = NewCategory("Condition      \0", 9, listConst);
	listConst[0]  = NewNoeud("Position       \0", 0b00000000, 0b00000010, "Position       \0");
	listConst[1]  = NewNoeud("Color          \0", 0b00000000, 0b00000001, "Color          \0");
	listConst[2]  = NewNoeud("Vitesse        \0", 0b00000000, 0b00000100, "Vitesse        \0");
	listConst[3]  = NewNoeud("Acceleration   \0", 0b00000000, 0b00000100, "Acceleration   \0");
	listConst[4]  = NewNoeud("Direction      \0", 0b00000000, 0b00001000, "Yaw            \0", "Pitch          \0");
	listConst[5]  = NewNoeud("Player Around  \0", 0b01000000, 0b00100000, "Next Player    \0", "Player id      \0");
	listConst[6]  = NewNoeud("Get Player     \0", 0b00100000, 0b00100000, "Player id      \0", "Player id      \0");
	listConst[7]  = NewNoeud("Caster         \0", 0b00000000, 0b00100000, "Player id      \0");
	listConst[8]  = NewNoeud("Attention?     \0", 0b00000000, 0b00000000);
	listConst[9]  = NewNoeud("Spell Around   \0", 0b01000000, 0b00100000, "Check Spell    \0", "Next Spell     \0", "Spell id       \0");
	listConst[10] = NewNoeud("Angle To       \0", 0b00000010, 0b00001000, "Position To    \0", "Yaw            \0", "Pitch          \0");
	listConst[11] = NewNoeud("Donne?         \0", 0b01000000, 0b00000000, "Data           \0", "ID             \0", "?              \0");
	listConst[12] = NewNoeud("Get Messages   \0", 0b00100000, 0b00100000, "Next Message   \0", "Message        \0");
	listConst[13] = NewNoeud("Get Value      \0", 0b00100000, 0b00100111, "Index          \0", "Int            \0", "Float          \0", "Position       \0", "Color          \0");
	listConst[14] = NewNoeud("ID             \0", 0b00000000, 0b00100000, "ID             \0");
	categories->suivant->suivant->suivant = NewCategory("Input          \0", 15, listConst);
	listConst[0]  = NewNoeud("Pos   ->  Float\0", 0b00000010, 0b00001000, "Position       \0", "X              \0", "Y              \0");
	listConst[1]  = NewNoeud("Float ->    Pos\0", 0b00001000, 0b00000010, "X              \0", "Y              \0", "Position       \0");
	listConst[2]  = NewNoeud("Float ->    Int\0", 0b00000100, 0b00100000, "A Virgulr      \0", "Sans Virgule   \0");
	listConst[3]  = NewNoeud("Int   ->  Float\0", 0b00100000, 0b00000100, "Sans Virgule   \0", "A Virgule      \0");
	listConst[4]  = NewNoeud("Pos   ->  Angle\0", 0b00000010, 0b00101000, "Position       \0", "Distance       \0", "Yaw            \0", "Pitch          \0");
	listConst[5]  = NewNoeud("Angle ->    Pos\0", 0b00001000, 0b00000010, "Yaw            \0", "Pitch          \0", "Distance       \0");
	categories->suivant->suivant->suivant->suivant = NewCategory("Transform      \0", 6, listConst);
	listConst[0]  = NewNoeud("Aller en       \0", 0b00000010, 0b00000000, "Position       \0");
	listConst[1]  = NewNoeud("Vitesse Max    \0", 0b00100000, 0b00000000, "Vitesse Max    \0");
	listConst[2]  = NewNoeud("Target         \0", 0b00000010, 0b00000000, "Player Position\0");
	listConst[3]  = NewNoeud("Explosion      \0", 0b00000100, 0b00000000, "Intensité      \0");
	listConst[4]  = NewNoeud("Température    \0", 0b00100000, 0b00000000, "°C             \0");
	listConst[5]  = NewNoeud("Rayon Détection\0", 0b00001000, 0b00000000, "Rayon Player   \0", "Rayon Spell    \0");
	listConst[6]  = NewNoeud("Direction      \0", 0b00001000, 0b00000000, "Yaw            \0", "Pitch          \0");
	listConst[7]  = NewNoeud("Send Message   \0", 0b01000000, 0b00000000, "Spell ID       \0", "Message        \0");
	listConst[8]  = NewNoeud("Rayon          \0", 0b00000100, 0b00000000, "Rayon          \0");
	listConst[9]  = NewNoeud("Masse          \0", 0b00000100, 0b00000000, "Masse          \0");
	listConst[10] = NewNoeud("Tick Per s     \0", 0b00100000, 0b00000000, "Tick Per s     \0");
	listConst[11] = NewNoeud("Check Spell    \0", 0b00000000, 0b00100000, "nb Spell       \0");
	listConst[12] = NewNoeud("Check Player   \0", 0b00000000, 0b00000000, "nb Player      \0");
	listConst[13] = NewNoeud("Stock Value    \0", 0b01000111, 0b00000000, "index          \0", "Int            \0", "Float          \0", "Position       \0", "Color          \0");
	categories->suivant->suivant->suivant->suivant = NewCategory("Effet          \0", 14, listConst);

	windoweGame = NewWindow("Game", -1, -1, 1920, 1080, 0, 0);
	windoweSpell = NewWindow("Spell", 0, 0, 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR, 0);
	SDL_HideWindow(windoweGame->window);
	
	mousePos.x = -1; mousePos.y = -1;
	camera.x = 0; camera.y = 0;

        bool Jeux = true;

        // gestion du temps
        float deltatime = 0;
        unsigned long lastTicks = SDL_GetTicks();
        unsigned long lastDeltaTicks = 0;

        // gestion des FPS
        bool needToDraw = true;
        bool FPSLimit = true;
        unsigned char nbFPS = 60;
        int timeForNextFrame = 0;
        int timeBetweenFrame = (1. / (float) nbFPS) * 1000;

	printfASuppr();

        InitWindow();

        while (Jeux) {
        	lastDeltaTicks = SDL_GetTicks();
        	lastDeltaTicks -= lastTicks;
        	deltatime = lastDeltaTicks / 1000.;
        	lastTicks += lastDeltaTicks;

        	if (needToDraw) {
        		if (FPSLimit) {
        			timeForNextFrame += lastDeltaTicks;
        			if (timeForNextFrame >= timeBetweenFrame) {
        				draw();

        				timeForNextFrame -= timeBetweenFrame;
        			}
        		} else {
        			draw();
        		}
        		//needToDraw = false;
        	}

        	SDL_Event events;

		if (moveScreen) {
			exScreenPose.x = exScreenPose.x + (mousePos.x - exMousePose.x); exScreenPose.y = exScreenPose.y + (mousePos.y - exMousePose.y);
			SDL_SetWindowPosition(windoweSpell->window, exScreenPose.x, exScreenPose.y);
		}
        
        	while (SDL_PollEvent(&events)) {
        	        switch (events.type)
        	        {
        	        case SDL_QUIT:
				Jeux = false;
        		        break;

			case SDL_WINDOWEVENT:
				if (events.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					if (events.window.windowID == windoweSpell->id) {
						SDL_GetWindowSize(windoweSpell->window, &windoweSpell->w, &windoweSpell->h);
					}
				} else if (events.window.event == SDL_WINDOWEVENT_CLOSE) {
					if (events.window.windowID == windoweSpell->id) {
						SDL_HideWindow(windoweSpell->window);
					} else if (events.window.windowID == windoweGame->id) {
						Jeux = false;
					}
				}
				break;

        	        case SDL_MOUSEMOTION:
        		        mousePos.x = events.motion.x;
        		        mousePos.y = events.motion.y;
				for (int i = 0; i < 10; i++) {
					if (collideBoxSpells[i] != NULL) {
						if (collideBoxSpells[i]->start.x <= mousePos.x && collideBoxSpells[i]->end.x >= mousePos.x && collideBoxSpells[i]->start.y <= mousePos.y && collideBoxSpells[i]->end.y >= mousePos.y) {
							if (!collideBoxSpells[i]->wasOver) {
								collideBoxSpells[i]->wasOver = true;
								collideBoxSpells[i]->whenOver(collideBoxSpells[i]->ID);
							}
						} else {
							if (collideBoxSpells[i]->wasOver) {
								collideBoxSpells[i]->wasOver = false;
								collideBoxSpells[i]->whenNotOver(collideBoxSpells[i]->ID);
							}
						}
					}
				}
				needToDraw = true;
        		        break;
			
			case SDL_MOUSEBUTTONDOWN:
				if (events.window.windowID == windoweGame->id) {
				} else if (events.window.windowID == windoweSpell->id) {
					if (events.button.button == 1) {
						exMousePose = mousePos;
						SDL_GetWindowPosition(windoweSpell->window, &exScreenPose.x, &exScreenPose.y);
						moveScreen = true;
						for (int i = 0; i < 10; i++) {
							if (collideBoxSpells[i] != NULL) {
								if (collideBoxSpells[i]->wasOver) {
									collideBoxSpells[i]->whenClick(collideBoxSpells[i]->ID);
								}
							}
						}
					}
				}
				break;
		
			case SDL_MOUSEBUTTONUP:
				if (events.window.windowID == windoweGame->id) {
				} else if (events.window.windowID == windoweSpell->id) {
					if (events.button.button == 1) {
						moveScreen = false;
						for (int i = 0; i < 10; i++) {
							if (collideBoxSpells[i] != NULL) {
								if (collideBoxSpells[i]->wasOver) {
									collideBoxSpells[i]->whenRelease(collideBoxSpells[i]->ID);
								}
							}
						}
					}
				}
				break;

			case SDL_KEYDOWN:
				if (events.key.keysym.sym == SDLK_ESCAPE) {
					Jeux = false;
					break;
				} else if (events.key.keysym.sym == SDLK_SPACE) {
					SDL_ShowWindow(windoweSpell->window);
					break;
				}
				break;
                
        	        default:
        		        break;
        	        }
        	}
        }

	for (int i = 0; i < 10; i++) {
		if (collideBoxSpells[i] != NULL) {
			free(collideBoxSpells[i]);
			collideBoxSpells[i] = NULL;
		}
	}

	freeCategory(categories);
        CloseWindows();
	EndLog();
        return 0;
}

void draw() {
	ChangeColorC(windoweSpell, blanc);

	for (int i = 0; i < 10; i++) {
		if (boutonSpellsCategory[i] != NULL && boutonSpellsCategory[i]->Draw) {
			ChangeColorC(windoweSpell, &boutonSpellsCategory[i]->colorUse);
			DrawRectangle(windoweSpell, boutonSpellsCategory[i]->collider.start.x, boutonSpellsCategory[i]->collider.start.y, boutonSpellsCategory[i]->collider.end.x, boutonSpellsCategory[i]->collider.end.y, true, 0);
			ChangeColorC(windoweSpell, boutonSpellsCategory[i]->text.color);
			DrawString(windoweSpell, boutonSpellsCategory[i]->text.pos.x + boutonSpellsCategory[i]->collider.start.x, boutonSpellsCategory[i]->text.pos.y + boutonSpellsCategory[i]->collider.start.y, boutonSpellsCategory[i]->text.text);//, boutonSpellsCategory[i]->text.size, &boutonSpellsCategory[i]->text.color, 0);
		}
	}

        #if debug
        ChangeColorC(windoweSpell, rouge);
        for (int i = 0; i < 10; i++) {
            if (collideBoxSpells[i] != NULL) {
                DrawRectangle(windoweSpell, collideBoxSpells[i]->start.x, collideBoxSpells[i]->start.y, collideBoxSpells[i]->end.x, collideBoxSpells[i]->end.y, false, 0);
            }
        }
        #endif

	ChangeColorC(windoweGame, noir); 
	RenderPresent(windoweGame);

	ChangeColorC(windoweSpell, noir); 
	RenderPresent(windoweSpell);
}

void clickedBoutonCategorie(int ID) {
	menuSelect = (CategoryType) ID;
	category * currentCategorie = categories;
	for (int i = 0; i < ID; i++) {
		if (currentCategorie->suivant != NULL) {
			currentCategorie = currentCategorie->suivant;
		}
	}
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->clicked;
	for (int i = 0; i < 10; i++) {
		if (boutonSpellsCategory[i] != NULL) {
			if (i > ID) {
				int c = (i + currentCategorie->nbElement) * 30 + 10;
				boutonSpellsCategory[i]->collider.start.y = c;
				boutonSpellsCategory[i]->collider.end.y = c + 20;
			} else {
				int c = (i) * 30 + 10;
				boutonSpellsCategory[i]->collider.start.y = c;
				boutonSpellsCategory[i]->collider.end.y = c + 20;
			}
		}
	}
}

void ReleaseBoutonCategorie(int ID) {
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->over;
}

void OveredBoutonCategorie(int ID) {
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->over;
}

void NotOveredBoutonCategorie(int ID) {
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->normal;
}

void Clicked(int ID) {
	printf("Clicked ID = %d\n", ID);
}

void Release(int ID) {
	printf("Release ID = %d\n", ID);
}

void Overed(int ID) {
	printf("Over ID = %d\n", ID);
}

void NotOvered(int ID) {
	printf("NotOver ID = %d\n", ID);
}

noeud * NewNoeud(const char name[16], unsigned char nbIn, unsigned char nbout, ...) {
	va_list args;
	va_start(args, nbout);

	noeud * result = (noeud *) malloc(sizeof(noeud));
	LOG("%08X \0", result);
	strncpy(result->name, name, 15);
	result->name[15] = '\0';
	result->nbIn = nbIn;
	result->nbout = nbout;
	unsigned char nbinput = ((nbIn & 0b00000001) + ((nbIn & 0b00000010) >> 1) + ((nbIn & 0b00011100) >> 2) + (nbIn >> 5));
	unsigned char nboutput = ((nbout & 0b00000001) + ((nbout & 0b00000010) >> 1) + ((nbout & 0b00011100) >> 2) + (nbout >> 5));
	result->input = (char **) malloc(nbinput * sizeof(char *));
	result->output = (char **) malloc(nboutput * sizeof(char *));
	char * str;
	for (int i = 0; i < nbinput; i++) {
		str = va_arg(args, char*);
		result->input[i] = (char *) malloc(16);
		strncpy(result->input[i], str, 15);
		result->input[i][15] = '\0';
	}
	for (int i = 0; i < nboutput; i++) {
		str = va_arg(args, char*);
		result->output[i] = (char *) malloc(16);
		strncpy(result->output[i], str, 15);
		result->output[i][15] = '\0';
	}

	va_end(args);
	LOG("──────────────────────────────────────────────────────────────────── \0");
	LOG("ligne : %d | dans le fichier : %s \0", __LINE__, __FILE__);
	LOG("name : %s \0", result->name);
	LOG("nbIn : %02X \0", result->nbIn);
	LOG("nbOut : %02X \0", result->nbout);
	LOG("nbIn = %d %d %d %d \0", result->nbIn >> 5, (result->nbIn & 0b00011100) >> 2, (result->nbIn & 0b00000010) >> 1, result->nbIn & 0b00000001);
	LOG("nbout = %d %d %d %d \0", result->nbout >> 5, (result->nbout & 0b00011100) >> 2, (result->nbout & 0b00000010) >> 1, result->nbout & 0b00000001);
	LOG("──────────────────────────────────────────────────────────────────── \0");
	return result;
}

category * NewCategory(const char name[16], Uint32 nbElement, noeud * Elements[]) {
	category * result = (category *) malloc(sizeof(category) + nbElement * sizeof(noeud *));
	LOG("┌────────────────────────────────────────────────────────────────────┐ \0");
	LOG("│                              %08X                              │ \0", result);
	LOG("└────────────────────────────────────────────────────────────────────┘ \0");
	strncpy(result->name, name, 16);
	result->nbElement = nbElement;
	for (int i = 0; i < nbElement; i++) {
		result->elements[i] = Elements[i];
	}
	result->suivant = NULL;
	return result;
}

void freeCategory(category * categories) {
	category * current = categories;
	category * asuppr = NULL;
	while (current != NULL) {
		for (int i = 0; i < current->nbElement; i++) {
			freeNoeud(current->elements[i]);
		}
		asuppr = current;
		current = current->suivant;
		free(asuppr);
	}
}

void freeNoeud(noeud * current) {
	unsigned char nbinput = ((current->nbIn & 0b00000001) + ((current->nbIn & 0b00000010) >> 1) + ((current->nbIn & 0b00011100) >> 2) + (current->nbIn >> 5));
	unsigned char nboutput = ((current->nbout & 0b00000001) + ((current->nbout & 0b00000010) >> 1) + ((current->nbout & 0b00011100) >> 2) + (current->nbout >> 5));
	for (int i = 0; i < nbinput; i++) {
		free(current->input[i]);
	}
	free(current->input);
	for (int i = 0; i < nboutput; i++) {
		free(current->output[i]);
	}
	free(current->output);
	free(current);
}

void printCategorie(category * categories) {
	category * current = categories;
	while (current != NULL) {
		printff("\n");
		snprintf(text, 128, "┌─%s\n\0", current->name);
		printff(text);
		snprintf(text, 128, "├─ %d noeuds\n\0", current->nbElement);
		printff(text);
		for (int i = 0; i < current->nbElement; i++) {
			if (current->elements[i] == NULL) {
				LOG("current->elements[%d] est NULL\n\0", i);
			} else {
				printNoeud(current->elements[i]);
			}
		}
		snprintf(text, 128, "└───────────────────\n\0");
		printff(text);
		current = current->suivant;
	}
}

void printNoeud(noeud * current) {
	LOG("current : %08X \000", current);
	logstr(current->name);
	//LOG("name : %s \0", current->name);
	LOG("nbIn : %02X \0", current->nbIn);
	LOG("nbOut : %02X \0", current->nbout);
	LOG("nbIn = %d %d %d %d \0", current->nbIn >> 5, (current->nbIn & 0b00011100) >> 2, (current->nbIn & 0b00000010) >> 1, current->nbIn & 0b00000001);
	LOG("nbout = %d %d %d %d \0", current->nbout >> 5, (current->nbout & 0b00011100) >> 2, (current->nbout & 0b00000010) >> 1, current->nbout & 0b00000001);
	unsigned char nbinputColor = (current->nbIn & 0b00000001);
	unsigned char nbinputPosition = ((current->nbIn & 0b00000010) >> 1);
	unsigned char nbinputFloat = ((current->nbIn & 0b00011100) >> 2);
	unsigned char nbinputInt = (current->nbIn >> 5);
	unsigned char nboutputColor = (current->nbout & 0b00000001);
	unsigned char nboutputPosition = ((current->nbout & 0b00000010) >> 1);
	unsigned char nboutputFloat = ((current->nbout & 0b00011100) >> 2);
	unsigned char nboutputInt = (current->nbout >> 5);
	snprintf(text, 128, "├┬─%s\n", current->name);
	printff(text);
	snprintf(text, 128, "│├┬─%d input\n", nbinputColor + nbinputPosition + nbinputFloat + nbinputInt);
	printff(text);
	if (nbinputColor + nbinputPosition + nbinputFloat + nbinputInt != 0) {
		if (nbinputInt != 0) {
			snprintf(text, 128, "││├┬─%d Int\n", nbinputInt);
			printff(text);
			for (int i = 0; i < nbinputInt; i++) {
				logstr(current->input[i]);
				snprintf(text, 128, (i == nbinputInt - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->input[i]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
		if (nbinputFloat != 0) {
			snprintf(text, 128, "││├┬─%d Float\n", nbinputFloat);
			printff(text);
			for (int i = 0; i < nbinputFloat; i++) {
				logstr(current->input[i]);
				snprintf(text, 128, (i == nbinputFloat - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->input[i + nbinputInt]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
		if (nbinputPosition != 0) {
			snprintf(text, 128, "││├┬─%d Position\n", nbinputPosition);
			printff(text);
			for (int i = 0; i < nbinputPosition; i++) {
				logstr(current->input[i]);
				snprintf(text, 128, (i == nbinputPosition - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->input[i + nbinputInt + nbinputFloat]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
		if (nbinputColor != 0) {
			snprintf(text, 128, "││├┬─%d Couleur\n", nbinputColor);
			printff(text);
			for (int i = 0; i < nbinputColor; i++) {
				logstr(current->input[i]);
				snprintf(text, 128, (i == nbinputColor - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->input[i + nbinputInt + nbinputFloat + nbinputPosition]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
	}
	snprintf(text, 128, "││└─────────────────\n");
	printff(text);
	snprintf(text, 128, "││\n");
	printff(text);
	snprintf(text, 128, "│├┬─%d output\n", nboutputColor + nboutputPosition + nboutputFloat + nboutputInt);
	printff(text);
	if (nboutputColor + nboutputPosition + nboutputFloat + nboutputInt != 0) {
		if (nboutputInt != 0) {
			snprintf(text, 128, "││├┬─%d Int\n", nboutputInt);
			printff(text);
			for (int i = 0; i < nboutputInt; i++) {
				logstr(current->output[i]);
				snprintf(text, 128, (i == nboutputInt - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->output[i]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
		if (nboutputFloat != 0) {
			snprintf(text, 128, "││├┬─%d Float\n", nboutputFloat);
			printff(text);
			for (int i = 0; i < nboutputFloat; i++) {
				logstr(current->output[i + nboutputInt]);
				snprintf(text, 128, (i == nboutputFloat - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->output[i + nboutputInt]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
		if (nboutputPosition != 0) {
			snprintf(text, 128, "││├┬─%d Position\n", nboutputPosition);
			printff(text);
			for (int i = 0; i < nboutputPosition; i++) {
				logstr(current->output[i + nboutputInt + nboutputFloat]);
				snprintf(text, 128, (i == nboutputPosition - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->output[i + nboutputInt + nboutputFloat]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
		if (nboutputColor != 0) {
			snprintf(text, 128, "││├┬─%d Couleur\n", nboutputColor);
			printff(text);
			for (int i = 0; i < nboutputColor; i++) {
				logstr(current->output[i + nboutputInt + nboutputFloat + nboutputPosition]);
				snprintf(text, 128, (i == nboutputColor - 1) ? "│││└─ %s\n" : "│││├─ %s\n", current->output[i + nboutputInt + nboutputFloat + nboutputPosition]);
				printff(text);
			}
			snprintf(text, 128, "│││\n");
			printff(text);
		}
	}
	snprintf(text, 128, "││└─────────────────\n");
	printff(text);
	snprintf(text, 128, "││\n");
	printff(text);
	snprintf(text, 128, "│└──────────────────\n");
	printff(text);
	snprintf(text, 128, "│\n"); 
	printff(text);
}

void printff(const char * texte) {
	nbPrintf++;
	printf("%03i │ %s", nbPrintf, texte);
}

void logstr(const char * texte) {
	if (texte == NULL) {
		LOG("text est NULL \0");
		return;
	} else {
		LOG("text : %08X \0", texte);
		LOG("text : %s \0", texte);
	}
}