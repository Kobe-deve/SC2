#ifndef CHARACTER_HANDLED
#include "stats.h"
#endif

#ifndef TEXT_DEFINED
#include "base/text.h"
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
	int trust; // how much the npc trusts the player 
	int goal; // the goals of the npc 
	int direction;
	
	int numSaved; // number of times the player has stepped into a fight an npc had with a curse 
	int numPassed; // number of times the player has passed by the npc 
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
	NPC_RESPONSE = 3, // npc response 
	NPC_QUESTION = 4, // npc asking a question 
	PLAYER_RESPONSE = 5, // player responding to a question 
	BATTLE = 6, // starting fight with npc (will be followed by pressing enter) 
	WAIT_RESPONSE = 7, 
	PASS_BY = 9, // player can pass by 
	LEAVE = 8 // end of conversation 
};

// specific NPC goals used for dialogue/movement 
enum goals
{
	SURVIVE_DUNGEON,
	ESCAPE_DUNGEON,
};

int conversation = NONE;

int npcNearPlayer = 0; // the npc the player is talking to 
int npcTalked = 0; // the npc the player is talking to (from the activeNPCs list)
int talkOver = 0; // is the conversation over?
int topicNum = 0; // what topic is being used? For NPC/Player questions  
 
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
		activeNPCs[i].direction = 0;
		
		activeNPCs[i].speed = rand()%3+4; // interval the npc moves at 
		activeNPCs[i].startTicks = 0;
		activeNPCs[i].enemyCombat = -1;
		
		activeNPCs[i].reception = 0; // conversation variables 
		activeNPCs[i].curiosity = 10;
		activeNPCs[i].numSaved = 0;
		activeNPCs[i].numPassed = 0;
		activeNPCs[i].passBy = 0;
		activeNPCs[i].goal = ESCAPE_DUNGEON;
		
		activeNPCs[i].stats = generateCharacter(HUMAN); // set up stats 
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
		case NO_DISCUSS: // base loop of conversation 
		if(s->listeners[MENU_SELECTION] == NULL) // generate list of commands 
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
		case GREETING: // npc response to greeting 
		updateStatus("Person: \"Yeah hello.\"");	
		updateStatus("Person: \"What do you want?\"");	
		conversation = NO_DISCUSS;
		break;
		case QUESTION: // npc response to question or generate list of questions 
		//conversation = NPC_RESPONSE;
		conversation = NPC_QUESTION;
		updateStatus("Person: \"What? Sorry I don't care.\"");	
		break;
		case PASS: // npc response to passing by 
		updateStatus("Person: \"Sure.\"");	
		
		conversation = PASS_BY;
		activeNPCs[spot].passBy = 1;
		activeNPCs[spot].numPassed++; // if the player can pass by, increase number of times passed 
		
		break;
		case BATTLE: // npc fights player 
		updateStatus(NPCBAT);	
		updateStatus(PRESS_ENTER);	
		talkOver = 1;
		break;
		case NPC_QUESTION: // NPC asks a question 
		
		// pick question to ask the player 
		topicNum = rand()%2+1;
		
		// free menu if it isn't freed
		if(s->listeners[MENU_SELECTION] != NULL)   
			freeMenuProcess(s);	
			
		char ** array;
				
		// generate the question and list of responses to give the player 
		switch(topicNum)
		{
			case 1:
			updateStatus(QUESTION1);	
			s->numOptions = 2;
			registerEvent(MENU_SELECTION,menuSelection,s->listeners);
			break;
			case 2:
			updateStatus(QUESTION2);	
			s->numOptions = 3;	
			registerEvent(MENU_SELECTION,menuSelection,s->listeners);
			break;
		}
		array = generateResponses(topicNum-1);
		
		// initialize menu of responses 
		initMenu(s,s->numOptions,array,70,31);
		free(array);		
	
		break;
		case PLAYER_RESPONSE: // respond based on what the player responded with 
		updateStatus("Person: \"Ah okay then, bye.\"");	
		talkOver = 1;
		conversation = LEAVE;
		
		// the more the player answers questions the NPC asks, the lower the NPC's curiosity 
		if(activeNPCs[spot].curiosity > 0)
			activeNPCs[spot].curiosity--;
		
		break;
		case NPC_RESPONSE: // NPC responds
		updateStatus(NPCRESP);	
		talkOver = 1;
		conversation = LEAVE;
		break;
	}
}

#endif