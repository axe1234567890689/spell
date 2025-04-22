#include <stdio.h>

#include "window.h"
#include "bouton.h"

Bouton * bouttons[10];

void initBouton() {
    for (int i = 0; i < 10; i++) {
        bouttons[i] = NULL;
    }
}

Bouton * NewBouton(Bouton * BoutonArray[], int nbBoutoninArray, MouseCollideBox * ColliderArray[], int nbColliderinArray, char * text, int TextSize, color * TextColor, color * normalColor, color * overColor, color * clickedColor, float x, float y, float w, float h, void (*whenOver)(int ID), void (*whenNotOver)(int ID), void (*whenClick)(int ID), void (*whenRelease)(int ID), int ID, float animMin, float animMax) {
    Bouton * result = (Bouton *) malloc(sizeof(Bouton));

	result->collider.start.x = x;
	result->collider.start.y = y;
	result->collider.end.x = x + w;
	result->collider.end.y = y + h;
	result->collider.wasOver = false;
	result->collider.whenOver = whenOver;
	result->collider.whenNotOver = whenNotOver;
	result->collider.whenRelease = whenRelease;
	result->collider.whenClick = whenClick;
	result->collider.ID = ID;

    result->text.text = text;
    result->text.size = TextSize;
    result->text.color = TextColor;
    result->text.pos.x = 0;
    result->text.pos.y = h / 2 - TextSize / 2;
    result->text.angle = 0.0f;

    result->normal = normalColor;
    result->over = overColor;
    result->clicked = clickedColor;
    result->colorUse = *normalColor;

    result->Draw = true;

    result->anim.i = 0.0f;
    result->anim.min = 0.0f;
    result->anim.max = 1.0f;
    result->anim.animated = result->anim.min;

    result->anim.nbModule = 0;
    result->anim.modules = NULL;

	for (int i = 0; i < nbBoutoninArray; i++) {
		if (BoutonArray[i] == NULL) {
			BoutonArray[i] = result;
            break;
		}
        if (i == nbBoutoninArray - 1) {
            free(result);
            return NULL;
        }
	}

	for (int i = 0; i < nbColliderinArray; i++) {
		if (ColliderArray[i] == NULL) {
			ColliderArray[i] = (MouseCollideBox *) result;
			return result;
		}
	}
    free(result);
    return NULL;
}

float calculi(float a, float min, float max) {
    return (a - min) / (max - min);
}

MouseCollideBox * newCollideBox(MouseCollideBox * ColliderArray[], int nbColliderinArray, float x, float y, float w, float h, void (*whenOver)(int ID), void (*whenNotOver)(int ID), void (*whenClick)(int ID), void (*whenRelease)(int ID), int ID) {
	MouseCollideBox * result = (MouseCollideBox *) malloc(sizeof(MouseCollideBox));
	result->start.x = x;
	result->start.y = y;
	result->end.x = x + w;
	result->end.y = y + h;
	result->wasOver = false;
	result->whenOver = whenOver;
	result->whenNotOver = whenNotOver;
	result->whenRelease = whenRelease;
	result->whenClick = whenClick;
	result->ID = ID;

	for (int i = 0; i < nbColliderinArray; i++) {
		if (ColliderArray[i] == NULL) {
			ColliderArray[i] = result;
			return result;
		}
	}
	free(result);
	return NULL;
}