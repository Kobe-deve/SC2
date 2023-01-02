// character stats/moves 

#ifndef CHARACTER_HANDLED
#define CHARACTER_HANDLED
struct character
{
	char name[20];
	int health;
	int maxHealth;
	
	int stamina;
	int maxStamina;
};
#endif