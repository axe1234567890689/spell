#include <stdio.h>

#include "window.h"
#include "bouton.h"

Bouton * bouttons[10];

void initBouton() {
    for (int i = 0; i < 10; i++) {
        bouttons[i] = NULL;
    }
}

void clickedBoutonCategorie(int ID) {
	switch (ID)
	{
	case CONSTANT:
		/* code */
		break;
		
	case DECLENCHEUR:
		/* code */
		break;

	case CONDITION:
		/* code */
		break;

	case INPUT:
		/* code */
		break;

	case TRANSFORM:
		/* code */
		break;

	case EFFET:
		/* code */
		break;
		
	default:
		break;
	}
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