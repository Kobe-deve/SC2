// for handling dungeon crawling in the game 
#ifndef DUNGEON_HANDLED
#define DUNGEON_HANDLED

// coord to print dungeon at 
int dungeonPrintCoordX = 1;
int dungeonPrintCoordY = 1;

void dungeonDisplay(struct gameState * state)
{
	switch(state->graphicsMode)
	{
		case 0:
		if(state->megaAlpha == 0) // update screen to add title screen 
		{	
			state->megaAlpha = 1;
		}
		// display player
		setCursor(dungeonPrintCoordX+state->playerX,dungeonPrintCoordY+state->playerY);
		printf("%c",1);	
		break;
		case 1:
		// get dungeon assets if screen is cleared out
		if(state->numImages == 0)
		{
			addImage(state,BACKGROUND_ASSET);
			state->images[0].x = 20*12;
			state->images[0].y = 10*12;
		}
		renderImage(&state->images[0], state->renderer, NULL);
		break;
	}
}

void dungeonLoop(struct gameState * state)
{
}

#endif
