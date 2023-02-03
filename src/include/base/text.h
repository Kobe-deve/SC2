// file for all dialogue/text 

#ifndef TEXT_DEFINED
#define TEXT_DEFINED

// title screen text 
	#define TITLE_DESC1 "Version 2.0: Idle on the Grindstone" 
	#define TITLE_DESC2 "Finality Games - Developed by Demi"

// dungeon status text 
	#define HEAR_FIGHT "You hear a fight breaking out."
	#define ENCOUNTERED "You encountered a curse!"
	#define EMPTY_CHEST "You stand near an opened empty steel chest."
	#define CLOSED_CHEST "You stand near a closed steel chest, press enter to open it."
	#define SHOP_ENTRANCE "You stand near a hole in the wall, press enter to go through it."
	#define NPC_FIGHT "You see someone fighting a curse! Press enter to help them!"
	#define ENEMY_MOVEMENT "A footstep echoes from the darkness..."
	#define PASS_NPC "They stepped aside for you to pass by."
	#define NPC_DONE_TALK "They stopped talking to you."
	#define NPC_RUN_AWAY "You walked away suddenly."
	#define OPENED_CHEST "You opened the chest and found... nothing wow awesome."
	#define TALK_TO_NPC "You began talking with the person."
	#define WALK_UP "You walk upstairs."
	#define WALK_DOWN "You walk downstairs."
	#define NPCvsENEMY "You hear fighting."
	#define NPCLOST "You hear someone crying out in pain."
	#define PASSOUT "You feel tired and can't move..."
	#define WAKIGNUP "You wake up."

#define FIRST_FLOOR_TEXT "You wake up in a strange stone room, dimly lit but still dark."

// npc names/description
	#define NPC1_N "Stranger"
	#define NPC2_N "Mysterious Figure"
	#define NPC1 "You see a person nearby, press enter to talk to them."
	#define NPC2 "A cloaked figure stands before you, press enter to talk to them."

// npc dialogue 
	#define NPCRESP "I am responding"
	#define NPCPASS_Yes "You can pass now"
	#define NPCGREET_RESPONSE "Responding to greet"
	#define NPCBAT "A battle is starting"
	#define NPCJOINED "NPC Joined your party"
	#define PRESS_ENTER "Press Enter"
	
// menu options 

	// initial talking with npc in dungeon 
	#define G_COMMAND "Greet" 
	#define Q_COMMAND "Ask a question"
	#define P_COMMAND "Ask to pass by"
	#define L_COMMAND "Leave"

	// combat options 
	#define A_COMMAND "Attack" 
	#define M_COMMAND "Magic"
	#define T_COMMAND "Talk"
	#define W_COMMAND "Lol" 
	#define E_COMMAND "Exit"

	//title screen options 
	#define NG "New Game"
	#define CT "Continue"

// npc questions 
	#define QUESTION1 "This is Q1"
	#define QUESTION2 "This is Q2"
	#define QUESTION3 "This is Q3"
	#define QUESTION4 "This is Q4"
	
	// generate responses based on question type, used for talking to npcs/demons
	char ** generateResponses(int questionType)
	{
		char ** array;
		switch(questionType)
		{
			default:
			case 0:
			array = malloc(2 * sizeof(char*));
			array[0] = "Yes";
			array[1] = "No";
			return array;
			break;
			case 1:
			array = malloc(3 * sizeof(char*));
			array[0] = "That's cool";
			array[1] = "I didn't know that";
			array[2] = "Wow";
			return array;
			break;
		}
	}

#endif

