#include "base/graphics/sprite_data.h"

// defines the title screen process used
#ifndef TITLE_SCREEN_HANDLED
#define TITLE_SCREEN_HANDLED

// display function for title screen 
void titleScreenDisplay(struct gameState * state)
{
	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		if(state->megaAlpha == 0) // update screen to add title screen 
		{
			system("cls");
			
			// display title 
			printPattern(TITLE,45,10,80,10);
		
			// descriptions 
			setCursor(55,20);
			printf(TITLE_DESC1);
			setCursor(55,21);
			printf(TITLE_DESC2);
		
			state->megaAlpha = 1;
		}
		break;
		case 1: // sprite mode
		renderImage(&state->images[0], state->renderer, NULL);
		break;
	}
}

#endif
