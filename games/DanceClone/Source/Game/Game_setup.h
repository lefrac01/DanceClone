//TODO: add second player
//


#ifdef WIN
Mix_Music* music = NULL;
#endif

#include "setup/sprites.h"

void Game_init()
{

#ifdef WIN
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
#endif

  Game_setup_sprites();
  
  goaloffset = rmode->viHeight / 4; // Immitate DDR2 on Wii
}
