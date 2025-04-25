#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "main.h"
#include "spellUI.h"
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
Pos sortCamera;
IPos exScreenPose;

int nbPrintf = 0;
int nbCollider = 0;

MouseCollideBox * aBouger = NULL;
MouseCollideBox * overedABouger = NULL;

char text[256];

CategoryType menuSelect = AUCUN;
category * categories;

color * noir;
color * gris;
color * grisClair;
color * blanc;
color * bleu;
color * vert;
color * rouge;

#define nbColliderMax 100
MouseCollideBox * collideBoxSpells[nbColliderMax];
Bouton * boutonSpellsCategory[6];
Bouton * boutonSpellsNoeud[56];
noeudModule * premierModule = NULL;

int main(int argc, char ** argv) {
	StartLog();
	initBouton();
	creatCategorie();

	sortCamera.x = 0;
	sortCamera.y = 0;

	for (int i = 0; i < nbColliderMax; i++) {
		collideBoxSpells[i] = NULL;
	}
	for (int i = 0; i < 6; i++) {
		boutonSpellsCategory[i] = NULL;
	}
	for (int i = 0; i < 56; i++) {
		boutonSpellsNoeud[i] = NULL;
	}

        noir = NewColor(0, 0, 0, 255);
        gris = NewColor(100, 100, 100, 255);
        grisClair = NewColor(150, 150, 150, 255);
        blanc = NewColor(200, 200, 200, 255);
        bleu = NewColor(0, 0, 200, 255);
        vert = NewColor(0, 200, 0, 255);
        rouge = NewColor(200, 0, 0, 255);

	int x = 0;
	int indexBouton = 0;
	category * currentCategorie = categories;
	while (currentCategorie != NULL) {
		NewBouton(boutonSpellsCategory, 6, collideBoxSpells, nbColliderMax, currentCategorie->name, 10, noir, blanc, vert, rouge, 10, x * 30 + 10, 120, 20, OveredBoutonCategorie, NotOveredBoutonCategorie, clickedBoutonCategorie, ReleaseBoutonCategorie, x, 0., 1.);
		nbCollider ++;
		for (int j = 0; j < currentCategorie->nbElement; j++) {
			NewBouton(boutonSpellsNoeud, 56, collideBoxSpells, nbColliderMax, currentCategorie->elements[j]->name, 10, noir, blanc, vert, rouge, 20, (j + x) * 30 + 40, 120, 20, OveredNoeud, NotOveredNoeud, ClickedNoeud, ReleaseNoeud, j * 10 + x, 0., 1.);
			boutonSpellsNoeud[indexBouton]->Draw = false;
			boutonSpellsNoeud[indexBouton]->collider.enabled = false;
			nbCollider ++;
			indexBouton ++;
		}
		x ++;
		currentCategorie = currentCategorie->suivant;
	}
	
	//FILE * spellSGL = OpenFile("spells/spell.json", "a"); if (spellSGL == NULL) { printf("%s\n", sqlLikeGetErreur()); }

	windoweGame = NewWindow("Game", -1, -1, 1920, 1080, 0, 0);
	windoweSpell = NewWindow("Spell", 0, 0, 960, 540, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_SKIP_TASKBAR, 0);
	
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
		} else if (aBouger != NULL) {
			aBouger->start.x += (mousePos.x - exMousePose.x); aBouger->start.y += (mousePos.y - exMousePose.y);
			aBouger->end.x += (mousePos.x - exMousePose.x); aBouger->end.y += (mousePos.y - exMousePose.y);
			exMousePose.x = mousePos.x; exMousePose.y = mousePos.y;
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
				for (int i = 0; i < nbColliderMax; i++) {
					if (collideBoxSpells[i] != NULL && collideBoxSpells[i]->enabled) {
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
						if (overedABouger != NULL) {
							aBouger = overedABouger;
							exScreenPose.x = overedABouger->start.x;
							exScreenPose.y = overedABouger->start.y;
						} else {
							SDL_GetWindowPosition(windoweSpell->window, &exScreenPose.x, &exScreenPose.y);
							moveScreen = true;
							for (int i = 0; i < nbColliderMax; i++) {
								if (collideBoxSpells[i] != NULL && collideBoxSpells[i]->enabled) {
									if (collideBoxSpells[i]->wasOver) {
										collideBoxSpells[i]->whenClick(collideBoxSpells[i]->ID);
									}
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
						aBouger = NULL;
						for (int i = 0; i < nbColliderMax; i++) {
							if (collideBoxSpells[i] != NULL && collideBoxSpells[i]->enabled) {
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

	for (int i = 0; i < 6; i++) {
		if (boutonSpellsCategory[i] != NULL) {
			free(boutonSpellsCategory[i]);
			boutonSpellsCategory[i] = NULL;
		}
	}

	for (int i = 0; i < 56; i++) {
		if (boutonSpellsNoeud[i] != NULL) {
			free(boutonSpellsNoeud[i]);
			boutonSpellsNoeud[i] = NULL;
		}
	}

	noeudModule * current = premierModule;
	noeudModule * next = NULL;
	while (current != NULL) {
		free(current->inputs);
		free(current->outputs);
		next = current->suivant;
		free(current);
		current = next;
	}

	freeCategory(categories);
        CloseWindows();
	EndLog();
        return 0;
}

void NewNoeudUI(noeud * noeu) {
	noeudModule * result = (noeudModule *) malloc(sizeof(noeudModule));
	result->name.text = noeu->name;
	result->name.size = 10;
	result->name.color = noir;
	result->name.angle = 0;
	result->name.pos.x = 70;
	result->name.pos.y = 10;
	
	unsigned char nbinputColor = (noeu->nbIn & 0b00000001);
	unsigned char nbinputPosition = ((noeu->nbIn & 0b00000010) >> 1);
	unsigned char nbinputFloat = ((noeu->nbIn & 0b00011100) >> 2);
	unsigned char nbinputInt = (noeu->nbIn >> 5);
	unsigned char nboutputColor = (noeu->nbout & 0b00000001);
	unsigned char nboutputPosition = ((noeu->nbout & 0b00000010) >> 1);
	unsigned char nboutputFloat = ((noeu->nbout & 0b00011100) >> 2);
	unsigned char nboutputInt = (noeu->nbout >> 5);
	unsigned char nbinput = nbinputColor + nbinputPosition + nbinputFloat + nbinputInt + 1;
	unsigned char nboutput = nboutputColor + nboutputPosition + nboutputFloat + nboutputInt + 1;

	result->collider.start.x = 0;
	result->collider.start.y = 0;
	result->collider.end.x = 260;
	result->collider.end.y = 60 + SDL_max(nbinput, nboutput) * 25;
	result->collider.ID = (uint64_t) &result->collider;
	result->collider.wasOver = false;
	result->collider.enabled = true;
	result->collider.whenOver = OveredNoeudModule;
	result->collider.whenNotOver = NotOveredNoeudModule;
	result->collider.whenClick = ClickedNoeudModule;
	result->collider.whenRelease = ReleaseNoeudModule;
	collideBoxSpells[nbCollider] = &result->collider;
	nbCollider ++;
	result->nbIn = nbinput;
	result->nbOut = nboutput;
	result->inputs = (noeudModuleIn *) malloc(sizeof(noeudModuleIn) * nbinput);
	for (int i = 0; i < nbinputInt; i++) {
		result->inputs[i].name.text = noeu->input[i];
		result->inputs[i].name.size = 10;
		result->inputs[i].name.color = noir;
		result->inputs[i].name.angle = 0;
		result->inputs[i].name.pos.x = 5;
		result->inputs[i].name.pos.y = 50 + i * 25;
		result->inputs[i].type = INT;
		result->inputs[i].nbout = 0;
	}
	for (int i = nbinputInt; i < nbinputFloat + nbinputInt; i++) {
		result->inputs[i].name.text = noeu->input[i];
		result->inputs[i].name.size = 10;
		result->inputs[i].name.color = noir;
		result->inputs[i].name.angle = 0;
		result->inputs[i].name.pos.x = 5;
		result->inputs[i].name.pos.y = 50 + (i + nbinputInt) * 25;
		result->inputs[i].type = FLOAT;
		result->inputs[i].nbout = 0;
	}
	for (int i = nbinputInt + nbinputFloat; i < nbinputFloat + nbinputInt + nbinputPosition; i++) {
		result->inputs[i].name.text = noeu->input[i];
		result->inputs[i].name.size = 10;
		result->inputs[i].name.color = noir;
		result->inputs[i].name.angle = 0;
		result->inputs[i].name.pos.x = 5;
		result->inputs[i].name.pos.y = 50 + (i + nbinputInt + nbinputFloat) * 25;
		result->inputs[i].type = POSITION;
		result->inputs[i].nbout = 0;
	}
	for (int i = nbinputInt + nbinputFloat + nbinputPosition; i < nbinputFloat + nbinputInt + nbinputPosition + nbinputColor; i++) {
		result->inputs[i].name.text = noeu->input[i];
		result->inputs[i].name.size = 10;
		result->inputs[i].name.color = noir;
		result->inputs[i].name.angle = 0;
		result->inputs[i].name.pos.x = 5;
		result->inputs[i].name.pos.y = 50 + (i + nbinputInt + nbinputFloat + nbinputPosition) * 25;
		result->inputs[i].type = COLOR;
		result->inputs[i].nbout = 0;
	}
	result->inputs[nbinput - 1].name.text = "Activer        \0";
	result->inputs[nbinput - 1].name.size = 10;
	result->inputs[nbinput - 1].name.color = noir;
	result->inputs[nbinput - 1].name.angle = 0;
	result->inputs[nbinput - 1].name.pos.x = 5;
	result->inputs[nbinput - 1].name.pos.y = 25 + (nbinput) * 25;
	result->inputs[nbinput - 1].type = BOOL;
	result->inputs[nbinput - 1].nbout = 0;
	result->outputs = (noeudModuleOut *) malloc(sizeof(noeudModuleOut) * nboutput);
	for (int i = 0; i < nboutputInt; i++) {
		result->outputs[i].name.text = noeu->output[i];
		result->outputs[i].name.size = 10;
		result->outputs[i].name.color = noir;
		result->outputs[i].name.angle = 0;
		result->outputs[i].name.pos.x = 135;
		result->outputs[i].name.pos.y = 50 + i * 25;
	}
	for (int i = nboutputInt; i < nboutputFloat + nboutputInt; i++) {
		result->outputs[i].name.text = noeu->output[i];
		result->outputs[i].name.size = 10;
		result->outputs[i].name.color = noir;
		result->outputs[i].name.angle = 0;
		result->outputs[i].name.pos.x = 135;
		result->outputs[i].name.pos.y = 50 + (i + nboutputInt) * 25;
	}
	for (int i = nboutputInt + nboutputFloat; i < nboutputFloat + nboutputInt + nboutputPosition; i++) {
		result->outputs[i].name.text = noeu->output[i];
		result->outputs[i].name.size = 10;
		result->outputs[i].name.color = noir;
		result->outputs[i].name.angle = 0;
		result->outputs[i].name.pos.x = 135;
		result->outputs[i].name.pos.y = 50 + (i + nboutputInt + nboutputFloat) * 25;
	}
	for (int i = nboutputInt + nboutputFloat + nboutputPosition; i < nboutputFloat + nboutputInt + nboutputPosition + nboutputColor; i++) {
		result->outputs[i].name.text = noeu->output[i];
		result->outputs[i].name.size = 10;
		result->outputs[i].name.color = noir;
		result->outputs[i].name.angle = 0;
		result->outputs[i].name.pos.x = 135;
		result->outputs[i].name.pos.y = 50 + (i + nboutputInt + nboutputFloat + nboutputPosition) * 25;
	}
	result->outputs[nboutput - 1].name.text = "Activer        \0";
	result->outputs[nboutput - 1].name.size = 10;
	result->outputs[nboutput - 1].name.color = noir;
	result->outputs[nboutput - 1].name.angle = 0;
	result->outputs[nboutput - 1].name.pos.x = 135;
	result->outputs[nboutput - 1].name.pos.y = 25 + (nboutput) * 25;
	result->suivant = NULL;
	if (premierModule == NULL) {
		premierModule = result;
	} else {
		noeudModule * current = premierModule;
		while (current->suivant != NULL) {
			current = current->suivant;
		}
		current->suivant = result;
	}
}

void creatCategorie() {
	noeud * listConst[16];
	listConst[0]  = NewNoeud("Color          \0", 0b10000000, 0b00000001, "Red            \0", "Green          \0", "Blue           \0", "Alpha          \0", "Color          \0");
	listConst[1]  = NewNoeud("Int            \0", 0b00000100, 0b00100000, "Float          \0", "Int            \0");
	listConst[2]  = NewNoeud("Float          \0", 0b00100000, 0b00000100, "Int            \0", "Float          \0");
	listConst[3]  = NewNoeud("Position       \0", 0b00001000, 0b00000010, "X              \0", "Y              \0", "Position       \0");
	listConst[4]  = NewNoeud("Random         \0", 0b00000000, 0b00000100, "Float          \0");
	categories = NewCategory("Constante      \0", 5, listConst);
	listConst[0]  = NewNoeud("Touche Player  \0", 0b00000000, 0b10000011, "PV             \0", "Mana           \0", "ID             \0", "ID Team        \0", "Position       \0", "Color          \0");
	listConst[1]  = NewNoeud("0Touche        \0", 0b00000000, 0b00100000, "Material       \0");
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
	listConst[8]  = NewNoeud("wait n tick    \0", 0b00100000, 0b00000000, "nb ticks       \0");
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
	listConst[0]  = NewNoeud("Pos  -> Float  \0", 0b00000010, 0b00001000, "Position       \0", "X              \0", "Y              \0");
	listConst[1]  = NewNoeud("Float->   Pos  \0", 0b00001000, 0b00000010, "X              \0", "Y              \0", "Position       \0");
	listConst[2]  = NewNoeud("Float->   Int  \0", 0b00000100, 0b00100000, "A Virgulr      \0", "Sans Virgule   \0");
	listConst[3]  = NewNoeud("Int  -> Float  \0", 0b00100000, 0b00000100, "Sans Virgule   \0", "A Virgule      \0");
	listConst[4]  = NewNoeud("Pos  -> Angle  \0", 0b00000010, 0b00101000, "Position       \0", "Distance       \0", "Yaw            \0", "Pitch          \0");
	listConst[5]  = NewNoeud("Angle->   Pos  \0", 0b00001000, 0b00000010, "Yaw            \0", "Pitch          \0", "Distance       \0");
	categories->suivant->suivant->suivant->suivant = NewCategory("Transform      \0", 6, listConst);
	listConst[0]  = NewNoeud("Aller en       \0", 0b00000010, 0b00000000, "Position       \0");
	listConst[1]  = NewNoeud("Vitesse Max    \0", 0b00100000, 0b00000000, "Vitesse Max    \0");
	listConst[2]  = NewNoeud("Target         \0", 0b00000010, 0b00000000, "Player Position\0");
	listConst[3]  = NewNoeud("Explosion      \0", 0b00000100, 0b00000000, "Intensité      \0");
	listConst[4]  = NewNoeud("Température    \0", 0b00100000, 0b00000000, "°C             \0");
	listConst[5]  = NewNoeud("Rayon Detection\0", 0b00001000, 0b00000000, "Rayon Player   \0", "Rayon Spell    \0");
	listConst[6]  = NewNoeud("Direction      \0", 0b00001000, 0b00000000, "Yaw            \0", "Pitch          \0");
	listConst[7]  = NewNoeud("Send Message   \0", 0b01000000, 0b00000000, "Spell ID       \0", "Message        \0");
	listConst[8]  = NewNoeud("Rayon          \0", 0b00000100, 0b00000000, "Rayon          \0");
	listConst[9]  = NewNoeud("Masse          \0", 0b00000100, 0b00000000, "Masse          \0");
	listConst[10] = NewNoeud("Tick Per s     \0", 0b00100000, 0b00000000, "Tick Per s     \0");
	listConst[11] = NewNoeud("Check Spell    \0", 0b00000000, 0b00100000, "nb Spell       \0");
	listConst[12] = NewNoeud("Check Player   \0", 0b00000000, 0b00000000, "nb Player      \0");
	listConst[13] = NewNoeud("Stock Value    \0", 0b01000111, 0b00000000, "index          \0", "Int            \0", "Float          \0", "Position       \0", "Color          \0");
	categories->suivant->suivant->suivant->suivant->suivant = NewCategory("Effet          \0", 14, listConst);
}

void draw() {
	noeudModule * current = premierModule;
	while (current != NULL) {
		ChangeColorC(windoweSpell, gris);
		DrawRectangle(windoweSpell, current->collider.start.x, current->collider.start.y, current->collider.end.x, current->collider.end.y, true, 0);
		ChangeColorC(windoweSpell, grisClair);
		DrawRectangle(windoweSpell, current->name.pos.x + current->collider.start.x - 10, current->name.pos.y + current->collider.start.y, current->name.pos.x + current->collider.start.x + 140, current->name.pos.y + current->collider.start.y + 20, true, 0);
		ChangeColorC(windoweSpell, current->name.color);
		DrawString(windoweSpell, current->name.pos.x + current->collider.start.x, current->name.pos.y + current->collider.start.y, current->name.text);
		for (int i = 0; i < current->nbIn; i++) {
			ChangeColorC(windoweSpell, grisClair);
			DrawRectangle(windoweSpell, current->inputs[i].name.pos.x + current->collider.start.x, current->inputs[i].name.pos.y + current->collider.start.y - 2, 120 + current->inputs[i].name.pos.x + current->collider.start.x, 20 + current->inputs[i].name.pos.y + current->collider.start.y, true, 0);
			ChangeColorC(windoweSpell, current->inputs[i].name.color);
			DrawString(windoweSpell, current->inputs[i].name.pos.x + current->collider.start.x, current->inputs[i].name.pos.y + current->collider.start.y, current->inputs[i].name.text);
		}
		for (int i = 0; i < current->nbOut; i++) {
			ChangeColorC(windoweSpell, grisClair);
			DrawRectangle(windoweSpell, current->outputs[i].name.pos.x + current->collider.start.x, current->outputs[i].name.pos.y + current->collider.start.y - 2, 120 + current->outputs[i].name.pos.x + current->collider.start.x, 20 + current->outputs[i].name.pos.y + current->collider.start.y, true, 0);
			ChangeColorC(windoweSpell, current->outputs[i].name.color);
			DrawString(windoweSpell, current->outputs[i].name.pos.x + current->collider.start.x, current->outputs[i].name.pos.y + current->collider.start.y, current->outputs[i].name.text);
		}

		current = current->suivant;
	}

	ChangeColorC(windoweSpell, gris);
	DrawRectangle(windoweSpell, 0, 0, 150, 1080, true, 0);

	for (int i = 0; i < 56; i++) {
		if (boutonSpellsNoeud[i] != NULL && boutonSpellsNoeud[i]->Draw) {
			ChangeColorC(windoweSpell, &boutonSpellsNoeud[i]->colorUse);
			DrawRectangle(windoweSpell, boutonSpellsNoeud[i]->collider.start.x, boutonSpellsNoeud[i]->collider.start.y, boutonSpellsNoeud[i]->collider.end.x, boutonSpellsNoeud[i]->collider.end.y, true, 0);
			ChangeColorC(windoweSpell, boutonSpellsNoeud[i]->text.color);
			DrawString(windoweSpell, boutonSpellsNoeud[i]->text.pos.x + boutonSpellsNoeud[i]->collider.start.x, boutonSpellsNoeud[i]->text.pos.y + boutonSpellsNoeud[i]->collider.start.y, boutonSpellsNoeud[i]->text.text);//, boutonSpellsCategory[i]->text.size, &boutonSpellsCategory[i]->text.color, 0);
		}
	}

	for (int i = 0; i < 6; i++) {
		if (boutonSpellsCategory[i] != NULL && boutonSpellsCategory[i]->Draw) {
			ChangeColorC(windoweSpell, &boutonSpellsCategory[i]->colorUse);
			DrawRectangle(windoweSpell, boutonSpellsCategory[i]->collider.start.x, boutonSpellsCategory[i]->collider.start.y, boutonSpellsCategory[i]->collider.end.x, boutonSpellsCategory[i]->collider.end.y, true, 0);
			ChangeColorC(windoweSpell, boutonSpellsCategory[i]->text.color);
			DrawString(windoweSpell, boutonSpellsCategory[i]->text.pos.x + boutonSpellsCategory[i]->collider.start.x, boutonSpellsCategory[i]->text.pos.y + boutonSpellsCategory[i]->collider.start.y, boutonSpellsCategory[i]->text.text);//, boutonSpellsCategory[i]->text.size, &boutonSpellsCategory[i]->text.color, 0);
		}
	}

        #if debug
        ChangeColorC(windoweSpell, rouge);
        for (int i = 0; i < nbColliderMax; i++) {
            if (collideBoxSpells[i] != NULL && collideBoxSpells[i]->enabled) {
                DrawRectangle(windoweSpell, collideBoxSpells[i]->start.x, collideBoxSpells[i]->start.y, collideBoxSpells[i]->end.x, collideBoxSpells[i]->end.y, false, 0);
            }
        }
        #endif

	ChangeColorC(windoweGame, noir); 
	RenderPresent(windoweGame);

	ChangeColorC(windoweSpell, noir); 
	RenderPresent(windoweSpell);
}

void clickedBoutonCategorie(uint64_t ID) {
	menuSelect = (CategoryType) ID;
	category * currentCategorie = categories;
	int index = 0;
	for (int i = 0; i < ID; i++) {
		if (currentCategorie->suivant != NULL) {
			index += currentCategorie->nbElement;
			currentCategorie = currentCategorie->suivant;
		}
	}
	for (int i = 0; i < 56; i++) {
		if (currentCategorie->nbElement + index > i && index <= i) {
			boutonSpellsNoeud[i]->Draw = true;
			boutonSpellsNoeud[i]->collider.enabled = true;
		} else {
			boutonSpellsNoeud[i]->Draw = false;
			boutonSpellsNoeud[i]->collider.enabled = false;
		}
	}
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->clicked;
	for (int i = 0; i < 6; i++) {
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

void ReleaseBoutonCategorie(uint64_t ID) {
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->over;
}

void OveredBoutonCategorie(uint64_t ID) {
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->over;
}

void NotOveredBoutonCategorie(uint64_t ID) {
	boutonSpellsCategory[ID]->colorUse = *boutonSpellsCategory[ID]->normal;
}

void ClickedNoeud(uint64_t ID) {
	category * currentCategorie = categories;
	int index = ID / 10;
	for (int i = 0; i < ID % 10; i++) {
		index += currentCategorie->nbElement;
		currentCategorie = currentCategorie->suivant;
	}
	if (boutonSpellsNoeud[index]->Draw) {
		NewNoeudUI(currentCategorie->elements[ID / 10]);
	}
}

void ReleaseNoeud(uint64_t ID) {
	category * currentCategorie = categories;
	int index = ID / 10;
	for (int i = 0; i < ID % 10; i++) {
		index += currentCategorie->nbElement;
		currentCategorie = currentCategorie->suivant;
	}
	boutonSpellsNoeud[index]->colorUse = *boutonSpellsNoeud[index]->over;
}

void OveredNoeud(uint64_t ID) {
	category * currentCategorie = categories;
	int index = ID / 10;
	for (int i = 0; i < ID % 10; i++) {
		index += currentCategorie->nbElement;
		currentCategorie = currentCategorie->suivant;
	}
	boutonSpellsNoeud[index]->colorUse = *boutonSpellsNoeud[index]->over;
}

void NotOveredNoeud(uint64_t ID) {
	category * currentCategorie = categories;
	int index = ID / 10;
	for (int i = 0; i < ID % 10; i++) {
		index += currentCategorie->nbElement;
		currentCategorie = currentCategorie->suivant;
	}
	boutonSpellsNoeud[index]->colorUse = *boutonSpellsNoeud[index]->normal;
}

void ClickedNoeudModule(uint64_t ID) {
	printf("Clicked ID = %d\n", ID);
}

void ReleaseNoeudModule(uint64_t ID) {
	printf("Release ID = %d\n", ID);
}

void OveredNoeudModule(uint64_t ID) {
	overedABouger = (MouseCollideBox *) ID;
}

void NotOveredNoeudModule(uint64_t ID) {
	if (overedABouger == (MouseCollideBox *) ID) {
		overedABouger = NULL;
	}
}

void Clicked(uint64_t ID) {
	printf("Clicked ID = %d\n", ID);
}

void Release(uint64_t ID) {
	printf("Release ID = %d\n", ID);
}

void Overed(uint64_t ID) {
	printf("Over ID = %d\n", ID);
}

void NotOvered(uint64_t ID) {
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
