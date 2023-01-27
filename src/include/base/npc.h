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
	int enemyCombat; // what enemy the npc is in combat with 
	int talking; // is the npc talking to the player 
	int speed; // interval the npc moves at 
	int startTicks;
	
	char * name; // name of the npc 
	struct character stats; // stats of the npc 
	int reception; // reception to player
	int curiosity; // how curious the npc is of the player 
};

// conversation phase enumerations
enum conversationPhase
{
	NONE = -1,
	NO_DISCUSS = -2,
	GREETING = 0,
	QUESTION = 1,
	PASS = 2,
	NPC_RESPONSE = 3,
	PASS_BY = 4,
	END = 5,
	BATTLE = 6,
	WAIT_RESPONSE = 7,
	LEAVE = 8
};

int conversation = NONE;

int npcNearPlayer = 0; // the npc the player is talking to 
int npcTalked = 0; // the npc the player is talking to 
int talkOver = 0; // is the conversation over?
int topicNum = 0; // what topic is being used? 
 
// npc information for dungeon 
int numNPCs = 0;
struct npc * activeNPCs = NULL;

// preemptively declare update status used in dungeon crawling 
void updateStatus(char * text);

// generate blank npcs in the dungeon 
void generateNPCs()
{
	int i;
	
	for(i=0;i<numNPCs;i++)
	{
		activeNPCs[i].name = ""; // name
		activeNPCs[i].x = i*2+1; // coordinates 
		activeNPCs[i].y = i*2+1;
		activeNPCs[i].floor = 0; // what floor the npc is on 
		activeNPCs[i].active = 1; // is the npc alive 
		activeNPCs[i].type = 1; // type of npc for dialogue/stats  
		activeNPCs[i].inCombat = 0; // is the npc fighting an npc 
		activeNPCs[i].talking = 0; // is the npc talking to the player 
		activeNPCs[i].speed = 2; // interval the npc moves at 
		activeNPCs[i].startTicks = 0;
		activeNPCs[i].enemyCombat = -1;
		
		activeNPCs[i].reception = 0;
		activeNPCs[i].curiosity = 0;
	}
}

// handles dialogue process with npc
void npcDialogueHandler(struct gameState * s)
{
	switch(conversation)
	{
		case GREETING:
		conversation = NPC_RESPONSE;
		updateStatus("Person: \"Yeah hello.\"");	
		break;
		case QUESTION:
		conversation = NPC_RESPONSE;
		updateStatus("Person: \"What? Sorry I don't care.\"");	
		break;
		case PASS:
		updateStatus("Person: \"Nah.\"");	
		conversation = NPC_RESPONSE;
		break;
		
		case NPC_RESPONSE:
		updateStatus("Person: \"I have nothing else to say to you leave me alone.\"");	
		talkOver = 1;
		conversation = LEAVE;
		break;
	}
}

#endif