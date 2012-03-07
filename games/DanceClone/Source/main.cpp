//TODO: tighten up file organisation.  header inclusion tree could be nicer
// and on-disk file layout is strange.  also main program flow is hard to 
// follow.  redo bad design ie play_data as a class is responsible for reading 
// player controls and updating the viewport every frame...
//
//TODO: instead of always opening/closing log as in: (or commenting that out as currently)
//log.open("debug", std::ios_base::app);
//log << "something" << endl;
//log.close();
//replace with a macro in which the open/close are controlled by a 
//FAILSAFE_LOG define

//#define WIN
#define WII
//#define USEACCELEROMETER




// quick hack delay song start for player prep
char* mp3_buffer;
long mp3_lSize;

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <cstdlib>


#define GAMEVERSIONSTRING "DanceClone v0.57"
#define DEBUG_OFF 0
#define DEBUG_BASIC 1
#define DEBUG_MINOR 2
#define DEBUG_DETAIL 3
#define DEBUG_GUTS 4
#define DEBUG_LEVEL DEBUG_OFF
#define LOG_DEBUG
#define LOG_ERRORS
ofstream log;
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
    if (gamestate!=8 && gamestate != 7)
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
