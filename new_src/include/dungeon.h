// for general handling dungeon crawling in the game 

// convert specific tiles in dungeon to type
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

// coord to print dungeon at 
int dungeonPrintCoordX = 1;
int dungeonPrintCoordY = 1;

void generateEnemies(struct gameState * state);

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
	int iz,ix,iy;
	
	// display dungeon process
	switch(state->graphicsMode)
	{
		case 0: // ascii mode 
		
		if(state->megaAlpha == 0) //  initialize display 
		{					
			// display dungeon walls 
			for(iy = -1;iy < state->dungeonSize+1;iy++)
			{
				for(ix = -1;ix < state->dungeonSize+1;ix++)
				{
					setCursor(dungeonPrintCoordX+ix,dungeonPrintCoordY+iy);
					if((ix == -1 || ix == state->dungeonSize || iy == -1 || iy == state->dungeonSize))
					{	
						setColor(BLUE);
						printf("%c",219);
					}
					else if(state->visible[state->floor][iy][ix] == 1)
					{
						setColor(WHITE);
						printf("%c",quickConvert(state->d[state->floor][iy][ix]));
					}
				}
			}		
			setColor(WHITE);
		
			state->megaAlpha = 1;
		}		
		
		// display player
		setCursor(dungeonPrintCoordX+state->playerX,dungeonPrintCoordY+state->playerY);
		printf("%c",1);	
		
		break;
		case 1: // sprite mode
		
		// get dungeon image assets initialized if screen is cleared out
		if(state->numImages == 0)
		{
			// initialize images used in dungeon crawling
			state->images = malloc(sizeof(struct image));
			state->numImages = 1;
				
			addImage(state,DUNGEON_SPRITE);
			state->images[0].x = SPRITE_SQUARE_SIZE;
			state->images[0].y = SPRITE_SQUARE_SIZE;
			state->images[0].scale = 2;
		}
		else
		{
			// display dungeon 
			for(iy=0;iy<state->dungeonSize;iy++)
			{
				for(ix=0;ix<state->dungeonSize;ix++)
				{	
					if(state->visible[state->floor][iy][ix])
					{
						
						switch(state->d[state->floor][iy][ix])
						{
							case 1: // wall
							state->spriteClip.x = SPRITE_SQUARE_SIZE*2;
							state->spriteClip.y = 0;
							break;
							case 2: // up stairs 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
							
							state->spriteClip.x = SPRITE_SQUARE_SIZE*3;
							state->spriteClip.y = 0;
							break;
							case 3: // down stairs
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*6;
							state->spriteClip.y = 0;
							break;
							case 4: // chest 
							state->spriteClip.x = SPRITE_SQUARE_SIZE*4;
							state->spriteClip.y = 0;
							break;
							case 5: // opened chest 
							state->spriteClip.x = SPRITE_SQUARE_SIZE*5;
							state->spriteClip.y = 0;
							break;
							case 9: // shop
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							state->images[0].x = ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
							state->images[0].y = iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
							renderImage(&state->images[0], state->renderer,&state->spriteClip);
						
							state->spriteClip.x = SPRITE_SQUARE_SIZE*9;
							state->spriteClip.y = 0;
							break;
							default: // floor 
							state->spriteClip.x = SPRITE_SQUARE_SIZE;
							state->spriteClip.y = 0;
							break;
						}
					
						state->images[0].x = ix*SPRITE_SQUARE_SIZE*state->images[0].scale;
						state->images[0].y = iy*SPRITE_SQUARE_SIZE*state->images[0].scale;
						renderImage(&state->images[0], state->renderer,&state->spriteClip);
					}
				}
			}
			
			// display player 
			state->spriteClip.x = 0;
			state->spriteClip.y = 0;
							
			state->images[0].x = state->playerX*SPRITE_SQUARE_SIZE*state->images[0].scale;
			state->images[0].y = state->playerY*SPRITE_SQUARE_SIZE*state->images[0].scale;
			renderImage(&state->images[0], state->renderer,&state->spriteClip);
		}			
		
		break;
	}
}

// general logic handler for dungeon crawling 
void dungeonLogic(struct gameState * state)
{
	// clear the screen before movement 
	if(state->graphicsMode == 0)
		clearDisplay(state);
	
	// player movement logic 
	dungeonMovement(state);
}

#endif
