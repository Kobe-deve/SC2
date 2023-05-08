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
	
		// display enemies 
		for(i =0;i<s->currentBattle.numEnemies;i++)
		{
			// switch based on enemy type 
			switch(s->currentBattle.enemies[i].type)
			{
				case STRANJER:
				printPattern(M1,i*25+5,0,20,20);
				break;
				case LOST_SPIRIT:
				printPattern(M2,i*25+5,0,20,20);
				break;
				case GUARD_DUCK:
				printPattern(M5,i*25+5,0,20,20);
				break;
				case WILLFUL_WISP:
				printPattern(M4,i*25+5,0,20,20);
				break;
				case CHILLER:
				printPattern(M7,i*25+5,0,20,20);
				break;
				case BRASS:
				printPattern(M8,i*25+5,0,20,20);
				break;
				case STARIP:
				printPattern(M6,i*25+5,0,20,20);
				break;
				case GUMMO:
				printPattern(M9,i*25+5,0,20,20);
				break;
				case LOST_HERO:
				printPattern(M12,i*25+5,0,20,20);
				break;
				case MICRONOS:
				printPattern(M13,i*25+5,0,20,20);
				break;
				case DIASNAK:
				printPattern(M14,i*25+5,0,20,20);
				break;
				case WANDERER:
				printPattern(M15,i*25+5,0,20,20);
				break;
				case DIAMAN:
				printPattern(M16,i*25+5,0,20,20);
				break;
				case REVENGE:
				printPattern(M17,i*25+5,0,20,20);
				break;
				default:
				printPattern(M12,i*25+5,0,20,20);
				break;
			}
		}
		break;
		case 1:
		for(i =0;i<s->currentBattle.numEnemies;i++)
			renderImage(&s->images[i], s->renderer,NULL);				
		break;
	}
	setColor(WHITE);
}



// main loop for handling battles 
void battleLoop(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER)
	{
		switch(s->option)
		{
			case 0: // attack
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
			case 1: // magic 
			break;
			case 2: // talk
			break;
			case 3: // item 
			break;
			case 4: // run
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
	int i;
	struct gameState * s = (struct gameState *)data;
	
	// reset values for battle 
	s->currentBattle.turns = 0;
	s->currentBattle.numEnemies = 0;
	
	// set up enemies based on the type of battle 
	switch(s->currentBattle.battleType)
	{
		default:
		case 0:
		s->currentBattle.numEnemies = rand()%3+1;
		
		system("cls");
		printf("%d",s->currentBattle.numEnemies);
		
		// allocate enemy data 
		s->currentBattle.enemies = malloc(s->currentBattle.numEnemies * sizeof(struct character));
		
		for(i =0;i<s->currentBattle.numEnemies;i++)
		{
			s->currentBattle.enemies[i] = generateCharacter(rand()%4+2);
		}
		break;
		case 1:
		
		// allocate enemy data 
		s->currentBattle.enemies = malloc(s->currentBattle.numEnemies * sizeof(struct character));
		break;
	}
	
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
		// set up enemy sprites used 
		s->images = malloc(sizeof(struct image));
		s->numImages = s->currentBattle.numEnemies;
			
		// set image based on enemy type 
		for(i =0;i<s->currentBattle.numEnemies;i++)
		{
			// switch based on enemy type 
			switch(s->currentBattle.enemies[i].type)
			{
				case STRANJER:
				addImage(s,STRANJER_SPRITE);
				break;
				case LOST_SPIRIT:
				addImage(s,COBOL_SPRITE);
				break;
				case GUARD_DUCK:
				addImage(s,APOCALYPSE_SPRITE);
				break;
				case WILLFUL_WISP:
				addImage(s,COSMIC_SPRITE);
				break;
				case CHILLER:
				addImage(s,DREAM_SPRITE);
				break;
				case BRASS:
				addImage(s,EATER_SPRITE);
				break;
				case STARIP:
				addImage(s,FIEND_SPRITE);
				break;
				case GUMMO:
				addImage(s,GATSBY_SPRITE);
				break;
				case LOST_HERO:
				addImage(s,HEART_SPRITE);
				break;
				case MICRONOS:
				addImage(s,LOATING_SPRITE);
				break;
				case DIASNAK:
				addImage(s,MOONLIGHT_SPRITE);
				break;
				case WANDERER:
				addImage(s,OLDER_SPRITE);
				break;
				case DIAMAN:
				addImage(s,SHELL_SPRITE);
				break;
				case REVENGE:
				addImage(s,SHOPKEEPER_SPRITE);
				break;
				default:
				addImage(s,STRANJER_SPRITE);
				break;
			}
			s->images[i].x = 240+200*i;
			s->images[i].y = 120+200*i;
		}
		
		// because we need to constantly display the image to the window, set battleDisplay as an event 
		registerEvent(DISPLAY,battleDisplay,s->listeners);
	
		break;
	}
	
	generateCommands(s);
}

#endif