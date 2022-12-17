#define SDL_MAIN_HANDLED

// main SDL2 handler
#include <SDL2/SDL.h>

// handles music with SDL2
#include <SDL2/SDL_mixer.h>

// main track that is playing
Mix_Music * music;

void initMusic()
{
	if(SDL_Init(SDL_INIT_AUDIO) >= 0)
	{
		//Initialize music handling 
		if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
		{
			printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		}
	}
	
	music = Mix_LoadMUS( "Live.wav");
	Mix_PlayMusic(music, 1);
}