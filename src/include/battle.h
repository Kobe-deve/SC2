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
	array[0] = "1";
	array[1] = "2";
	array[2] = "3";
	array[3] = "4";
	array[4] = "Exit";
		
	initMenu(s,5,array,50,21);
		
	free(array);
}

// displaying during battle
void battleDisplay()
{
	int i = 0;
	int x,y;
	
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
			array[0] = "1";
			array[1] = "2";
			array[2] = "3";
			array[3] = "4";
			array[4] = "Exit";
		
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
			freeMenuProcess(s);
			freeBattleData(s);
			
			system("cls");		
			destroyListeners(s->listeners,MAX_EVENTS);
			registerEvent(DISPLAY,resetDungeon,s->listeners);

			switchTrack(DUNGEON_MUSIC,s);
			break;
		}
	}
}

// free battle data 
void freeBattleData(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	int i;

	if(s->currentBattle.enemies != NULL)
	{
		for(i=0;i<s->currentBattle.numEnemies;i++)
			free(&s->currentBattle.enemies[i]);
		
		free(s->currentBattle.enemies);
	}

	if(s->currentBattle.turnOrder != NULL)
		free(s->currentBattle.turnOrder);
	
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
	registerEvent(DISPLAY,battleLoop,s->listeners);
	
	// start battle music 
	switchTrack(BATTLE_MUSIC,s);
	
	// initialize battle display 
	system("cls");

	// display outlines 
	borders();
	
	// display sprites
	battleDisplay();
	
	generateCommands(s);
}

#endif