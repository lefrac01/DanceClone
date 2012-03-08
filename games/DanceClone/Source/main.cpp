
//#define WIN
#define WII
//#define USEACCELEROMETER

/*
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <cstdlib>
*/

#define GAMEVERSIONSTRING "DanceClone v0.57"
/*
#define DEBUG_OFF 0
#define DEBUG_BASIC 1
#define DEBUG_MINOR 2
#define DEBUG_DETAIL 3
#define DEBUG_GUTS 4
#define DEBUG_LEVEL DEBUG_OFF
#define LOG_DEBUG
ofstream log;
*/

#include "Game/Game.h"
using DanceClone::Game;

#ifdef WIN
#include "Platform/Win32Platform.h"
#endif

#ifdef WII
#include "Platform/WiiPlatform.h"
#endif

int main(int argc, char* argv[])
{
/*
  if (DEBUG_LEVEL > DEBUG_OFF)
  {
    log.open("debug", std::ios_base::trunc);
    log << GAMEVERSIONSTRING << " startup" << endl;
    log.close();
  }
*/  
  #ifdef WIN
  Win32Platform platform;
  #endif
  #ifdef WII
  WiiPlatform platform;
  #endif

  platform.Init();
  
  Game game(platform);
  game.Init();
//  Game_init();
//  WiiDash_init();

  bool done = false;
  while(!done)
  {
    platform.Pump();
    
    //updatecontrolinput();
    
//    if(WDonoffpercent == 0)
//    {
//      Game_run();
//    }
    if (!platform.DashVisible())
    {
      game.Pump();
    }
    
    // unless playing song, run wiidash code
//    if (gamestate!=8 && gamestate != 7)
//TODO: enum
    if (game.State() != 8 && game.State() != 7)
    {
      //WiiDash_run();
      platform.PumpDash();
    }
    
//    SDL_Flip(screen);
    SDL_Event event;
    
    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
    {
      if(event.type == SDL_QUIT)
      {
        done = 1;
      }
    }
  }
  //game_cleanup();
  game.Cleanup();
  platform.Cleanup();
  //generic_cleanup();
  return 0;
}
