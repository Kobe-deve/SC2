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

#endif