#include "Platform/OS.h"

#include "Game/Game.h"
using DanceClone::Game;
#include "Game/Constants.h"
using DanceClone::Constants;

#ifdef WIN
#include "Platform/Win32/Win32OS.h"
using Platform::Win32OS;
#endif

#ifdef WII
#include "Platform/Wii/WiiOS.h"
using Platform::WiiOS;
#endif

#ifdef LINUX
#include "Platform/Linux/LinuxOS.h"
using Platform::LinuxOS;
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
  #ifdef LINUX
  LinuxOS sys;
  #endif

  //TODO: all failure cases
  sys.Init();

  INIT_LOG
  LOG(DEBUG_BASIC, GAMEVERSIONSTRING << " startup" << endl)
  FLUSH_LOG


  Constants constants;
  constants.Init(); // may eventually use a config file 

  GUI gui(sys);
  if (!gui.Init())
  {
    LOG(DEBUG_BASIC, "ERROR: gui.Init() failed" << endl)
    gui.Cleanup();
    sys.Cleanup();
    exit(-1);
  } 
  
  Dash dash(sys, gui);
  if (!dash.Init())
  {
    LOG(DEBUG_BASIC, "ERROR: dash.Init() failed" << endl)
    gui.Cleanup();
    sys.Cleanup();
    exit(-2);
  }
  
  Game game(sys, gui);
  if (!game.Init())
  {
    LOG(DEBUG_BASIC, "ERROR: game.Init() failed" << endl)
    gui.Cleanup();
    dash.Cleanup();
    sys.Cleanup();
    exit(-3);
  }

  bool done = false;
  while(!done)
  {
    sys.Pump();
    
    if (dash.UserWantsOut())
    {
      done = true;
    }
    
    if (game.CanInterrupt())
    {
      gui.Update();
      dash.Run();
    }

    if (!dash.Visible())
    {
      game.Run();
    }

      
    SDL_Event event;
    
    LOG(DEBUG_GUTS, "Entering main loop" << endl)
    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
    {
      LOG(DEBUG_GUTS, "Peeped SDL event in main loop" << endl)
      if(event.type == SDL_QUIT)
      {
        LOG(DEBUG_GUTS, "got SDL_QUIT" << endl)
        done = true;
      }
    }
  }
  
  game.Cleanup();
  gui.Cleanup();
  dash.Cleanup();
  sys.Cleanup();
  CLOSE_LOG
  
  return 0;
}
