// handles quest logic 
#ifndef QUEST_HANDLED
#define QUEST_HANDLED

// data structure for quests in the game 
struct quest
{
	int questType; // the type of quest 
	
	int progress; // counter for progress on a specific quest 
	int maxProg; // max amount of progress made on the quest till it is finished 
	
	int completed; // is the quest completed
};

// specific quest info 

#define QUEST_1 "Help the guy"
#define QUEST_2 "Get some items"

// number of quests in the game 
#define NUM_QUESTS 2

#endif