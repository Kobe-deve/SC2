// basic character menu for seeing the status, accessing settings, etc.
#ifndef CHARACTER_MENU_HANDLED
#define CHARACTER_MENU_HANDLED

// initialize data for character menu
void initCharacterMenu(struct gameState * state)
{
	char ** array = malloc(2 * sizeof(char*));
	array[0] = NG;
	array[1] = CT;
	
	initMenu(state,2,array,85,24);
	
	free(array);
}

// deallocate data for character menu 
void deallocateCharacterMenu(struct gameState * state)
{
	
}

// logic for character menu
void characterMenuLogic(struct gameState * state)
{
	handleMenu(state);
	if(state->input == ENTER)
	{		
		state->switchSystem = 1;
		state->switchTo = DUNGEON_SCREEN;

		deallocateMenu(state);
	}
}


// display character menu 
void characterMenuDisplay(struct gameState * state)
{
	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		if(state->megaAlpha == 0) //  initialize display 
		{
			state->megaAlpha = 1;
			setCursor(1,30);
			printf("MENU");

		}
		break;
		case 1: // sprite mode
		
		printText("We are menu lmao", 10, 400, state->fontHandler);
		/*if(state->numImages == 0)
		{
			addImage(state,LOGO_IMAGE);
			state->images[0].x = 20*12;
			state->images[0].y = 10*12;
		}
		else
			renderImage(&state->images[0], state->renderer, NULL);*/
		
		break;
	}
}



#endif