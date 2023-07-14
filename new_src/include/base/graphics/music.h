#ifndef MUSIC_HANDLED
#define MUSIC_HANDLED

// switch music to specific track
void switchTrack(char * songName, struct gameState * state)
{
	if(state->track != songName)
	{
		state->track = songName;
		Mix_FadeOutMusic(0); // stop current music 
		state->music = Mix_LoadMUS(songName);
		Mix_FadeInMusic(state->music, -1, 2000);
	}
}

// initialize music handling
void initMusic(struct gameState * state)
{
	//Initialize music handling 
	if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024*2 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		return;
	}
	
	// set sound effect volume to 1/3 max 
	Mix_Volume(0,MIX_MAX_VOLUME/2);
	
	// start title music 
	switchTrack(TITLE_MUSIC,state);
}


#endif