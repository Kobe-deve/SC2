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
	int vitality; // health of the weapon 
	int level; // level of the weapon 
	int type; // the type of weapon 
	int chambers; // max ammo of weapon if it takes any 
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

enum personality_type
{
	OPTIMISTIC,
	DEPRESSED,
	PESSIMIST,
	GOOFY,
	ANGRY,
	NEUTRAL,
	CURIOUS,
	ANNOYED
};

// struct for characters 
struct character
{
	char name[20]; 
	int health; // health stats 
	int maxHealth;
	
	// general stats 
	int strength;
	int damageCap;
	int maxStr;

	int resistance;
	int painCap;
	int maxRes;
	
	int agility;
	int exhaustCounter;
	int maxAgi;
	
	int dexterity;
	int efficiencyCounter;
	int maxDex;
	
	int spirit;
	int maxSpr;
	int attunementPoints;
	
	int stamina; // stamina stats 
	int maxStamina;
	
	int dnaSize; // dna of the specific character, uses char_type enum 
	int * dna;

	int weakness; // move type the character is weak to
	
	struct weapon heldWeapon; // the weapon the character is holding
	
	int personality; // personality type of the character 
};
#endif