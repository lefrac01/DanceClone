//#define WIN
#define WII
//#define USEACCELEROMETER






#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <cstdlib>


#define GAMEVERSIONSTRING "DanceClone v0.54"
#define DEBUG_OFF 0
#define DEBUG_BASIC 1
#define DEBUG_MINOR 2
#define DEBUG_DETAIL 3
#define DEBUG_GUTS 4
#define DEBUG_LEVEL DEBUG_OFF
#define LOG_DEBUG
#define LOG_ERRORS
ofstream debug_log;
ofstream error_log;


bool done = false;
#include "../../../Generic/generic.h"
#include "../../../Generic/WiiDash/WiiDash_main.h"
#include "Game/Game_main.h"

int main(int argc, char* argv[])
{
  init();
  Game_init();
  WiiDash_init();
  
  while(!done)
  {
    SDL_PumpEvents();
    updatecontrolinput();
    
    if(WDonoffpercent == 0)
    {
      Game_run();
    }
    
    // unless playing song, run wiidash code
    if (gamestate!=8)
    {
      WiiDash_run();
    }
    
    SDL_Flip(screen);
    SDL_Event event;
    
    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
    {
      if(event.type == SDL_QUIT)
      {
        done = 1;
      }
    }
  }
  game_cleanup();
  generic_cleanup();
  return 0;
}
