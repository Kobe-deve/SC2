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

#ifndef TEXT_DEFINED
#include "base/text.h"
#endif 

#ifndef BATTLE_HANDLED
#define BATTLE_HANDLED

	// status text lines 
	int maxbattleStatus = 10; // max number of status lines visible 
	int battleMaxStatus = 10; // max number of status lines visible 
	char ** battleStatusText = NULL;
	int battleNumStatusLines = 0;
	
	void freeBattleData(void *data);

	// logic during battle 
	void battleLogic()
	{
		
	}

	// print battle status text	
	void battleDisplayStatus(struct gameState * s)
	{
		int i;
		
		switch(graphicsMode)
		{
			case 0:
			setCursor(1,30);
			printf("STATUS:");
		
			// reset displayed order 
			for(i=0;i<battleNumStatusLines;i++)
			{
				setCursor(1,31+i);
				printf("\33[2K");
				setCursor(1,31+i);
				printf("%s",battleStatusText[i]);
			}
			break;
			case 1:
			
			printText("STATUS:", 10, 400, s->fontHandler);
			
			for(i=0;i<battleNumStatusLines;i++)
				printText(battleStatusText[i], 10, 400+FONT_SIZE+i*FONT_SIZE, s->fontHandler);
			break;
		}
	}
	
	// add line to battle status
	void updateBattleStatus(char * text,struct gameState * s)
	{
		int i;

		// set order of text
		if(battleNumStatusLines < maxbattleStatus)
		{
			battleStatusText[battleNumStatusLines] = text;
			++battleNumStatusLines;
		}
		else // move recent to bottom if max is filled 
		{
			if(graphicsMode == 0 && strlen(battleStatusText[0]) > strlen(text))
			{
				setColor(BLACK);
				for(i=0;i<strlen(battleStatusText[0])+1;i++)
				{
					setCursor(1+i,21);
					printf("%c",219);
				}
				setColor(WHITE);
			}
		
			free(battleStatusText[0]);
			for(i=0;i<battleNumStatusLines-1;i++)
				battleStatusText[i] = battleStatusText[i+1];
			
			battleStatusText[battleNumStatusLines-1] = text;
		}
		
		if(graphicsMode == 0)
			battleDisplayStatus(s);
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

	void displayEnemies(void *data)
	{
		struct gameState * s = (struct gameState *)data;
		int i = 0;
		
		switch(graphicsMode)
		{
			case 0:
			// display enemies 
			for(i = 0;i<s->currentBattle.numEnemies;i++)
			{
				if(s->currentBattle.enemies[i].health > 0)
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
			}
			break;
			case 1:
			// display enemy sprites 
			for(i =0;i<s->currentBattle.numEnemies;i++)
				renderImage(&s->images[i], s->renderer,NULL);	
			break;
		}
	}

	// displaying during battle
	void battleDisplay(void *data)
	{
		struct gameState * s = (struct gameState *)data;
		int x,y,i;
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
			break;
			case 1:
			// display stats
			printText("Stats:", 1000, 10, s->fontHandler);
			char displayer[400];
			
			sprintf(displayer,"%s - HP:%d/%d STAMINA:%d/%d",s->protag_stats.name,s->protag_stats.health,s->protag_stats.maxHealth,s->protag_stats.stamina,s->protag_stats.maxStamina);
			
			printText(displayer,1000,10+FONT_SIZE,s->fontHandler);
			
			// print party 
			for(i=0;i<s->partySize;i++)
			{
				sprintf(displayer,"%s - HP:%d/%d STAMINA:%d/%d",s->party[i].name,s->party[i].health,s->party[i].maxHealth,s->party[i].stamina,s->party[i].maxStamina);
				printText(displayer,1000,10+FONT_SIZE+FONT_SIZE*(i+1),s->fontHandler);
			}
			
			battleDisplayStatus(s);
			break;
		}
		displayEnemies(data);
			
		setColor(WHITE);
	}
	
	// called when a character performs an action 
	void performAction(void *data)
	{
		struct gameState * s = (struct gameState *)data;
		
		// update status to show that the action was performed 
		char actionDisplayer[400];
		char endStatus[400];
		sprintf(actionDisplayer,"%s used slash on %s",s->protag_stats.name,s->currentBattle.enemies[s->enemySelector].name);
		updateBattleStatus(actionDisplayer,s);
		
		// perform action
		s->currentBattle.enemies[s->enemySelector].health = 0;
		battleDisplay(data);
		
		sprintf(endStatus,"%s was defeated",s->currentBattle.enemies[s->enemySelector].name);
		updateBattleStatus(endStatus,s);
		
	}

	// selecting enemies in battle 
	void selectEnemies(void *data)
	{
		struct gameState * s = (struct gameState *)data;
		int i,x,y;
		
		// display cursor 
		//i*25+5,0
		
		for(i=0;i<5;i++)
		{
			setCursor(s->enemySelector*25,i+10);
			printf(">");
		}	
		
		switch(s->input)
		{
			case ENTER:
			performAction(data);
			s->enemySelector = -1;
	
			// reinitialize the menu 
			registerEvent(MENU_SELECTION,menuSelection,s->listeners);
			char ** array = malloc(5 * sizeof(char*));
			array[0] = A_COMMAND;
			array[1] = M_COMMAND;
			array[2] = T_COMMAND;
			array[3] = W_COMMAND;
			array[4] = E_COMMAND;
						
			initMenu(s,5,array,50,21);
				
			free(array);
			
			break;
			case RIGHT:
			if(s->enemySelector < s->currentBattle.numEnemies-1)
			{
				for(i=0;i<5;i++)
				{
					x = s->enemySelector*25;
					y = i+10;
					
					if(x%2 == 1)
						setColor(1);
					else 
						setColor(4);
					
					setCursor(x,y);
					printf("%c",219);
				}
				setColor(WHITE);

				s->enemySelector++;
			}
			break;
			case LEFT:
			if(s->enemySelector > 0)
			{
				for(i=0;i<5;i++)
				{
					x = s->enemySelector*25;
					y = i+10;
					
					if(x%2 == 1)
						setColor(1);
					else 
						setColor(4);
					
					setCursor(x,y);
					printf("%c",219);
				}
				setColor(WHITE);

				s->enemySelector--;
			}
			break;
		}
	}

	// main loop for handling battles 
	void battleLoop(void *data)
	{
		struct gameState * s = (struct gameState *)data;
		int i;
		
		setCursor(125,0);
		printf("Stats:");
		
		// print protag stats 
		setCursor(125,1);
		printf("%s - HP:%d/%d STAMINA:%d/%d",s->protag_stats.name,s->protag_stats.health,s->protag_stats.maxHealth,s->protag_stats.stamina,s->protag_stats.maxStamina);
		
		// print party 
		for(i=0;i<s->partySize;i++)
		{
			setCursor(125,i+2);
			printf("%s - HP:%d/%d STAMINA:%d/%d",s->party[i].name,s->party[i].health,s->party[i].maxHealth,s->party[i].stamina,s->party[i].maxStamina);
		}

		// menu selection 
		if(s->enemySelector == -1 && s->input == ENTER) 
		{
			switch(s->option)
			{
				case 0: // attack
				s->enemySelector = 0;
				
				// set the selector position to the first available enemy who is still alive 
				while(s->enemySelector < s->currentBattle.numEnemies-1 && s->currentBattle.enemies[s->enemySelector].health <= 0)
					s->enemySelector++;
			
				freeMenuProcess(s);				
				break;
				case 1: // magic 
				break;
				case 2: // talk
				break;
				case 3: // item 
				break;
				case 4: // run
				break;
			}
		}
		else if(s->enemySelector >= 0) // if enemy selection starts 
			selectEnemies(data);
		
		int endBattle = 1;
		// check if all enemies are defeated
		for(i = 0;i<s->currentBattle.numEnemies;i++)
		{
			if(s->currentBattle.enemies[i].health > 0)
			{
				endBattle = 0;
				break;
			}
		}
		
		// end battle if condition is met 
		if(endBattle == 1)
		{
			if(graphicsMode == 1)
					clearImages(s);
				
			freeBattleData(s);
			destroyListener(DISPLAY,s->listeners);
			registerEvent(DISPLAY,resetDungeon,s->listeners);	
		}
	}

	// free battle data 
	void freeBattleData(void *data)
	{
		struct gameState * s = (struct gameState *)data;
		
		int i, iy;
		
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
		
		// free status
		if(battleStatusText != NULL)
		{
			for(iy=0;iy<battleMaxStatus;iy++)
			{
				if(battleStatusText[iy] != NULL)
					free(battleStatusText[iy]);
			}
			free(battleStatusText);
		}
		
		// reset all variables used 
		battleStatusText = NULL;
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
		
		// set default enemy selector value_comp
		s->enemySelector = -1;
		
		// reset values for battle 
		s->currentBattle.turns = 0;
		s->currentBattle.numEnemies = 0;
		
		// set up enemies based on the type of battle 
		switch(s->currentBattle.battleType)
		{
			default:
			case 0:
			s->currentBattle.numEnemies = 4;//rand()%3+1;
			
			// allocate enemy data 
			s->currentBattle.enemies = malloc(s->currentBattle.numEnemies * sizeof(struct character));
			
			for(i =0;i<s->currentBattle.numEnemies;i++)
			{
				s->currentBattle.enemies[i] = generateCharacter(rand()%2+2);
			}
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
				s->images[i].y = 120;
			}
			
			// because we need to constantly display the image to the window, set battleDisplay as an event 
			registerEvent(DISPLAY,battleDisplay,s->listeners);
		
			break;
		}
		
		// generate initial menu for options 
		generateCommands(s);
		
		// initialize battle status 
		battleNumStatusLines = 0;
		battleStatusText = malloc(maxbattleStatus * sizeof(char *));	
		for(i=0;i<maxbattleStatus;i++)
			battleStatusText[i] = NULL;
		
		updateBattleStatus("Battle start!",s);
	}

#endif