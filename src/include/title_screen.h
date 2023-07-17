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
		// add title screen image if images are cleared out  
		if(state->numImages == 0)
		{
			addImage(state,LOGO_IMAGE);
			state->images[0].x = 20*12;
			state->images[0].y = 10*12;
		}
		else
			renderImage(&state->images[0], state->renderer, NULL);
		break;
	}
}

// logic handling for title screen 
void titleScreenHandler(struct gameState * state)
{
	if(state->options == NULL) // initialize menu used
	{
		char ** array = malloc(2 * sizeof(char*));
		array[0] = NG;
		array[1] = CT;
		
		initMenu(state,2,array,85,24);
		
		free(array);
	}
	else
		handleMenu(state);
	
	if(state->input == ENTER)
	{		
		state->switchSystem = 1;
		state->switchTo = DUNGEON_SCREEN;
		
		deallocateMenu(state);
	}
}

#endif
