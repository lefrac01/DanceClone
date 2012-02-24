//TODO: add second player
//


#ifdef WIN
Mix_Music* music = NULL;
#endif

#include "setup/sprites.h"
extern float pixels_per_ms_at_1_bpm;

void Game_init()
{
  pixels_per_ms_at_1_bpm = rmode->viHeight / 4000.0 / 120.0;  
  
#ifdef WIN
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
#endif

  Game_setup_sprites();
  
  goal_offset = rmode->viHeight / 4; // Immitate DDR2 on Wii
}
