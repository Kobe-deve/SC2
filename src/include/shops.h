// defines the shop process used
#ifndef SHOP_HANDLED
#define SHOP_HANDLED

void shopScreenDisplay(struct gameState * state)
{
	// add title screen image if images are cleared out  
	if(state->numImages == 0)
	{
		addImage(state,LOGO_IMAGE);
		state->images[0].x = 20*12;
		state->images[0].y = 10*12;
	}
	else
		renderImage(&state->images[0], state->renderer, NULL);
}

void shopLogic(struct gameState * state)
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
