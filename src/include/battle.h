// for handling dungeon crawling in the game 
#ifndef BATTLE_HANDLED
#define BATTLE_HANDLED

// initialize battle data 
void initBattle(struct gameState * state)
{
	char ** array = malloc(5 * sizeof(char*));
	array[0] = A_COMMAND;
	array[1] = M_COMMAND;
	array[2] = T_COMMAND;
	array[3] = W_COMMAND;
	array[4] = E_COMMAND;
	
	initMenu(state,5,array,85,24);
	
	state->option = 0;
	
	// set up enemy based on the type encounterd with 
	switch(state->enemyType)
	{
		case 0:
		break;
	}
	
	free(array);
}

// deallocate battle data 
void deallocateBattle(struct gameState * state)
{
}

// general battle display 
void battleDisplay(struct gameState * state)
{	
	if(state->numImages == 0)
	{
		// initialize images based on what enemies are present 
		switch(state->enemyType)
		{
			case 0:
			addImage(state,STRANJER_SPRITE);
			break;
			case 1:
			addImage(state,COBOL_SPRITE);
			break; 
			case 2:
			addImage(state,APOCALYPSE_SPRITE);
			break; 
			case 3:
			addImage(state,COSMIC_SPRITE);
			break;
		}
		
		// set positions based on the number of enemies present 
		state->images[0].x = 20*12;
		state->images[0].y = 10*12;
		
		// initialize ui assets 
	}
	else
		renderImage(&state->images[0], state->renderer, NULL);
	
}

// general battle logic 
void battleLogic(struct gameState * state)
{
	handleMenu(state);
	if(state->input == ENTER)
	{		
		deallocateMenu(state);
		
		state->switchSystem = 1;
		state->switchTo = DUNGEON_SCREEN;
	}
}

#endif
