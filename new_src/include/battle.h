// for handling dungeon crawling in the game 
#ifndef BATTLE_HANDLED
#define BATTLE_HANDLED

// initialize battle data 
void initBattle(struct gameState * state)
{
}

// deallocate battle data 
void deallocateBattle(struct gameState * state)
{
}

// general battle display 
void battleDisplay(struct gameState * state)
{	
	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		if(state->megaAlpha == 0) //  initialize display 
		{
			state->megaAlpha = 1;
		}
		break;
		case 1: // sprite mode
		
		printText("TOOL", 10, 400, state->fontHandler);
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

// general battle logic 
void battleLogic(struct gameState * state)
{
	if(state->input == ENTER)
	{		
		state->switchSystem = 1;
		state->switchTo = DUNGEON_SCREEN;
	}
}

#endif
