//TODO: add second player

int longestcombo;
int combo;
int boo;
int good;
int perfect;

int songtime = 0;
int songstarttime = 0;

int upcontrol = 0;
int downcontrol = 0;
int leftcontrol = 0;
int rightcontrol = 0;

int goalline = 50;

char songfilename[MAXPATHLEN];

#ifdef WIN
Mix_Music* music = NULL;
#endif

#include "setup/classes.h"
#include "setup/sprites.h"

void Game_init()
{
  if (DEBUG_LEVEL > DEBUG_OFF)
  {
    debug_log.open("debug", std::ios_base::trunc);
    debug_log << GAMEVERSIONSTRING << " startup" << endl;
    debug_log.close();
  }
  #ifdef LOG_ERRORS
  error_log.open("errors", std::ios_base::trunc);
  error_log.close();
  #endif

#ifdef WIN
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
#endif

  Game_setup_sprites();
}
