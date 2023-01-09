#ifndef STATE_HANDLED
#include "state.h"
#endif

#ifndef MUSIC_HANDLED
#define MUSIC_HANDLED

// switch music to specific track
void switchTrack(char * songName, struct gameState * s)
{
	Mix_FadeOutMusic(0); // stop current music 
	s->music = Mix_LoadMUS(songName);
	Mix_FadeInMusic(s->music, -1, 2000);
}

// initialize music handling
void initMusic(struct gameState * input)
{
	if(SDL_Init(SDL_INIT_AUDIO) >= 0)
	{
		//Initialize music handling 
		if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
			return;
		}
	}
	
	// set sound effect volume to 1/3 max 
	Mix_Volume(0,MIX_MAX_VOLUME/2);
	
	// start title music 
	switchTrack(TITLE_MUSIC,input);
	
	// set initial game state variables 
	input->input = 0;
	input->option = 0;
	input->options = NULL;
	
	input->currentBattle.turns = 0;
	input->currentBattle.numEnemies = 0;
	input->currentBattle.enemies = NULL;
	input->currentBattle.turnOrder = NULL;
	input->options = NULL;
}
#endif