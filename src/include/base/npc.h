#ifndef CHARACTER_HANDLED
#include "stats.h"
#endif

#ifndef NPC_HANDLED
#define NPC_HANDLED

struct npc
{
	int x; // coordinates 
	int y;
	int floor; // what floor the npc is on 
	int active; // is the npc alive 
	int type; // type of npc for dialogue/stats  
	int inCombat; // is the npc fighting an npc 
	int talking; // is the npc talking to the player 
	int speed; // interval the npc moves at 
	int startTicks;
	
	struct character stats; // stats of the npc 
	int reception; // reception to player
	int curiosity; // how curious the npc is of the player 
};

// is the player talking to an npc 
int talking = 0;
int npcNearPlayer = 0; // the npc the player is talking to 
int npcTalked = 0; // the npc the player is talking to 

// variables for npc discussion
int questionAsked = 0; // was a question asked?
int talkOver = 0; // is the talk over? 

int passBy = 0; // can the player pass through the npc 


// npc information for dungeon 
int numNPCs = 0;
struct npc * activeNPCs = NULL;

// generate blank npcs in the dungeon 
void generateNPCs()
{
	int i;
	
	for(i=0;i<numNPCs;i++)
	{
		activeNPCs[i].x = i*2+1; // coordinates 
		activeNPCs[i].y = i*2+1;
		activeNPCs[i].floor = 0; // what floor the npc is on 
		activeNPCs[i].active = 1; // is the npc alive 
		activeNPCs[i].type = 1; // type of npc for dialogue/stats  
		activeNPCs[i].inCombat = 0; // is the npc fighting an npc 
		activeNPCs[i].talking = 0; // is the npc talking to the player 
		activeNPCs[i].speed = 2; // interval the npc moves at 
		activeNPCs[i].startTicks = 0;
		
		activeNPCs[i].reception = 0;
		activeNPCs[i].curiosity = 0;
	}
}



#endif