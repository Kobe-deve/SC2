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
	JOIN_PARTY = 10, // npc joins the player's party 
	LEAVE = 8 // end of conversation 
};

// specific NPC goals used for dialogue/movement 
enum goals
{
	SURVIVE_DUNGEON = 1,
	ESCAPE_DUNGEON = 2,
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

// load npcs from a file based on the dungeon type 
void generateNPCs(int dungeonType)
{
	FILE *readFile;
	char * fileReader = malloc(128 * sizeof(char)); 
	
	int i;
	
	switch(dungeonType)
	{
		default:
		case 0:
		readFile = fopen(ORIGINAL_DUNGEON_NPC,"r");
		break;
	}
	
	if(!readFile)
	{
		printf("ERROR:DUNGEON NPC FILE COULD NOT BE READ");
		getchar();
		exit(0);
	}
	else
	{
		// read number of npcs
		fscanf(readFile,"%s",fileReader);
		numNPCs = atoi(fileReader);
		
		activeNPCs = malloc(numNPCs * sizeof(struct npc));
	
		fscanf(readFile,"%s",fileReader);
		for(i=0;i<numNPCs;i++)
		{
			// define variables from file 
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].x = atoi(fileReader);
		
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].y = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].floor = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].type = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].speed = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].reception = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].curiosity = atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			activeNPCs[i].goal = (enum goals)atoi(fileReader);
			
			fscanf(readFile,"%s",fileReader);
			switch(atoi(fileReader))
			{
				default:
				activeNPCs[i].stats = generateCharacter(HUMAN); // set up stats 
				break;
			}
			
			// set default for conditions
			activeNPCs[i].inCombat = 0;  
			activeNPCs[i].talking = 0;  
			activeNPCs[i].direction = 0;
			activeNPCs[i].active = 1; 
			activeNPCs[i].startTicks = 0;
			activeNPCs[i].enemyCombat = -1;
			activeNPCs[i].passBy = 0;
			
			activeNPCs[i].numSaved = 0;
			activeNPCs[i].numPassed = 0;
			
			// skip last line 
			fscanf(readFile,"%s",fileReader);
		}
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
		conversation = JOIN_PARTY;
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
		
		case JOIN_PARTY: // npc joins the party 
		updateStatus(NPCJOINED);	
		updateStatus(PRESS_ENTER);	
		talkOver = 1;
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