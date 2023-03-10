#ifndef STATE_HANDLED
#include "base/state.h"
#endif

#ifndef MUSIC_HANDLED
#include "base/music.h"
#endif

#ifndef EVENT_HANDLING
#include "base/event_handler.h"
#endif

#ifndef MENU_HANDLED
#include "base/input.h"
#endif

#ifndef GRAPHICS
#include "base/graphics.h"
#endif

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

#ifndef TEXT_DEFINED
#include "base/text.h"
#endif 

#ifndef BATTLE_HANDLED
#define BATTLE_HANDLED

void freeBattleData(void *data);

// logic during battle 
void battleLogic()
{
	
}

// print battle situation text
void battleSituation()
{
	
}

// print party status
void partyStats()
{
	
}

// generate basic battle menu commands 
void generateCommands(struct gameState * s)
{
	registerEvent(MENU_SELECTION,menuSelection,s->listeners);
	char ** array = malloc(5 * sizeof(char*));
	array[0] = A_COMMAND;
	array[1] = M_COMMAND;
	array[2] = T_COMMAND;
	array[3] = W_COMMAND;
	array[4] = E_COMMAND;
				
	initMenu(s,5,array,50,21);
		
	free(array);
}

// displaying during battle
void battleDisplay(void *data)
{
	struct gameState * s = (struct gameState *)data;
	int i = 0;
	int x,y;
	
	switch(graphicsMode)
	{
		case 0:
		// print background pattern
		for(y=0;y<21;y++)
		{
			for(x=0;x<110;x++)
			{
				if(x%2 == 1)
					setColor(1);
				else 
					setColor(4);
				setCursor(x,y);
				printf("%c",219);
			}
		}	
	
		int numEnemies = 3;
	
		// display enemies 
		for(i =0;i<numEnemies;i++)
		{
			printPattern(M12,i*25+5,0,20,20);
		}
		break;
		case 1:
		renderImage(&s->images[0], s->renderer,NULL);				
		break;
	}
}



// main loop for handling battles 
void battleLoop(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER)
	{
		switch(s->option)
		{
			case 0:
			freeMenuProcess(s);
			
			registerEvent(MENU_SELECTION,menuSelection,s->listeners);
			char ** array = malloc(5 * sizeof(char*));
			array[0] = A_COMMAND;
			array[1] = M_COMMAND;
			array[2] = T_COMMAND;
			array[3] = W_COMMAND;
			array[4] = E_COMMAND;
		
			initMenu(s,5,array,70,21);
		
			free(array);
			break;
			case 1:
			break;
			case 2:
			break;
			case 3:
			break;
			case 4:
			if(graphicsMode == 1)
				clearImages(s);
			
			freeBattleData(s);
			destroyListener(DISPLAY,s->listeners);
			registerEvent(DISPLAY,resetDungeon,s->listeners);
			break;
		}
	}
}

// free battle data 
void freeBattleData(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	int i;
	
	// clear menu process if used 
	if(s->options != NULL)
		freeMenuProcess(s);
			
	// deallocate enemy data 
	if(s->currentBattle.enemies != NULL)
	{
		for(i=0;i<s->currentBattle.numEnemies;i++)
			free(&s->currentBattle.enemies[i]);
		
		free(s->currentBattle.enemies);
	}

	// free turn pointer 
	if(s->currentBattle.turnOrder != NULL)
		free(s->currentBattle.turnOrder);
	
	// reset all variables used 
	s->currentBattle.turns = 0;
	s->currentBattle.numChars = 0;
	s->currentBattle.numEnemies = 0;
	s->currentBattle.turnOrder = NULL;
	s->currentBattle.enemies = NULL;
	
}

// start battle 
void initBattle(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	// destroy all listeners to make way for ones used in battle
	destroyListeners(s->listeners,MAX_EVENTS);

	// set specific event handlers 
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(LOGIC_HANDLER,battleLoop,s->listeners);
	
	// start battle music 
	switchTrack(BATTLE_MUSIC,s);
	
	// display handling 
	switch(graphicsMode)
	{
		case 0:
		// initialize battle display 
		system("cls");

		// display outlines 
		borders();
		
		// display sprites 
		battleDisplay(data);
		break;
		case 1:
		s->images = malloc(sizeof(struct image));
		s->numImages = 1;
			
		addImage(s,TEST_ENEMY_SPRITE);
		s->images[0].x = 20*12;
		s->images[0].y = 10*12;
		registerEvent(DISPLAY,battleDisplay,s->listeners);
	
		break;
	}
	
	generateCommands(s);
}

#endif