#ifndef STATE_HANDLED
#include "state.h"
#endif

#ifndef MUSIC_HANDLED
#define MUSIC_HANDLED
void initMusic(struct gameState * input)
{
	if(SDL_Init(SDL_INIT_AUDIO) >= 0)
	{
		//Initialize music handling 
		if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		}
	}
	
	input->music = Mix_LoadMUS("Live.wav");
	Mix_PlayMusic(input->music, 1);
}
#endif