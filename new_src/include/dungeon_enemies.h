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
	int numSpawnPoints = 0;
	int ix,iy;
	int i = 0;
	
	// set enemies on floor
	if(state->activeEnemies != NULL)
	{
		free(state->activeEnemies);
		state->activeEnemies = NULL;
	}
	
	// variable used to check how many predetermined spawn points there are vs generated number from file 
	int numGenerate = state->numEnemies;
	
	// allocate memory
	state->activeEnemies = malloc(state->numEnemies * sizeof(struct enemies));
	
	// check if the map has any specific enemy spawn points 
	for(iy=0;iy<state->dungeonSize;iy++)
	{	
		for(ix=0;ix<state->dungeonSize;ix++)
		{	
			if(state->d[state->floor][iy][ix] == E && numGenerate > 0)
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
			}
		}
	}
	
	// if there are no specific spawn point coordinates, randomly spawn the remaining number of enemies 
	if(numGenerate > 0)
	{
		// generate enemies to specific coordinates 
		for(i = 0;i<state->numEnemies;i++)
		{
			state->activeEnemies[i].speed = rand()%3+1; 
			state->activeEnemies[i].x = rand()%state->dungeonSize;
			state->activeEnemies[i].startTicks = 0;
			state->activeEnemies[i].y = rand()%state->dungeonSize;
			state->activeEnemies[i].inCombat = 0;
			while(state->d[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1)
			{
				state->activeEnemies[i].x = rand()%state->dungeonSize;
				state->activeEnemies[i].y = rand()%state->dungeonSize;	
			}
			state->activeEnemies[i].active = 1;
			state->activeEnemies[i].npcFighting = -1;
			state->activeEnemies[i].type = rand()%3+1;
			state->activeEnemies[i].health = 5;
		}
	}
}


// moves enemies on the floor 
void enemyHandler(struct gameState * state)
{
	int i = 0;
	int j,enemyHere;
	int cYo;
	int directionX, directionY;

	for(i = 0;i<state->numEnemies;i++)
	{
		enemyHere = 0;
		
		directionY = 0;
		directionX = 0;
		if(state->activeEnemies[i].inCombat == 0 && state->activeEnemies[i].active == 1 && ((int)(SDL_GetTicks() - state->activeEnemies[i].startTicks))/1000.f >= state->activeEnemies[i].speed)
		{
			// erase/update current spot when moving 
			if(state->graphicsMode == 0 && state->visible[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 1)
			{
				// make sure there isn't another enemy on this spot if updating 
				setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordX+state->activeEnemies[i].y);
				printf("%c",quickConvert(state->d[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x]));	
			}	
			else if(/*!sleeping && conversation == NONE &&*/ state->visible[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x] == 0) // display status that enemies are moving in the dark 
				updateStatus(ENEMY_MOVEMENT,state);	
			
			
			if(state->activeEnemies[i].x < state->dungeonSize-1 && state->d[state->floor][state->activeEnemies[i].y][state->activeEnemies[i].x+1] != 1)
				state->activeEnemies[i].x++;
			/*
			TODO fix enemy movement logic
			
			// set up direction variables 
			if(state->playerX > activeEnemies[i].x)
				directionX = 1;
			else if(state->playerX < activeEnemies[i].x)
				directionX = 3;
				
			if(state->playerY > activeEnemies[i].y)
				directionY = 1;
			else if(state->playerY < activeEnemies[i].y)
				directionY = 2;
			
			int cY = activeEnemies[i].y;
			int cX = activeEnemies[i].x; 
			cYo = 0;
			
			// move in y direction 
			switch(directionY)
			{
				case 1:
				if(cY < dungeonSize-1 && d[state->floor][cY+1][cX] != 1)
					activeEnemies[i].y++;
				break;
				case 2:
				if(cY > 0 && d[state->floor][cY-1][cX] != 1)
					activeEnemies[i].y--;
				break;
			}
			
			// move in x direction 
			cYo = cY;
			cY = activeEnemies[i].y;
			
			switch(directionX)
			{
				case 1:
				if(cX < dungeonSize-1 && d[state->floor][cY][cX+1] != 1)
					activeEnemies[i].x++;
				break;
				case 3:
				if(cX > 0 && d[state->floor][cY][cX-1] != 1)
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
			*/
			
			// starting fight with npc 
			int checkNPC = -1;
		
			for(j=0;j<state->numNPCs;j++)
			{
				if(state->activeEnemies[j].active == 1 && (state->activeNPCs[j].floor == state->floor && (state->activeEnemies[i].y == state->activeEnemies[j].y && state->activeEnemies[i].x == state->activeNPCs[j].x)))
				{
					checkNPC = j;
					break;
				}
			}

			
			// if npc found, start battle 
			if(checkNPC != -1 && checkNPC < state->numNPCs)
			{
				printf("%d",checkNPC);
				updateStatus(HEAR_FIGHT,state);
				state->activeEnemies[i].inCombat = 1;
				state->activeEnemies[i].npcFighting = checkNPC;
				state->activeNPCs[checkNPC].passBy = 0;
				state->activeNPCs[checkNPC].inCombat = 1;
				state->activeNPCs[checkNPC].enemyCombat = i;
			}
			
			// reset start tick 
			state->activeEnemies[i].startTicks = SDL_GetTicks();
		}
		else if(state->activeEnemies[i].inCombat == 1 && ((int)(SDL_GetTicks() - state->activeEnemies[i].startTicks))/1000.f >= state->activeEnemies[i].speed) // handle fighting an npc over time 
		{
			// update status 
			updateStatus(NPCvsENEMY,state);	
			
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
			setCursor(dungeonPrintCoordX+state->activeEnemies[i].x,dungeonPrintCoordX+state->activeEnemies[i].y);
			printf("+");
		}
	}
}


#endif