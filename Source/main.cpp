
//#define WIN
#define WII
//#define USEACCELEROMETER

#define GAMEVERSIONSTRING "DanceClone v0.58"


#include "Game/Game.h"
using DanceClone::Game;

#ifdef WIN
#include "Platform/Win32OS.h"
using Platform::Win32OS;
#endif

#ifdef WII
#include "Platform/WiiOS.h"
using Platform::WiiOS;
#endif

#include "GUI/GUI.h"
using Gooey::GUI;

#include "Dash/Dash.h"

int main(int argc, char* argv[])
{
  #ifdef WIN
  Win32OS sys;
  #endif
  #ifdef WII
  WiiOS sys;
  #endif

  //TODO: failure cases
  sys.Init();

  INIT_LOG
  LOG(DEBUG_BASIC, GAMEVERSIONSTRING << " startup" << endl)
  FLUSH_LOG

  GUI gui(sys);
  gui.Init();
  
  Dash dash(sys, gui);
  dash.Init();
  
  Game game(sys, gui);
  game.Init();

  bool done = false;
  while(!done)
  {
    sys.Pump();
    
    dash.Run();
    if (dash.UserWantsOut())
    {
      done = true;
    }
    
    if (!dash.Visible())
    {
      game.Run();
    }
    
    if (game.State() != Game::PLAY_PREP1 && game.State() != Game::PLAY_PREP2)
    {
      gui.Update();
    }
    
    SDL_Event event;
    
    LOG(DEBUG_GUTS, "Entering main loop" << endl)
    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
    {
      LOG(DEBUG_GUTS, "Peeped SDL event in main loop" << endl)
      if(event.type == SDL_QUIT)
      {
        LOG(DEBUG_GUTS, "got SDL_QUIT" << endl)
        done = 1;
      }
    }
  }
  
  game.Cleanup();
  gui.Cleanup();
  sys.Cleanup();
  CLOSE_LOG
  
  return 0;
}
