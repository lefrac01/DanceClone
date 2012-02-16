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
  
	Timer fps;
	while(!done)
  {
		fps.start();
		SDL_PumpEvents();
		updatecontrolinput();
    
		if(WDonoffpercent == 0)
    {
      Game_run();
    }
		WiiDash_run();
    
		SDL_Flip(screen);
		SDL_Event event;
    
		while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0)
    {
			if(event.type == SDL_QUIT)
      {
        done = 1;
      }
    }
    
		if(WDonoffpercent==0)
			if(fps.get_ticks()<1000/framecap)
				SDL_Delay((1000/framecap)-fps.get_ticks());
		if(WDonoffpercent!=0)
			if(fps.get_ticks()<1000/60)
				SDL_Delay((1000/60)-fps.get_ticks());
	}
  cleanup();
	return 0;
}
