// character stats/moves 

#include <string.h>

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

// generate a character based on the type given 
struct character generateCharacter(int characterType)
{
	struct character returnedChar = {};
	
	switch(characterType)
	{
		case HUMAN:
		strcpy(returnedChar.name,"Human");
		returnedChar.health = 5;
		returnedChar.maxHealth = 5;
		returnedChar.stamina = 20;
		returnedChar.maxStamina = 20;
		break;
		case STRANJER:
		strcpy(returnedChar.name,"Stranjer");
		break;
		case LOST_SPIRIT:
		strcpy(returnedChar.name,"Lost Spirit");
		break;
		case GUARD_DUCK:
		strcpy(returnedChar.name,"Guard Duck");
		break;
		case WILLFUL_WISP:
		strcpy(returnedChar.name,"Willful Wisp");
		break;
		case CHILLER:
		strcpy(returnedChar.name,"Chiller");
		break;
		case BRASS:
		strcpy(returnedChar.name,"Brass");
		break;
		case STARIP:
		strcpy(returnedChar.name,"Starip");
		break;
		case GUMMO:
		strcpy(returnedChar.name,"Gummo");
		break;
		case LOST_HERO:
		strcpy(returnedChar.name,"Lost Hero");
		break;
		case MICRONOS:
		strcpy(returnedChar.name,"Micronos");
		break;
		case DIASNAK:
		strcpy(returnedChar.name,"Diasnak");
		break;
		case WANDERER:
		strcpy(returnedChar.name,"Wanderer");
		break;
		case DIAMAN:
		strcpy(returnedChar.name,"Diaman");
		break;
		case REVENGE:
		strcpy(returnedChar.name,"Vengence");
		break;
	}
	
	return returnedChar;
}

#endif