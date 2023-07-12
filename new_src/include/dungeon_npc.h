// for handling npcs in dungeons 
#ifndef DUNGEON_LOGIC_HANDLED
#include "dungeon_logic.h"
#endif

#ifndef DUNGEON_DISPLAY_HANDLED
#include "dungeon_display.h"
#endif

#ifndef DUNGEON_NPC__HANDLED
#define DUNGEON_NPC__HANDLED

// function to check if an npc is at a certain coordinate 
int npcNearby(int x, int y, int f, int isPlayer)
{
	/*
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
	*/
	return 0;
}

#endif