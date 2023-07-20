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

#ifndef DUNGEON_NPC__HANDLED
#include "dungeon_npc.h"
#endif

#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#include "dungeon_display.h"
#endif

#ifndef DUNGEON_ENEMIES__HANDLED
#define DUNGEON_ENEMIES__HANDLED

// generate enemies on the floor 
void generateEnemies(struct gameState * state)
{
	// number of spawn points specifically on the map
	int numGenerate = 0;
	int i = 0;
	int ix,iy;
	
	state->numEnemies = 0;
	
	// set enemies on floor
	if(state->activeEnemies != NULL)
	{
		free(state->activeEnemies);
		state->activeEnemies = NULL;
	}
	
	// variable used to check how many predetermined spawn points there are vs generated number from file 
	for(iy=0;iy<state->dungeonSize;iy++)
	{	
		for(ix=0;ix<state->dungeonSize;ix++)
		{	
			if(state->d[state->floor][iy][ix] == E)
				state->numEnemies++;
		}
	}
	
	// allocate memory
	state->activeEnemies = malloc(state->numEnemies * sizeof(struct enemies));
	
	numGenerate = state->numEnemies;
	
	// check if the map has any specific enemy spawn points 
	for(iy=0;iy<state->dungeonSize;iy++)
	{	
		for(ix=0;ix<state->dungeonSize;ix++)
		{	
			if(state->d[state->floor][iy][ix] == E)
			{
				state->activeEnemies[numGenerate-1].speed = rand()%3+1; 
				state->activeEnemies[numGenerate-1].startTicks = 0;
				state->activeEnemies[numGenerate-1].x = ix;
				state->activeEnemies[numGenerate-1].y = iy;
				state->activeEnemies[numGenerate-1].inCombat = 0;
				
				state->activeEnemies[numGenerate-1].active = 1;
				state->activeEnemies[numGenerate-1].type = rand()%3+1;
				state->activeEnemies[numGenerate-1].npcFighting = -1;
				state->activeEnemies[numGenerate-1].health = 5;
				numGenerate--;
				updateStatus(ENEMY_MOVEMENT,state);	
			}
		}
	}
}


// moves enemies on the floor 
void enemyHandler(struct gameState * state)
{
	int i = 0;
	int j,enemyHere;
	int cYo;
	int direction;

	for(i = 0;i<state->numEnemies;i++)
	{
		enemyHere = 0;
		
		direction = 0;
		if(state->activeEnemies[i].inCombat == 0 && state->activeEnemies[i].active == 1 && ((int)(SDL_GetTicks() - state->activeEnemies[i].startTicks))/1000.f >= state->activeEnemies[i].speed)
		{
			// erase/update current spot when moving 
			if(state->graphicsMode == 0 && state->visible[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1)
			{
				// make sure there isn't another enemy on this spot if updating 
				setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordY+state->activeEnemies[i].y);
				printf("%c",quickConvert(state->d[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x]));	
			}	
			
			// determine movement 
			if(state->playerX > state->activeEnemies[i].x)
				direction = 1;
			else if(state->playerX < state->activeEnemies[i].x)
				direction = 3;
			else if(state->playerY > state->activeEnemies[i].y)
				direction = 2;
			else if(state->playerY < state->activeEnemies[i].y)
				direction = 0;
			else 
				direction = rand()%4;
			
			// execute movement 
			switch(direction)
			{
				case 0: // UP
				if(state->activeEnemies[i].y - 1 >= 0 && passableBlock(state->activeEnemies[i].x,state->activeEnemies[i].y-1,state))
					state->activeEnemies[i].y = state->activeEnemies[i].y - 1;				
				break;
				case 1: // RIGHT
				if(state->activeEnemies[i].x + 1 < state->dungeonSize && passableBlock(state->activeEnemies[i].x+1,state->activeEnemies[i].y,state))
					state->activeEnemies[i].x = state->activeEnemies[i].x + 1;				
				break;
				case 2: // DOWN
				if(state->activeEnemies[i].y + 1 < state->dungeonSize && passableBlock(state->activeEnemies[i].x,state->activeEnemies[i].y+1,state))
					state->activeEnemies[i].y = state->activeEnemies[i].y + 1;
				break;
				case 3: // LEFT
				if(state->activeEnemies[i].x - 1 >= 0 && passableBlock(state->activeEnemies[i].x-1,state->activeEnemies[i].y,state))
					state->activeEnemies[i].x = state->activeEnemies[i].x - 1;
				break;
			}
			
			
			// starting fight with npc 
			int checkNPC = -1;
		
			for(j=0;j<state->numNPCs;j++)
			{
				if(state->activeNPCs[j].active == 1 && (state->activeNPCs[j].floor == state->floor && (state->activeEnemies[i].y == state->activeNPCs[j].y && state->activeEnemies[i].x == state->activeNPCs[j].x)))
				{
					checkNPC = j;
					break;
				}
			}

			
			// if npc found, start battle 
			if(checkNPC != -1 && checkNPC < state->numNPCs)
			{
				if(state->graphicsMode == 0)
					printf("%d",checkNPC);
				updateStatus(HEAR_FIGHT,state);
				state->activeEnemies[i].inCombat = 1;
				state->activeEnemies[i].npcFighting = checkNPC;
				state->activeNPCs[checkNPC].passBy = 0;
				state->activeNPCs[checkNPC].inCombat = 1;
				state->activeNPCs[checkNPC].enemyCombat = i;
				
				// update status 
				updateStatus(NPCvsENEMY,state);	
			}
			
			// reset start tick 
			state->activeEnemies[i].startTicks = SDL_GetTicks();
		}
		else if(state->activeEnemies[i].inCombat == 1 && ((int)(SDL_GetTicks() - state->activeEnemies[i].startTicks))/1000.f >= state->activeEnemies[i].speed) // handle fighting an npc over time 
		{
			// change stats based on combat 
			switch(rand()%4)
			{
				case 0: // npc takes damage
				state->activeNPCs[state->activeEnemies[i].npcFighting].stats.health--;
				break;
				case 1: // curse takes some sort of damage 
				case 2:
				state->activeEnemies[i].health--;
				break;
				default: // nothing 
				break;
			}
			
			// check if NPC was defeated, if so deactivate NPC and get enemy moving again 
			if(state->activeNPCs[state->activeEnemies[i].npcFighting].stats.health <= 0)
			{
				updateStatus(NPCLOST,state);	
				
				state->activeNPCs[state->activeEnemies[i].npcFighting].active = 0;
				state->activeNPCs[state->activeEnemies[i].npcFighting].inCombat = 0;
				state->activeEnemies[i].inCombat = 0;
				
			}
			else if(state->activeEnemies[i].health <= 0) // check if enemy was defeated 
			{
				updateStatus(NPCWON,state);
	
				state->activeNPCs[state->activeEnemies[i].npcFighting].inCombat = 0;
				state->activeEnemies[i].inCombat = 0;
				state->activeEnemies[i].active = 0;
			}
			
			// reset start tick 
			state->activeEnemies[i].startTicks = SDL_GetTicks();
		}
		
		// display enemy 
		if(state->graphicsMode == 0 && state->visible[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1 && state->activeEnemies[i].inCombat == 0 && state->activeEnemies[i].active == 1)
		{
			setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordY+state->activeEnemies[i].y);
			printf("+");
		}
	}
}


#endif