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
#define DUNGEON_HANDLED

#ifndef BATTLE_HANDLED
#include "battle.h"
#endif

#ifndef SHOP_HANDLED
#include "shop.h"
#endif

#ifndef CHARACTER_HANDLED
#include "stats.h"
#endif

#ifndef NPC_HANDLED
#include "base/npc.h"
#endif

#ifndef TEXT_DEFINED
#include "base/text.h"
#endif
 
#ifndef PMENU_HANDLED
#include "menu.h"
#endif

int direction = 0;

Uint32 startTicks = 1;
Uint32 capTicks = 0;
int frames = 0;

// coord to print dungeon at 
int dungeonPrintCoordX = 1;
int dungeonPrintCoordY = 1;

// struct for movable enemies
struct enemies 
{
	int x; // coordinates
	int y;
	int active; // is the enemy alive 
	int type; // the type of enemy for the encounter 
	int inCombat; // is the enemy fighting an npc 
	int npcFighting; // what npc the enemy is fighting 
	int speed; // what interval does the enemy move 
	int startTicks;
	int health; // overworld health of enemy 
};

// array of visible tiles in an area
int *** visible = NULL;

// width/height of the current dungeon
int dungeonSize = 0;

// enemy movement information for dungeon 
int numEnemies = 0;
struct enemies * activeEnemies = NULL;

// status text lines 
int maxStatus = 10; // max number of status lines visible 
char ** statusText = NULL;
int numStatusLines = 0;

// is the player going to sleep?
int sleeping = 0;
int tired = 0;

// dungeon array pointer 
int *** d = NULL;

// coordinates of stairs going up for AI 
int ** upStairCoords = NULL;

// for sprite rendering a section of the spirte sheet 
SDL_Rect spriteClip = {0,0,SPRITE_SQUARE_SIZE,SPRITE_SQUARE_SIZE};

void initDungeonFloor(void *data);
void displayRange(struct gameState * s);
void generateEnemies(struct gameState * s);

// function to check if an npc is at a certain coordinate 
int npcNearby(int x, int y, int f, int isPlayer)
{
	int i;
	
	for(i=0;i<numNPCs;i++)
	{
		if(!activeNPCs[i].passBy && activeNPCs[i].active && (activeNPCs[i].floor == f && (y == activeNPCs[i].y && x == activeNPCs[i].x)))
		{
			// if the coordinates are for the player, set the talked variable to this npc 
			if(isPlayer)
				npcTalked = i;
			return 1;
		}
	}
	
	return 0;
}

// display status in dungeon 
void displayStatus(struct gameState * s)
{
	int i;
	
	switch(graphicsMode)
	{
		case 0:
		setCursor(1,30);
		printf("STATUS:");
	
		// reset displayed order 
		for(i=0;i<numStatusLines;i++)
		{
			setCursor(1,31+i);
			printf("\33[2K");
			setCursor(1,31+i);
			printf("%s",statusText[i]);
		}
		break;
		case 1:
		
		printText("STATUS:", 10, 400, s->fontHandler);
		
		for(i=0;i<numStatusLines;i++)
			printText(statusText[i], 10, 400+FONT_SIZE+i*FONT_SIZE, s->fontHandler);
		break;
	}
}

// update status text and display it 
void updateStatus(char * text,struct gameState * s)
{
	int i;

	// set order of text
	if(numStatusLines < maxStatus)
	{
		statusText[numStatusLines] = text;
		++numStatusLines;
	}
	else // move recent to bottom if max is filled 
	{
		if(graphicsMode == 0 && strlen(statusText[0]) > strlen(text))
		{
			setColor(BLACK);
			for(i=0;i<strlen(statusText[0])+1;i++)
			{
				setCursor(1+i,21);
				printf("%c",219);
			}
			setColor(WHITE);
		}
	
		free(statusText[0]);
		for(i=0;i<numStatusLines-1;i++)
			statusText[i] = statusText[i+1];
		
		statusText[numStatusLines-1] = text;
	}
	
	if(graphicsMode == 0)
		displayStatus(s);
}

// briefly using this function to convert old dungeon 
int quickConvert(int x)
{
	switch(x)
	{
		case 1:
		return 219;
		break;
		
		case 2:
		case 3:
		return 220;
		break;
		
		case 4:
		return 233;
		break;
		
		case 5:
		return 237;
		break;
		
		case 6:
		case A:
		case B:
		case 12:
		case D:
		case F:
		case G:
		case 7:
		return 1;
		break;
		
		case 8:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		return 234;
		break;
		
		case 9:
		return 206;
		break;
		
		default:
		case 100:
		return 178;
		break;
	}
}

// reset after encounter or moving to a new floor
void resetDungeon(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	int counterx,countery;
	int i;
	
	// clear screen
	system("cls");	

	// clear all event functions
	destroyListeners(s->listeners,MAX_EVENTS);
	
	// allocate sprites back if they're gone 
	if(s->images == NULL)
	{
		s->images = malloc(sizeof(struct image));
		s->numImages = 1;
			
		addImage(s,DUNGEON_SPRITE);
		s->images[0].x = SPRITE_SQUARE_SIZE;
		s->images[0].y = SPRITE_SQUARE_SIZE;
		s->images[0].scale = 2;
	}
	
	// if variables used in battle aren't null, free them
	if(s->currentBattle.enemies != NULL)
		free(s->currentBattle.enemies);
	s->currentBattle.enemies = NULL;
	
	// display visible spaces and dungeon border 
	for(countery = -1;countery < dungeonSize+1;countery++)
	{
		for(counterx = -1;counterx < dungeonSize+1;counterx++)
		{
			setCursor(dungeonPrintCoordX+counterx,dungeonPrintCoordY+countery);
			if((counterx == -1 || counterx == dungeonSize || countery == -1 || countery == dungeonSize))
			{	
				setColor(BLUE);
				printf("%c",219);
			}				
			else if(visible[s->floor][countery][counterx] == 1)
				printf("%c",quickConvert(d[s->floor][countery][counterx]));
			setColor(WHITE);
		}
	}
	
	// check main display range around the player 
	displayRange(s);
	
	// display visible enemies 
	if(graphicsMode == 0)
	{
		for(i = 0;i<numEnemies;i++)
		{
			if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1 && activeEnemies[i].active == 1)
			{
				setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
				printf("+");
			}
		}
	}
	
	// reset passBy variable for npcs 
	for(i=0;i<numNPCs;i++)
		activeNPCs[i].passBy = 0;
	
	// switch back to dungeon track 
	switchTrack(DUNGEON_MUSIC,s);
			
	// register back to main dungeon loop 
	registerEvent(DISPLAY,walkAround,s->listeners);
	
	if(graphicsMode == 0)
		displayStatus(s);
}

// start encounter (setting up the stats for the enemy side)
void startEncounter(int type, void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	// set up battle structure based on the type of enemy 
	s->currentBattle.turns = 0;
	s->currentBattle.numEnemies = 0;
	s->currentBattle.enemies = malloc(s->currentBattle.numEnemies * sizeof(struct character));
	switch(type)
	{
		case 0:
		break;
		case 1:
		break;
		case 2:
		break;
		case 3:
		break;
	}
	
	// clear dungeon images 
	if(graphicsMode == 1)
		clearImages(s);
	
	// set events to start battle processing
	destroyListener(MENU_SELECTION,s->listeners);
	destroyListener(DISPLAY,s->listeners);
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(LOGIC_HANDLER,initBattle,s->listeners);
	
	updateStatus(ENCOUNTERED,s);
}

// displaying a dream 
void dreamDisplay(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	if(s->input == ENTER) // exit sleeping/dream back to dungeon 
	{
		system("cls");
		updateStatus(WAKIGNUP,s);	
		sleeping = 0;
		tired = 0;
		registerEvent(DISPLAY,resetDungeon,s->listeners);
	}
}

// starting a dream 
void startDream(struct gameState * s)
{
	system("cls");
	printf("You are dreaming");
	
	destroyListener(MENU_SELECTION,s->listeners);
	destroyListener(DISPLAY,s->listeners);
	destroyListener(LOGIC_HANDLER,s->listeners);
	registerEvent(DISPLAY,dreamDisplay,s->listeners);
}

// display range when moving  
void displayRange(struct gameState * s)
{
	int i;
	int enemyHere = 0;
	int x,y;
	for(y=s->playerY-2;y<s->playerY+3;y++)
	{
		for(x=s->playerX-2;x<s->playerX+3;x++)
		{	
			if(!(x == s->playerX && y == s->playerY))
			{
				if((y >= 0 && x >= 0 && y < dungeonSize && x < dungeonSize))
				{
					if((visible[s->floor][y][x] == 0))
					{
						setCursor(dungeonPrintCoordX+x,dungeonPrintCoordY+y);
			
						visible[s->floor][y][x] = 1;
						
						printf("%c",quickConvert(d[s->floor][y][x]));
					}
				}
				
				setColor(WHITE);
			}
			
			// checks if any enemies are visible with the updated range
			if((y >= 0 && x >= 0 && y < dungeonSize && x < dungeonSize))
			{
				for(i = 0;i<numEnemies;i++)
				{
					if(graphicsMode == 0 && activeEnemies[i].inCombat == 0 && activeEnemies[i].active == 1 && activeEnemies[i].x == x && activeEnemies[i].y == y)
					{
						setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
						printf("+");
					}
				}
			}
		}
	}
}

// generate text status descriptions based on where the player is 
void description(struct gameState * s)
{	
	// check nearby npcs close to the player 
	npcNearPlayer = (npcNearby(s->playerX,s->playerY-1,s->floor,1) || npcNearby(s->playerX,s->playerY+1,s->floor,1) || npcNearby(s->playerX+1,s->playerY,s->floor,1) || npcNearby(s->playerX-1,s->playerY,s->floor,1));
				
	// update status text based on where the player is at 
	switch(d[s->floor][s->playerY][s->playerX])
	{
		case 5:
		updateStatus(EMPTY_CHEST,s);
		break;
		case 4:
		updateStatus(CLOSED_CHEST,s);
		break;
		case 9:
		updateStatus(SHOP_ENTRANCE,s);
		break;
		default:
		// if an npc is nearby, update status 
		if(npcNearPlayer && !activeNPCs[npcTalked].inCombat && activeNPCs[npcTalked].active == 1)
		{
			switch(activeNPCs[npcTalked].type)
			{
				case 0:
				updateStatus(NPC1,s);	
				break;
				case 1:
				updateStatus(NPC2,s);	
				break;
			}
		}
		else if(npcNearPlayer && activeNPCs[npcTalked].inCombat )
			updateStatus(NPC_FIGHT,s);	
				
		break;
	}
}

// clear display when player moves  
void clearDisplay(struct gameState * s)
{	
	if(s->input != 0)
	{
		setCursor(dungeonPrintCoordX+s->playerX,dungeonPrintCoordY+s->playerY);
		printf("%c",quickConvert(d[s->floor][s->playerY][s->playerX]));	
	}
}

// moves enemies on the floor 
void enemyHandler(struct gameState * s)
{
	int i = 0;
	int j,enemyHere;
	int cYo;
	int directionX, directionY;
	
	if(debug == 1)
	{
		setCursor(50,19);
		printf("ENEMIES: %d",numEnemies);
	}
	
	for(i = 0;i<numEnemies;i++)
	{
		enemyHere = 0;
		
		directionY = 0;
		directionX = 0;
		if(activeEnemies[i].inCombat == 0 && activeEnemies[i].active == 1 && ((int)(SDL_GetTicks() - activeEnemies[i].startTicks))/1000.f >= activeEnemies[i].speed)
		{
			// erase/update current spot when moving 
			if(graphicsMode == 0 && visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				// make sure there isn't another enemy on this spot if updating 
				setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
				printf("%c",quickConvert(d[s->floor][activeEnemies[i].y][activeEnemies[i].x]));	
			}	
			else if(!sleeping && conversation == NONE)
				updateStatus(ENEMY_MOVEMENT,s);	
			
			// set up direction variables 
			if(s->playerX > activeEnemies[i].x)
				directionX = 1;
			else if(s->playerX < activeEnemies[i].x)
				directionX = 3;
				
			if(s->playerY > activeEnemies[i].y)
				directionY = 1;
			else if(s->playerY < activeEnemies[i].y)
				directionY = 2;
			
			int cY = activeEnemies[i].y;
			int cX = activeEnemies[i].x; 
			cYo = 0;
			
			// move in y direction 
			switch(directionY)
			{
				case 1:
				if(cY < dungeonSize-1 && d[s->floor][cY+1][cX] != 1)
					activeEnemies[i].y++;
				break;
				case 2:
				if(cY > 0 && d[s->floor][cY-1][cX] != 1)
					activeEnemies[i].y--;
				break;
			}
			
			// move in x direction 
			cYo = cY;
			cY = activeEnemies[i].y;
			
			switch(directionX)
			{
				case 1:
				if(cX < dungeonSize-1 && d[s->floor][cY][cX+1] != 1)
					activeEnemies[i].x++;
				break;
				case 3:
				if(cX > 0 && d[s->floor][cY][cX-1] != 1)
					activeEnemies[i].x--;
				break;
			}
			
			// make sure enemies don't overlap when moving, if so then keep them at their spot 
			for(j=0;j<numEnemies;j++)
			{
				if(j !=i && activeEnemies[j].active == 1 && ((activeEnemies[i].y == activeEnemies[j].y) && (activeEnemies[i].x == activeEnemies[j].x)))
				{
					activeEnemies[i].x = cX;
					activeEnemies[i].y = cYo;
					break;
				}
			}
		
			// display enemy 
			
			// starting fight with npc 
			int checkNPC = -1;
			
			for(j=0;j<numNPCs;j++)
			{
				if(activeNPCs[j].active == 1 && (activeNPCs[j].floor == s->floor && (activeEnemies[i].y == activeNPCs[j].y && activeEnemies[i].x == activeNPCs[j].x)))
				{
					checkNPC = j;
					break;
				}
			}

			// if npc found, start battle 
			if(checkNPC != -1)
			{
				updateStatus(HEAR_FIGHT,s);
				activeEnemies[i].inCombat = 1;
				activeEnemies[i].npcFighting = checkNPC;
				activeNPCs[checkNPC].passBy = 0;
				activeNPCs[checkNPC].inCombat = 1;
				activeNPCs[checkNPC].enemyCombat = i;
			}
			
			// reset start tick 
			activeEnemies[i].startTicks = SDL_GetTicks();
		}
		else if(activeEnemies[i].inCombat == 1 && ((int)(SDL_GetTicks() - activeEnemies[i].startTicks))/1000.f >= activeEnemies[i].speed) // handle fighting an npc over time 
		{
			// update status 
			updateStatus(NPCvsENEMY,s);	
			
			// change stats based on combat 
			switch(rand()%4)
			{
				case 0: // npc takes damage
				activeNPCs[activeEnemies[i].npcFighting].stats.health--;
				break;
				case 1: // curse takes some sort of damage 
				case 2:
				activeEnemies[i].health--;
				break;
				default: // nothing 
				break;
			}
			
			// check if NPC was defeated, if so deactivate NPC and get enemy moving again 
			if(activeNPCs[activeEnemies[i].npcFighting].stats.health <= 0)
			{
				updateStatus(NPCLOST,s);	
				
				activeNPCs[activeEnemies[i].npcFighting].active = 0;
				activeNPCs[activeEnemies[i].npcFighting].inCombat = 0;
				activeEnemies[i].inCombat = 0;
				
			}
			else if(activeEnemies[i].health <= 0) // check if enemy was defeated 
			{
				updateStatus(NPCWON,s);
	
				activeNPCs[activeEnemies[i].npcFighting].inCombat = 0;
				activeEnemies[i].inCombat = 0;
				activeEnemies[i].active = 0;
			}
			
			// reset start tick 
			activeEnemies[i].startTicks = SDL_GetTicks();
		}
		
		if(debug == 1)
		{
			setCursor(50,20+i);
			printf("ENEMY #%d: (%d, %d) STATUS:%d MOVEMENT: %d",i,activeEnemies[i].x,activeEnemies[i].y,activeEnemies[i].active,((int)(SDL_GetTicks()))% activeEnemies[i].speed);
		}
		
		// display enemy 
		if(graphicsMode == 0 && visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1 && activeEnemies[i].inCombat == 0 && activeEnemies[i].active == 1)
		{
			setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
			printf("+");
		}
	}
}

// handling npcs on the floor 
void npcHandler(struct gameState * s)
{
	int i,j;
	int movement = 0;
	int cx,cy;
	
	if(debug)
	{
		setCursor(120,15);
		printf("NPC NEAR:%d",npcTalked);	
		
		setCursor(110,19);
		printf("NPCS:");	
	}
	
	// display/move npcs  
	for(i = 0;i<numNPCs;i++)
	{	
		movement = 0;
		if(debug == 1)
		{
			setCursor(110,19+1+i);
			setColor(WHITE);
			printf("NPC #%d: (%d, %d, %d) HP:%d RECPT:%d CURIOUS:%d NUMSAVED:%d NUMPASSED:%d PASSBY:%d GOAL:%d  ",
				i,activeNPCs[i].x,activeNPCs[i].y,activeNPCs[i].floor,activeNPCs[i].stats.health,activeNPCs[i].reception, activeNPCs[i].curiosity,activeNPCs[i].numSaved,
				activeNPCs[i].numPassed,activeNPCs[i].passBy,activeNPCs[i].goal);
		}

		if(activeNPCs[i].active)
		{
			switch(activeNPCs[i].goal)
			{
				case ESCAPE_DUNGEON:
				movement = 1;
				break;
			}
			
			
			// movement logic 
			if(movement == 1 && activeNPCs[i].inCombat != 1 && activeNPCs[i].talking != 1 && ((int)(SDL_GetTicks() - activeNPCs[i].startTicks))/1000.f >= activeNPCs[i].speed)
			{
				// erase/update current spot when moving 
				if(graphicsMode == 0 && !activeNPCs[i].talking && !activeNPCs[i].inCombat && movement == 1 && s->floor == activeNPCs[i].floor && visible[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x] == 1)
				{
					setColor(WHITE);
					setCursor(dungeonPrintCoordX+activeNPCs[i].x,dungeonPrintCoordX+activeNPCs[i].y);
					printf("%c",quickConvert(d[s->floor][activeNPCs[i].y][activeNPCs[i].x]));	
				}
				
				// movement strategy 
				/* 
					0 - up 
					1 - right 
					2 - down 
					3 - left 
				*/
				
				switch(activeNPCs[i].goal)
				{
					case ESCAPE_DUNGEON:
					cx = activeNPCs[i].x;
					cy = activeNPCs[i].y;
					
					if(upStairCoords[activeNPCs[i].floor][0] == activeNPCs[i].x)
						activeNPCs[i].direction = -2;			
					else if(upStairCoords[activeNPCs[i].floor][0] > activeNPCs[i].x && d[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x+1] != 1 && activeNPCs[i].x < dungeonSize-1)
						activeNPCs[i].direction = 1;
					else if(upStairCoords[activeNPCs[i].floor][0] < activeNPCs[i].x &&  d[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x-1] != 1 && activeNPCs[i].x > 0)
						activeNPCs[i].direction = 3;
					else
						activeNPCs[i].direction = -1;
						
					switch(activeNPCs[i].direction)
					{
						case 1:
						activeNPCs[i].x++;
						break;
						case 3:
						activeNPCs[i].x--;
						break;
						case -2:
						break;
						case -1:
						if(d[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x-1] != 1)
							activeNPCs[i].x--;
						else if(d[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x+1] != 1)
							activeNPCs[i].x++;
						break;
					}
					
					if(upStairCoords[activeNPCs[i].floor][1] == activeNPCs[i].y)
						activeNPCs[i].direction = -2;			
					else if(upStairCoords[activeNPCs[i].floor][1] > activeNPCs[i].y && d[activeNPCs[i].floor][activeNPCs[i].y+1][activeNPCs[i].x] != 1 && activeNPCs[i].y < dungeonSize-1)
						activeNPCs[i].direction = 2;
					else if(upStairCoords[activeNPCs[i].floor][1] < activeNPCs[i].y && d[activeNPCs[i].floor][activeNPCs[i].y-1][activeNPCs[i].x] != 1 && activeNPCs[i].y > 0)
						activeNPCs[i].direction = 0;
					else
						activeNPCs[i].direction = -1;
					
					switch(activeNPCs[i].direction)
					{
						case 0:
						activeNPCs[i].y--;
						break;
						case 2:
						activeNPCs[i].y++;
						break;
						case -2:
						break;
						default:
						case -1:
						if(d[activeNPCs[i].floor][activeNPCs[i].y-1][activeNPCs[i].x] != 1)
							activeNPCs[i].y--;
						else if(d[activeNPCs[i].floor][activeNPCs[i].y+1][activeNPCs[i].x] != 1)
							activeNPCs[i].y++;
						
						break;
					}
					
					// check if there isn't an overlap with another npc 
					for(j=0;j<numNPCs;j++)
					{
						if(j!=i && (activeNPCs[i].x == activeNPCs[j].x) && (activeNPCs[i].y == activeNPCs[j].y))
						{
							activeNPCs[i].x = cx;
							activeNPCs[i].y = cy;
							break;
						}
					}
					
					// check if there isn't any overlap with the player 
					if(activeNPCs[i].y == s->playerY && activeNPCs[i].x==  s->playerX)
					{
						activeNPCs[i].x = cx;
						activeNPCs[i].y = cy;
					}						
				
					break;
				}
				
				// go upstairs if at stairs 
				if(d[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x] == 2)
					activeNPCs[i].floor++;
				
				activeNPCs[i].startTicks = SDL_GetTicks();
			}
			
			// display npc 
			if(visible[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x] == 1 && s->floor == activeNPCs[i].floor)
			{
				switch(graphicsMode)
				{
					case 0:
					setCursor(dungeonPrintCoordX+activeNPCs[i].x,dungeonPrintCoordX+activeNPCs[i].y);
					switch(activeNPCs[i].type)
					{
						case 0:
						setColor(DARK_BABY_BLUE);
						break;
						case 1:
						setColor(SILVER);
						break;
					}
				
					if(activeNPCs[i].inCombat == 1) // shows a fight 
						printf("X");
					else
						printf("%c",1);
					break;
					case 1:
					
					break;
				}
			}
		}
	}
	setColor(WHITE);
}

// logic handling in dungeon section for moving player/npcs/etc
void dungeonLogic(void *data, struct gameState * s)
{
	int i;
	char test;
	
	// move enemies 
	enemyHandler(s);
	
	// move npcs 
	npcHandler(s);
	
	// check if player has run into enemies 
	for(i=0;i<numEnemies;i++)
	{
		if(activeEnemies[i].active == 1 && activeEnemies[i].y == s->playerY && activeEnemies[i].x == s->playerX)
		{
			activeEnemies[i].active = 0;
			startEncounter(activeEnemies[i].type,data);
			
			setCursor(dungeonPrintCoordX+activeEnemies[i].x,dungeonPrintCoordX+activeEnemies[i].y);
			printf("%c",quickConvert(d[s->floor][activeEnemies[i].y][activeEnemies[i].x]));	
		}
	}
	
	if(tired == 1 && !sleeping) // showing move to sleep/dream 
	{
		switchTrack(DREAM_MUSIC,s);
		
		updateStatus(PASSOUT,s);	
		updateStatus(PRESS_ENTER,s);	
		sleeping = 1;
	}
	
	// if talking, handle discussion logic 
	if(conversation != NONE)
	{
		// checks if a menu is being used for responding 
		if(!talkOver && s->listeners[MENU_SELECTION] == NULL)
			npcDialogueHandler(npcTalked,s);
		else if(conversation != BATTLE && conversation != JOIN_PARTY && s->input == ENTER) // free menu if selection made and push conversation in direction 
		{
			// handle response based on option selected
			if(conversation == INTRO) // handle introduction response 
				conversation = s->option;
			else if(conversation == NO_DISCUSS) // handle no discussion response 
			{
				switch(s->option)
				{
					case 0:
					conversation = NPC_QUESTION;
					break;
					case 1:
					conversation = PASS;
					break;
					case 2:
					talkOver = 1;
					conversation = LEAVE;
					break;
				}
			}
			else if(conversation == NPC_QUESTION) // handle npc question response 
				conversation = PLAYER_RESPONSE;
			
			// clear menu 
			clearMenu(s);
			freeMenuProcess(s);
		}
		
		// if conversation over, press enter to leave 
		if(conversation != BATTLE && conversation != JOIN_PARTY && talkOver == 1 && s->input == ENTER )
			conversation = LEAVE;
	
		// walking away from conversation 
		if(conversation == LEAVE || conversation == PASS_BY)  // being done with conversation 
		{
			if(conversation == PASS_BY)
				updateStatus(PASS_NPC,s);	
			else
				updateStatus(NPC_DONE_TALK,s);	
			
			conversation = NONE;
		}
		else if(conversation != BATTLE && conversation != JOIN_PARTY && s->input == BACKSPACE) // walking away
		{
			conversation = NONE;
			
			updateStatus(NPC_RUN_AWAY,s);	
		}
		else if(talkOver == 1 && conversation == BATTLE && s->input == ENTER) // battling npc 
		{
			activeNPCs[npcTalked].active = 0;
			talkOver = 0;
			conversation = NONE;
			
			if(s->listeners[MENU_SELECTION] != NULL)
			{
				clearMenu(s);
				freeMenuProcess(s);	
			}
			
			startEncounter(activeNPCs[npcTalked].enemyCombat,data);
		}
		else if(talkOver == 1 && conversation == JOIN_PARTY && s->input == ENTER) // if an npc joins the party 
		{
			// add to party 
			addPartyMember(activeNPCs[npcTalked].stats,s);
			
			// clear from map 
			setCursor(dungeonPrintCoordX+activeNPCs[npcTalked].x,dungeonPrintCoordX+activeNPCs[npcTalked].y);
			printf("%c",quickConvert(d[s->floor][activeNPCs[npcTalked].y][activeNPCs[npcTalked].x]));	
				
			// set conversation variables back 
			activeNPCs[npcTalked].active = 0;
			talkOver = 0;
			conversation = NONE;
		}
		
		// free menu if conversation over
		if(conversation == NONE)
		{
			activeNPCs[npcTalked].talking = 0;
			talkOver = 0;
			if(s->listeners[MENU_SELECTION] != NULL)
			{
				clearMenu(s);
				freeMenuProcess(s);	
			}
		}
	}
	else if(tired == 1 && s->input == ENTER) // moving to sleep 
		startDream(s);
	else if(!tired)// regular dungeon crawling controls 
	{
		// input handling with moving the player and other commands 
		switch(s->input)
		{
			// movement 
			case UP:
			direction = 0;
			if(s->playerY > 0 && d[s->floor][s->playerY-1][s->playerX] != 1 && !npcNearby(s->playerX,s->playerY-1,s->floor,0))
			{
				displayRange(s);
				s->playerY--;
				direction = 0;
				description(s);
				displayRange(s);
			}
			break;
			case DOWN:
			direction = 2;
			if(s->playerY < dungeonSize-1 && d[s->floor][s->playerY+1][s->playerX] != 1 && !npcNearby(s->playerX,s->playerY+1,s->floor,0))
			{
				displayRange(s);
				s->playerY++;
				direction = 2;
				description(s);
				displayRange(s);
			}
			break;
			case LEFT:
			direction = 3;
			if(s->playerX > 0 && d[s->floor][s->playerY][s->playerX-1] != 1 && !npcNearby(s->playerX-1,s->playerY,s->floor,0))
			{
				displayRange(s);
				s->playerX--;
				direction = 3;
				description(s);
				displayRange(s);
			}
			break;
			case RIGHT:
			direction = 1;
			if(s->playerX < dungeonSize-1 && d[s->floor][s->playerY][s->playerX+1] != 1  && !npcNearby(s->playerX+1,s->playerY,s->floor,0))
			{
				displayRange(s);
				s->playerX++;
				direction = 1;
				description(s);
				displayRange(s);
			}	
			break;
			
			case MENU: // toggle menu 
			initPlayerMenu(s);
			break;
			
			case ENTER: // interact with nearby objects
			// handle interacting with items/objects 
			if(d[s->floor][s->playerY][s->playerX] != 0 && d[s->floor][s->playerY][s->playerX] != E)
			{
				switch(d[s->floor][s->playerY][s->playerX])
				{
					case 9: // shop 
					initShop(s);
					break;
					case 4: // chest
					d[s->floor][s->playerY][s->playerX] = 5;
					updateStatus(OPENED_CHEST,s);
					break;
				}
			}	
			else if(activeNPCs[npcTalked].active == 1 && npcNearPlayer && !activeNPCs[npcTalked].inCombat) // interact with npc
			{
				// set up menu and variables for talking 
				updateStatus(TALK_TO_NPC,s);
				conversation = INTRO;
				
				activeNPCs[npcTalked].talking = 1;
				
				// set up initial dialogue process 
				freeMenuProcess(s);
			
				registerEvent(MENU_SELECTION,menuSelection,s->listeners);
				char ** array = malloc(3 * sizeof(char*));
				array[0] = G_COMMAND;
				array[1] = Q_COMMAND;
				array[2] = P_COMMAND;
				initMenu(s,3,array,70,31);
		
				free(array);
			}	
			else if(activeNPCs[npcTalked].active == 1 && npcNearPlayer && activeNPCs[npcTalked].inCombat) // help npc in combat 
			{
				activeEnemies[activeNPCs[npcTalked].enemyCombat].active = 0;
				activeEnemies[activeNPCs[npcTalked].enemyCombat].inCombat = 0;
				
				activeNPCs[npcTalked].inCombat = 0;
				activeNPCs[npcTalked].numSaved++;
				
				// start encounter 
				startEncounter(activeNPCs[npcTalked].enemyCombat,data);
			}
			break;
			case BACKSPACE:
			//tired = 1;
			debug = !debug;
			break;
			default:
			break;
		}
	}
}

// display movable elements in dungeon 
void display(struct gameState * s)
{
	int i,ix,iy;
	char * output = "";
		
	switch(graphicsMode)
	{
		case 0:
		// display player
		setCursor(dungeonPrintCoordX+s->playerX,dungeonPrintCoordY+s->playerY);
		printf("%c",1);	
	
		// display UI
		if(debug == 0)
		{
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
		}
		break;
		case 1:
		// display dungeon 
		for(iy=0;iy<dungeonSize;iy++)
		{
			for(ix=0;ix<dungeonSize;ix++)
			{	
				if(visible[s->floor][iy][ix])
				{
					
					switch(d[s->floor][iy][ix])
					{
						case 1: // wall
						spriteClip.x = SPRITE_SQUARE_SIZE*2;
						spriteClip.y = 0;
						break;
						case 2: // up stairs 
						spriteClip.x = SPRITE_SQUARE_SIZE;
						spriteClip.y = 0;
						s->images[0].x = ix*SPRITE_SQUARE_SIZE*s->images[0].scale;
						s->images[0].y = iy*SPRITE_SQUARE_SIZE*s->images[0].scale;
						renderImage(&s->images[0], s->renderer,&spriteClip);
						
						spriteClip.x = SPRITE_SQUARE_SIZE*3;
						spriteClip.y = 0;
						break;
						case 3: // down stairs
						spriteClip.x = SPRITE_SQUARE_SIZE;
						spriteClip.y = 0;
						s->images[0].x = ix*SPRITE_SQUARE_SIZE*s->images[0].scale;
						s->images[0].y = iy*SPRITE_SQUARE_SIZE*s->images[0].scale;
						renderImage(&s->images[0], s->renderer,&spriteClip);
					
						spriteClip.x = SPRITE_SQUARE_SIZE*6;
						spriteClip.y = 0;
						break;
						case 4: // chest 
						spriteClip.x = SPRITE_SQUARE_SIZE*4;
						spriteClip.y = 0;
						break;
						case 5: // opened chest 
						spriteClip.x = SPRITE_SQUARE_SIZE*5;
						spriteClip.y = 0;
						break;
						case 9: // shop
						spriteClip.x = SPRITE_SQUARE_SIZE;
						spriteClip.y = 0;
						s->images[0].x = ix*SPRITE_SQUARE_SIZE*s->images[0].scale;
						s->images[0].y = iy*SPRITE_SQUARE_SIZE*s->images[0].scale;
						renderImage(&s->images[0], s->renderer,&spriteClip);
					
						spriteClip.x = SPRITE_SQUARE_SIZE*9;
						spriteClip.y = 0;
						break;
						default: // floor 
						spriteClip.x = SPRITE_SQUARE_SIZE;
						spriteClip.y = 0;
						break;
					}
				
					s->images[0].x = ix*SPRITE_SQUARE_SIZE*s->images[0].scale;
					s->images[0].y = iy*SPRITE_SQUARE_SIZE*s->images[0].scale;
					renderImage(&s->images[0], s->renderer,&spriteClip);
				}
			}
		}
		
		// display npc 
		for(i = 0;i<numNPCs;i++)
		{		
			if(activeNPCs[i].active && visible[activeNPCs[i].floor][activeNPCs[i].y][activeNPCs[i].x] == 1 && s->floor == activeNPCs[i].floor)
			{
				// change sprite based on situation the npc is in 
				if(activeNPCs[i].inCombat == 1)
					spriteClip.x = SPRITE_SQUARE_SIZE*10;
				else	
					spriteClip.x = SPRITE_SQUARE_SIZE*11;
				
				spriteClip.y = 0;
						
				s->images[0].x = activeNPCs[i].x*SPRITE_SQUARE_SIZE*s->images[0].scale;
				s->images[0].y = activeNPCs[i].y*SPRITE_SQUARE_SIZE*s->images[0].scale;
				renderImage(&s->images[0], s->renderer,&spriteClip);
			}
		}
		
		// display player 
		spriteClip.x = 0;
		spriteClip.y = 0;
						
		s->images[0].x = s->playerX*SPRITE_SQUARE_SIZE*s->images[0].scale;
		s->images[0].y = s->playerY*SPRITE_SQUARE_SIZE*s->images[0].scale;
		renderImage(&s->images[0], s->renderer,&spriteClip);
		
		// display enemies 
		for(i = 0;i<numEnemies;i++)
		{		
			if(visible[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1 && activeEnemies[i].inCombat == 0 && activeEnemies[i].active == 1)
			{				
				spriteClip.x = SPRITE_SQUARE_SIZE*8;
				spriteClip.y = 0;
				s->images[0].x = activeEnemies[i].x*SPRITE_SQUARE_SIZE*s->images[0].scale;
				s->images[0].y = activeEnemies[i].y*SPRITE_SQUARE_SIZE*s->images[0].scale;
				renderImage(&s->images[0], s->renderer,&spriteClip);			
			}
		}	
		
		// display UI
		printText("Stats:", 1000, 10, s->fontHandler);
		
		displayStatus(s);
		break;
	}
	
	if(debug == 1)
	{
		setCursor(60,10);
		printf("FLOOR: %d",s->floor);
		setCursor(60,15);
		printf("VALUE: %d  ",d[s->floor][s->playerY][s->playerX]);
	}
	
}

// main dungeon crawling function 
void walkAround(void *data)
{
	struct gameState * s = (struct gameState *)data;
	
	capTicks = 0;
	
	if(graphicsMode == 0)
		clearDisplay(s);
	
	dungeonLogic(data,s);
	
	// move floors and update display 
	if(d[s->floor][s->playerY][s->playerX] == 2 || d[s->floor][s->playerY][s->playerX] == 3)
	{	
		system("cls");
		destroyListener(DISPLAY,s->listeners);
		switch(d[s->floor][s->playerY][s->playerX])
		{
			case 2:
			updateStatus(WALK_UP,s);
			s->floor = s->floor+1;
			break;
			case 3:
			updateStatus(WALK_DOWN,s);
			s->floor = s->floor-1;
			break;
		}	
		generateEnemies(s);
		resetDungeon(data);
	}
	else if(s->listeners[DISPLAY] != NULL && s->listeners[DISPLAY]->mainFunction == walkAround)// display 
		display(s);
		
}

// define dungeon based on file
void readDungeonFile(char * fileName)
{
	// the file to be read 
	FILE *readFile;
	char * fileReader = malloc(128 * sizeof(char)); 
	
	readFile = fopen(fileName,"r");
	
	int iy, iz, ix;
	
	// check if file is opened
	if(!readFile)
		throwError("ERROR:DUNGEON FILE COULD NOT BE READ");
	else
	{
		// read size of dungeon 
		fscanf(readFile,"%s",fileReader);
		dungeonSize = atoi(fileReader);
		
		// read number of enemies spawned per floor 
		fscanf(readFile,"%s",fileReader);
		numEnemies = atoi(fileReader);
		
		// skip line
		fscanf(readFile,"%s",fileReader);
	
		// allocate size 
		d = malloc(dungeonSize * sizeof(int **));
		upStairCoords = malloc(dungeonSize * sizeof(int *));
		for(iz=0;iz<dungeonSize;iz++)
		{
			upStairCoords[iz] = malloc(sizeof(int[2])); // malloc stair coords 
			d[iz] = malloc(dungeonSize * sizeof(int *));
			for(iy=0;iy<dungeonSize;iy++)
			{	
				d[iz][iy] = malloc(dungeonSize * sizeof(int));
				for(ix=0;ix<dungeonSize;ix++)
					d[iz][iy][ix] = 0;
			}	
		}
		
		// read file data 
		for(iz=0;iz<dungeonSize;iz++)
		{
			for(iy=0;iy<dungeonSize;iy++)
			{	
				for(ix=0;ix<dungeonSize;ix++)
				{	
					fscanf(readFile,"%s",fileReader);
					
					// checks if character is a letter 
					if(fileReader[0] >= 'A' && (strcmp(fileReader," ") != 0))
						d[iz][iy][ix] = 10+(fileReader[0]-65);		
					else if(strcmp(fileReader," ") != 0)
						d[iz][iy][ix] = atoi(fileReader);
					
					if(d[iz][iy][ix] == 2)
					{
						upStairCoords[iz][0] = ix;
						upStairCoords[iz][1] = iy;
					}
				}
			}	
		}
	}
		
	fclose(readFile);
	free(fileReader);
}

// generate enemies on the floor 
void generateEnemies(struct gameState * s)
{
	// number of spawn points specifically on the map
	int numSpawnPoints = 0;
	int ix,iy;
	int i = 0;
	
	// set enemies on floor
	if(activeEnemies != NULL)
	{
		free(activeEnemies);
		activeEnemies = NULL;
	}
	
	// variable used to check how many predetermined spawn points there are vs generated number from file 
	int numGenerate = numEnemies;
	
	// allocate memory
	activeEnemies = malloc(numEnemies * sizeof(struct enemies));
	
	// check if the map has any specific enemy spawn points 
	for(iy=0;iy<dungeonSize;iy++)
	{	
		for(ix=0;ix<dungeonSize;ix++)
		{	
			if(d[s->floor][iy][ix] == E && numGenerate > 0)
			{
				activeEnemies[numGenerate-1].speed = rand()%3+1; 
				activeEnemies[numGenerate-1].startTicks = 0;
				activeEnemies[numGenerate-1].x = ix;
				activeEnemies[numGenerate-1].y = iy;
				activeEnemies[numGenerate-1].inCombat = 0;
				
				activeEnemies[numGenerate-1].active = 1;
				activeEnemies[numGenerate-1].type = rand()%3+1;
				activeEnemies[numGenerate-1].npcFighting = -1;
				activeEnemies[numGenerate-1].health = 5;
				numGenerate--;
			}
		}
	}
	
	// if there are no specific spawn point coordinates, randomly spawn the remaining number of enemies 
	if(numGenerate > 0)
	{
		// generate enemies to specific coordinates 
		for(i = 0;i<numEnemies;i++)
		{
			activeEnemies[i].speed = rand()%3+1; 
			activeEnemies[i].x = rand()%dungeonSize;
			activeEnemies[i].startTicks = 0;
			activeEnemies[i].y = rand()%dungeonSize;
			activeEnemies[i].inCombat = 0;
			while(d[s->floor][activeEnemies[i].y][activeEnemies[i].x] == 1)
			{
				activeEnemies[i].x = rand()%dungeonSize;
				activeEnemies[i].y = rand()%dungeonSize;	
			}
			activeEnemies[i].active = 1;
			activeEnemies[i].npcFighting = -1;
			activeEnemies[i].type = rand()%3+1;
			activeEnemies[i].health = 5;
		}
	}
}

// for freeing data when exiting a dungeon 
void freeDungeonData(void *data)
{
	int iy, iz, ix;

	
	if(d != NULL && visible != NULL)
	{
		// free dungeon and visibility array 
		for(iz=0;iz<dungeonSize;iz++)
		{
			for(iy=0;iy<dungeonSize;iy++)
			{		
				free(d[iz][iy]);
				free(visible[iz][iy]);
			}
			free(upStairCoords[iz]);
			free(visible[iz]);
			free(d[iz]);
		}
	}
	
	
	printf("SEE");
	// free enemies/NPCs
	if(activeEnemies != NULL)
		free(activeEnemies);
	
	if(activeNPCs != NULL)
		free(activeNPCs);

	// free status
	if(statusText != NULL)
	{
		for(iy=0;iy<maxStatus;iy++)
		{
			if(statusText[iy] != NULL)
				free(statusText[iy]);
		}
		free(statusText);
	}
	
	statusText = NULL;
	d = NULL;
	visible = NULL;
	activeNPCs = NULL;
	activeEnemies = NULL;
	printf("SEE");
}

// display the dungeon floor and set up used varaiables initially
// used for start of dungeon crawling 
void initDungeonFloor(void *data)
{	
	struct gameState * s = (struct gameState *)data;
	
	// destroy all listeners to make way for ones used in dungeon crawling 
	destroyListeners(s->listeners,MAX_EVENTS);
	
	// reset visibility array if it is empty/deleted
	int iz,ix,iy;
	
	// the dungeon file that needs to be read 
	char * fileName; 

	// set specific conditions based on what dungeon is being initialized 	
	switch(s->building)
	{
		default:
		case 0:
		fileName = ORIGINAL_DUNGEON;
		break;
	}
	
	// read dungeon file 
	readDungeonFile(fileName);
	
	// check if dungeon array is filled with data 
	if(d == NULL)
		throwError("ERROR:DUNGEON FILE NOT COMPATIBLE");
	
	// allocate visible array 
	if(visible == NULL)
	{
		visible = malloc(dungeonSize * sizeof(int **));
		for(iz=0;iz<dungeonSize;iz++)
		{
			visible[iz] = malloc(dungeonSize * sizeof(int *));
			for(iy=0;iy<dungeonSize;iy++)
			{	
				visible[iz][iy] = malloc(dungeonSize * sizeof(int));
				for(ix=0;ix<dungeonSize;ix++)
					visible[iz][iy][ix] = 0;
			}	
		}
	}		
	
	// reset status text
	if(statusText != NULL)
	{
		for(iy=0;iy<maxStatus;iy++)
		{
			free(statusText[iy]);
		}
		free(statusText);
		statusText = NULL;
	}
	
	// generate enemies on the first floor 
	generateEnemies(s);
	
	// initial display range 
	displayRange(s);
	
	if(graphicsMode == 0)
	{
		// display dungeon walls 
		for(iy = -1;iy < dungeonSize+1;iy++)
		{
			for(ix = -1;ix < dungeonSize+1;ix++)
			{
				setCursor(dungeonPrintCoordX+ix,dungeonPrintCoordY+iy);
				if((ix == -1 || ix == dungeonSize || iy == -1 || iy == dungeonSize))
				{	
					setColor(BLUE);
					printf("%c",219);
				}
				else if(visible[s->floor][iy][ix] == 1)
				{
					setColor(WHITE);
					printf("%c",quickConvert(d[s->floor][iy][ix]));
				}
			}
		}		
		setColor(WHITE);
	}
	else
	{
		// initialize images used in dungeon crawling
		s->images = malloc(sizeof(struct image));
		s->numImages = 1;
			
		addImage(s,DUNGEON_SPRITE);
		s->images[0].x = SPRITE_SQUARE_SIZE;
		s->images[0].y = SPRITE_SQUARE_SIZE;
		s->images[0].scale = 2;
		
	}
	
	// generate npcs
	generateNPCs(s->building);

	// set up status handling array 
	numStatusLines = 0;
	statusText = malloc(maxStatus * sizeof(char *));	
	for(iy=0;iy<maxStatus;iy++)
		statusText[iy] = NULL;
	
	// set initial status 
	updateStatus(FIRST_FLOOR_TEXT,s);

	// set to main loop of dungeon in event listener 
	destroyListener(DISPLAY,s->listeners);
	registerEvent(DISPLAY,walkAround,s->listeners);
	
	// start dungeon crawling music 
	switchTrack(DUNGEON_MUSIC,s);
}

#endif