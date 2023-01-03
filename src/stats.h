// character stats/moves 

#ifndef CHARACTER_HANDLED
#define CHARACTER_HANDLED

// types of moves in the game 
enum move_types
{
	SLASH,
	BLUDGEON,
	PIERCE,
	DARKNESS,
	SEAL,
	HOLY
};

// weapon types in the game 
enum wpn_types
{
	SWORD,
	LANCE,
	AXE,
	RANGED,
	COMBINATION
};

// struct for weapons 
struct weapon
{
	char name[20];
	int vitality;
	int level;
	int chambers; 
};

// type of character in the game 
enum char_type
{
	HUMAN,
	DEMONOID,
	// main demons 
	STRANJER,
	LOST_SPIRIT,
	GUARD_DUCK,
	WILLFUL_WISP,
	CHILLER,
	BRASS,
	STARIP,
	GUMMO,
	LOST_HERO,
	MICRONOS,
	DIASNAK,
	WANDERER,
	DIAMAN,
	REVENGE
};

// struct for characters 
struct character
{
	char name[20];
	int health;
	int maxHealth;
	
	int strength;
	int resistance;
	int agility;
	int dexterity;
	int spirit;
	
	int stamina;
	int maxStamina;
};
#endif