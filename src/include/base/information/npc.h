// holds all the npc information for dungeons
#ifndef NPC_HANDLED
#define NPC_HANDLED

// structure for npcs in the dungeon 
struct npc
{
	int x; // coordinates 
	int y;
	int floor; // what floor the npc is on 
	int active; // is the npc alive 
	int type; // type of npc for dialogue/stats  
	int inCombat; // is the npc fighting an npc 
	int enemyCombat; // what enemy the npc is in combat with 
	int talking; // is the npc talking to the player 
	int speed; // interval the npc moves at 
	int startTicks;
	int passBy; // can pass by the npc 
	int direction; // what direction the NPC is facing
	
	struct character stats; // stats of the npc 
	
	int numSaved; // number of times the player has stepped into a fight an npc had with a curse 
	int numPassed; // number of times the player has passed by the npc 
};


#endif
