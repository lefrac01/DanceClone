//TODO: scrap wiidish and SDL_GetTicks() in favor of gettime()
//
//DOH on second thought, inspecting SDL_Systimer.c reveals the
// exact code block below is implemented in SDL_GetTicks().
//eg:
/*
Uint64 get_tick_count()
{
  const Uint64 ticks = gettime();
  const Uint64 ms = ticks / TB_TIMER_CLOCK;
  return ms;
}
*/
//
//TODO: ask tueidj advice on gettime()-based frame updates vs VIinterrupt...
//
//TODO: prevent wiidash from being callable during play.  dash appears,
// music keeps playing but arrows stop animating so sync is lost.
//

//#define WIN
#define WII
//#define USEACCELEROMETER
bool done = false;
#include "../../../Generic/generic.h"
#include "../../../Generic/WiiDash/WiiDash_main.h"
#include "Game/Game_main.h"

int main(int argc, char* argv[])
{
  init();
  Game_init();
  WiiDash_init();
  
//  Timer fps;
  while(!done)
  {
//    fps.start();
    SDL_PumpEvents();
    updatecontrolinput();
    
    if(WDonoffpercent == 0)
    {
      Game_run();
    }
    WiiDash_run();  // hmm.  would rather rip out functionality and run wiidash when home button callback is pressed
    
    SDL_Flip(screen);
    SDL_Event event;
    
    while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
    {
      if(event.type == SDL_QUIT)
      {
        done = 1;
      }
    }
    
    
    // game runs fine without the following...
    
    // LOOK FOR HITCH HERE
/*    if(WDonoffpercent==0)
      if(fps.get_ticks()<1000/framecap)
        SDL_Delay((1000/framecap)-fps.get_ticks());
    if(WDonoffpercent!=0)
      if(fps.get_ticks()<1000/60)
        SDL_Delay((1000/60)-fps.get_ticks());
        */
  }
  game_cleanup();
  generic_cleanup();
  return 0;
}
