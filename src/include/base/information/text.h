// holds text information used in the game
#ifndef TEXT_HANDLED
#define TEXT_HANDLED
// title screen text 
	#define GAME_WINDOW_NAME "Stone Crawler 2.0"

	#define TITLE_DESC1 "Version 2.0" 
	#define TITLE_DESC2 "Finality Games - Developed by Demi"

// dungeon status text 
	#define HEAR_FIGHT "You hear a fight breaking out."
	#define ENCOUNTERED "You encountered a curse!"
	#define EMPTY_CHEST "You stand near an opened empty steel chest."
	#define CLOSED_CHEST "You stand near a closed steel chest, press enter to open it."
	#define SHOP_ENTRANCE "You stand near a hole in the wall, press enter to go through it."
	#define UPSTAIRS_TILE "Press enter to go upstairs."
	#define DOWNSTAIRS_TILE "Press enter to go downstairs."
	#define NPC_FIGHT "You see someone fighting a curse! Press enter to help them!"
	#define ENEMY_MOVEMENT "A footstep echoes from the darkness..."
	#define PASS_NPC "They stepped aside for you to pass by."
	#define NPC_DONE_TALK "They stopped talking to you."
	
	#define UNLOCKED_DOOR "You see a locked door, press enter to unlock it"
	#define LOCKED_DOOR "You see a locked door, you need a key to open it..."
	#define UNLOCK_DOOR "You unlocked the door and the key vanished?"
	
	#define NPC_RUN_AWAY "You walked away suddenly."
	#define OPENED_CHEST "You opened the chest and found... nothing wow awesome."
	#define TALK_TO_NPC "You began talking with the person."
	#define WALK_UP "You walk upstairs."
	#define WALK_DOWN "You walk downstairs."
	#define NPCvsENEMY "You hear fighting."
	#define NPCLOST "You hear someone crying out in pain."
	#define NPCWON "The fighting seemed to stop."
	#define PASSOUT "You feel tired and can't move..."
	#define WAKINGUP "You wake up."
	#define CRUSHED "Something got crushed by a wall"
	
// signs in the game 
	#define SECTOR_0_SIGN "You see a sign that says \"BEWARE\""

// specific dungeon narrative/description text 
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
	#define W_COMMAND "Item" 
	#define E_COMMAND "Run"

	//title screen options 
	#define NG "New Game"
	#define ST "Settings"
	#define CT "Continue"
	#define HTP "How to Play"
	
	// settings options
	#define SETTING_TEXT_R "Resolution"
	#define SETTING_TEXT_F "Full Screen"
	#define SETTING_TEXT_FPS "FPS"
	#define SETTING_TEXT_C "Controls"
	#define SETTING_TEXT_D "Revert to Default"
	
	#define DEF_Y "Revert settings to default"
	#define DEF_N "Keep settings"
	
	// character menu stuff 
	#define C_SAVE "Save Game"
	#define C_SET "Settings"
	#define C_EXIT "Back"
	
#endif
