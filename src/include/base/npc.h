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
	int passBy; // can pass by the npc 
	
	struct character stats; // stats of the npc 
	int reception; // reception to player
	int curiosity; // how curious the npc is of the player 
};

// conversation phase enumerations
enum conversationPhase
{
	NONE = -1, // no conversation happening 
	NO_DISCUSS = -2, // base loop of asking a question, to pass, or to leave 
	INTRO = -3, // initial start of convesation with greeting, question, or ask to pass 
	GREETING = 0, // greeting option 
	QUESTION = 1, // asking a question to the npc 
	PASS = 2, // asking to pass 
	NPC_RESPONSE = 3,
	BATTLE = 6, // starting fight with npc (will be followed by pressing enter) 
	WAIT_RESPONSE = 7,
	PASS_BY = 9,
	LEAVE = 8 // end of conversation 
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
void generateNPCs(int dungeonType)
{
	int i;
	
	for(i=0;i<numNPCs;i++)
	{
		activeNPCs[i].x = i*2+1; // coordinates 
		activeNPCs[i].y = i*2+1;
		activeNPCs[i].floor = 0; // what floor the npc is on 
		activeNPCs[i].active = 1; // is the npc alive 
		activeNPCs[i].type = i; // type of npc for dialogue/stats  
		activeNPCs[i].inCombat = 0; // is the npc fighting an npc 
		activeNPCs[i].talking = 0; // is the npc talking to the player 
		activeNPCs[i].speed = 2; // interval the npc moves at 
		activeNPCs[i].startTicks = 0;
		activeNPCs[i].enemyCombat = -1;
		activeNPCs[i].passBy = 0;
		
		activeNPCs[i].reception = 0;
		activeNPCs[i].curiosity = 0;
	}
}

// handles dialogue process with npc
void npcDialogueHandler(int spot, struct gameState * s)
{	
	switch(activeNPCs[spot].type)
	{
		case 0:
		break;
		case 1:
		break;
	}
	
	switch(conversation)
	{
		case NO_DISCUSS:
		if(s->listeners[MENU_SELECTION] == NULL)
		{
			registerEvent(MENU_SELECTION,menuSelection,s->listeners);
			char ** array = malloc(3 * sizeof(char*));
			array[0] = Q_COMMAND;
			array[1] = P_COMMAND;
			array[2] = L_COMMAND;
			initMenu(s,3,array,70,31);
	
			free(array);		
		}
		break;
		case GREETING:
		conversation = NO_DISCUSS;
		updateStatus("Person: \"Yeah hello.\"");	
		updateStatus("Person: \"What do you want?\"");	
		break;
		case QUESTION:
		//conversation = NPC_RESPONSE;
		conversation = BATTLE;
		updateStatus("Person: \"What? Sorry I don't care.\"");	
		break;
		case PASS:
		updateStatus("Person: \"Sure.\"");	
		conversation = PASS_BY;
		activeNPCs[spot].passBy = 1;
		break;
		case BATTLE:
		updateStatus("Person: \"Die :).\"");	
		updateStatus("Press Enter.");	
		talkOver = 1;
		break;
		
		case NPC_RESPONSE:
		updateStatus("Person: \"I have nothing else to say to you leave me alone.\"");	
		talkOver = 1;
		conversation = LEAVE;
		break;
	}
}

#endif