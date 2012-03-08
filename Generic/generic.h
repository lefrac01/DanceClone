#ifdef WIN
  #include <windows.h>
  #include <SDL.h>
  #include <SDL_image.h>
  #include <SDL_mixer.h>
  #ifdef ONLINE
    #include <SDL_net.h>
  #endif
#endif
#ifdef WII
  #include <gccore.h>
  #include <wiiuse/wpad.h> 
  #include <SDL/SDL.h>
  #include <SDL/SDL_image.h>
  #include <SDL/SDL_mixer.h>
  #include <fat.h>
  #include <dirent.h>
  #include <unistd.h>
  #include <asndlib.h>
  #include <mp3player.h>
#endif

//#include <Box2D.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <math.h>


#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <cstdlib>






#define PI (3.141592653589793);

SDL_Surface *screen = NULL;
GXRModeObj *rmode = NULL;  
Uint8* pWDrgb1 = NULL;
Uint8* pWDrgb2 = NULL;

#include "timer.h"
#include "simplemath.h"
#include "charstuff.h"
#include "dirlist.h"
#include "loadanddraw.h"
#include "getputpixel.h"
#include "bresenham.h"
#include "segmentintersectrectangle.h"
#include "input.h"
#ifdef WII
#include "savepng.h"
#endif

#ifdef WIN
const int screenx=640;
const int screeny=480;
const int screenb=0;
int framecap=60;
#endif

void init()
{
  fatInitDefault();

  if (DEBUG_LEVEL > DEBUG_OFF)
  {
    log.open("debug", std::ios_base::trunc);
    log << GAMEVERSIONSTRING << " startup" << endl;
    //#log.close();
  }
  
  
  srand((int)time(NULL));

  SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);

  
#ifdef WIN
  screen = SDL_SetVideoMode(screenx, screeny, screenb, SDL_DOUBLEBUF);
  SDL_WM_SetCaption( GAMEVERSIONSTRING, NULL );
#endif

#ifdef WII

  rmode = VIDEO_GetPreferredMode(NULL);

  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    
    //#log.open("debug", std::ios_base::app);
    log << "VIDEO_GetPreferredMode(NULL) returns:"
      << "\nrmode->viTVMode:" << rmode->viTVMode \
      << "\nrmode->fbWidth:" << rmode->fbWidth \
      << "\nrmode->efbHeight:" << rmode->efbHeight \
      << "\nrmode->xfbHeight:" << rmode->xfbHeight \
      << "\nrmode->viXOrigin:" << rmode->viXOrigin \
      << "\nrmode->viYOrigin:" << rmode->viYOrigin \
      << "\nrmode->viWidth:" << rmode->viWidth \
      << "\nrmode->viHeight:" << rmode->viHeight \
      << endl;
    //#log.close();
  }

  switch (rmode->viTVMode >> 2)
  {
          case VI_NTSC: // 480 lines (NTSC 60hz)
                  break;
          case VI_PAL: // 576 lines (PAL 50hz)
                  rmode = &TVPal574IntDfScale;
                  rmode->xfbHeight = 480;
                  rmode->viYOrigin = (VI_MAX_HEIGHT_PAL - 480)/2;
                  rmode->viHeight = 480;
                  break;
          default: // 480 lines (PAL 60Hz)
                  break;
  }

  pWDrgb1 = (Uint8*)malloc(rmode->viWidth * rmode->viHeight * 3 * sizeof(Uint8));
  pWDrgb2 = (Uint8*)malloc(rmode->viWidth * rmode->viHeight * 3 * sizeof(Uint8));
 
  screen = SDL_SetVideoMode(rmode->viWidth, rmode->viHeight, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  
  WPAD_Init();
  WPAD_SetVRes(WPAD_CHAN_ALL,rmode->viWidth,rmode->viHeight);  
  WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);  
  
  SYS_SetResetCallback(WiiResetPressed);
  SYS_SetPowerCallback(WiiPowerPressed);
  WPAD_SetPowerButtonCallback(WiimotePowerPressed);
#endif
}

void generic_cleanup()
{
  free(pWDrgb1);
  free(pWDrgb2);
  SDL_Quit();
#ifdef WII
  // http://www.codemii.com/2009/03/02/tutorial-10-using-the-filesystem/
  fatUnmount(0); 
#endif
}
