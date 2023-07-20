// for general handling dungeon crawling in the game 

// coord to print dungeon at in ascii mode 
int dungeonPrintCoordX = 1;
int dungeonPrintCoordY = 1;

// coord to print dungeon at in sprite mode 
int spriteDungeonPrintCoordX = (WINDOW_WIDTH-100)/2;
int spriteDungeonPrintCoordY = (WINDOW_HEIGHT-100)/2;

void generateEnemies(struct gameState * state);
int npcNearby(int x, int y, int f, int isPlayer, struct gameState * state);
int nearbyBlocks(struct gameState * state, int action);

// check if the position is passable 
int passableBlock(int x, int y, struct gameState * state)
{
	// checks if there is an impassable block
	switch(state->d[state->floor][y][x])
	{
		case 1:
		case D:
		case A:
		case G:
		case B:
		case M:
		return 0;
		break;
	}
	
	// true
	return 1;
}

#ifndef DUNGEON_ENEMIES__HANDLED
#include "dungeon_enemies.h"
#endif

#ifndef DUNGEON_NPC__HANDLED
#include "dungeon_npc.h"
#endif

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#include "dungeon_display.h"
#endif

#ifndef DUNGEON_HANDLED
#define DUNGEON_HANDLED

// general dungeon display function 
void dungeonDisplay(struct gameState * state)
{	
	// reset visibility array if it is empty/deleted
	int i,iz,ix,iy;
	
	// display dungeon process
	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		
		if(state->megaAlpha == 0) //  initialize display 
		{		
			initDungeonDisplay(state);
			state->megaAlpha = 1;
		}		
		
		// display player
		setCursor(dungeonPrintCoordX+state->playerX,dungeonPrintCoordY+state->playerY);
		printf("%c",1);	
		
		break;
		case 1: // sprite mode
		
		if(state->numImages == 0) // get dungeon image assets initialized if screen is cleared out
		{
			initDungeonDisplay(state);
		}
		else // general display operations 
		{
			// display dungeon 
			for(iy=0;iy<state->dungeonSize;iy++)
			{
				for(ix=0;ix<state->dungeonSize;ix++)
				{	
					if(state->visible[state->floor][iy][ix]) // visibility setting 
					{
						
						switch(state->d[state->floor][iy][ix])
						{
							case D: // hidden door (closed) 
							case 1: // wall
							state->spriteClip.x = SPRITE_SQUARE_SIZE*2;
							state->spriteClip.y = 0;
							break;
							case 2: // up stairs 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
							
							state->spriteClip.x = SPRITE_SQUARE_SIZE*3;
							state->spriteClip.y = 0;
							break;
							case 3: // down stairs
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*6;
							state->spriteClip.y = 0;
							break;
							case 4: // chest 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*4;
							state->spriteClip.y = 0;
							break;
							case 5: // opened chest 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*5;
							state->spriteClip.y = 0;
							break;
							case 9: // shop
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*9;
							state->spriteClip.y = 0;
							break;
							
							case B: // door
							state->spriteClip.x = SPRITE_SQUARE_SIZE*12;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
							break;
							
							case A: // switch (closed)
							state->spriteClip.x = SPRITE_SQUARE_SIZE*14;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
							break;
							
							case G: // switch (open)
							state->spriteClip.x = SPRITE_SQUARE_SIZE*13;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
							break;
							
							case M: // sign 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*17;
							state->spriteClip.y = 0;
							state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							break;
							
							default: // floor 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							break;
						}
					
						state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
						state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
						renderImage(&state->images[0], state->renderer,&state->spriteClip);
					}
					else
					{
						state->spriteClip.x = SPRITE_SQUARE_SIZE*7;
						state->spriteClip.y = 0;
						state->images[0].x = spriteDungeonPrintCoordX+ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
						state->images[0].y = spriteDungeonPrintCoordY+iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
						renderImage(&state->images[0], state->renderer,&state->spriteClip);
					}
				}
			}
			
			// display enemies 
			for(i = 0;i<state->numEnemies;i++)
			{		
				if(state->visible[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1 && state->activeEnemies[i].inCombat == 0 && state->activeEnemies[i].active == 1)
				{				
					state->spriteClip.x = SPRITE_SQUARE_SIZE*8;
					state->spriteClip.y = 0;
					state->images[0].x = spriteDungeonPrintCoordX+state->activeEnemies[i].x*SPRITE_SQUARE_SIZE*state->images[0].scale;
					state->images[0].y = spriteDungeonPrintCoordY+state->activeEnemies[i].y*SPRITE_SQUARE_SIZE*state->images[0].scale;
					renderImage(&state->images[0], state->renderer,&state->spriteClip);			
				}
			}	
			
			// display npc 
			for(i = 0;i<state->numNPCs;i++)
			{		
				if(state->activeNPCs[i].active && state->visible[state->activeNPCs[i].floor][state->activeNPCs[i].y][state->activeNPCs[i].x] == 1 && state->floor == state->activeNPCs[i].floor)
				{
					// change sprite based on situation the npc is in 
					if(state->activeNPCs[i].inCombat == 1)
						state->spriteClip.x = SPRITE_SQUARE_SIZE*11;
					else	
						state->spriteClip.x = SPRITE_SQUARE_SIZE*10;
					
					state->spriteClip.y = 0;
							
					state->images[0].x = spriteDungeonPrintCoordX+state->activeNPCs[i].x*SPRITE_SQUARE_SIZE*state->images[0].scale;
					state->images[0].y = spriteDungeonPrintCoordY+state->activeNPCs[i].y*SPRITE_SQUARE_SIZE*state->images[0].scale;
					renderImage(&state->images[0], state->renderer,&state->spriteClip);
				}
			}
			
			// display player 
			state->spriteClip.x = 0;
			state->spriteClip.y = 0;
							
			state->images[0].x = spriteDungeonPrintCoordX+state->playerX*SPRITE_SQUARE_SIZE*state->images[0].scale;
			state->images[0].y = spriteDungeonPrintCoordY+state->playerY*SPRITE_SQUARE_SIZE*state->images[0].scale;
			renderImage(&state->images[0], state->renderer,&state->spriteClip);
			
			// display in-game status 
			displayStatus(state);
		}			
		
		break;
	}
}

// general logic handler for dungeon crawling 
void dungeonLogic(struct gameState * state)
{
	int i;
	
	// clear the screen before movement 
	if(state->graphicsMode == 0)
		clearDisplay(state);
	
	// player movement logic 
	if(state->options == NULL) // if player isn't interacting with an object/character they can move 
		dungeonMovement(state);
	else // basic menu handling 
	{
		handleMenu(state);
		
		if(state->activeNPCs[state->nearestNPC].inCombat == 1 || state->input == BACKSPACE) // handle menu logic 
		{
			clearMenu(state);
			deallocateMenu(state);	
			state->activeNPCs[state->nearestNPC].talking = 0;
		}
	}
	
	// npc handler
	npcHandler(state);
	
	// enemy handler 
	enemyHandler(state);
	
	// check if player has run into enemies, if so start a battle  
	for(i=0;i<state->numEnemies;i++)
	{
		if(state->activeEnemies[i].active == 1 && state->activeEnemies[i].y == state->playerY && state->activeEnemies[i].x == state->playerX)
		{
			state->activeEnemies[i].active = 0;
			updateStatus(ENCOUNTERED,state);
	
			state->switchSystem = 1;
			state->switchTo = BATTLE_SCREEN;
		}
	}
}

#endif
