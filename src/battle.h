#ifndef STATE_HANDLED
#include "state.h"
#endif

#ifndef MUSIC_HANDLED
#include "music.h"
#endif

#ifndef EVENT_HANDLING
#include "event_handler.h"
#endif

#ifndef MENU_HANDLED
#include "input.h"
#endif

#ifndef GRAPHICS
#include "graphics.h"
#endif

#ifndef DUNGEON_HANDLED
#include "dungeon.h"
#endif

#ifndef BATTLE_HANDLED
#define BATTLE_HANDLED

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
		system("cls");
		destroyListener(DISPLAY,s->listeners);
		registerEvent(DISPLAY,resetDungeon,s->listeners);

		switchTrack(DUNGEON_MUSIC,s);
	}
}

// start battle 
void initBattle(void *data)
{
	struct gameState * s = (struct gameState *)data;

	// set specific event handlers 
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(DISPLAY,battleLoop,s->listeners);
	
	// start battle music 
	switchTrack(BATTLE_MUSIC,s);
	
	// initialize battle display 
	system("cls");

	// display sprites
	battleDisplay();
	
	// display outlines 
	int i = 0;
	for(i = 21;i<41;i++)
	{
		// for text description 
		setCursor(109,i);
		printf("|");
		
	}
}

#endif