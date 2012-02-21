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



#define GAMEVERSIONSTRING "DanceClone v0.53"
#define DEBUG_OFF 0
#define DEBUG_BASIC 1
#define DEBUG_MINOR 2
#define DEBUG_DETAIL 3
#define DEBUG_GUTS 4
#define DEBUG_LEVEL DEBUG_DETAIL
#define LOG_DEBUG
//#define LOG_IMPORT
//#define LOG_ERRORS
ofstream debug_log;
ofstream error_log;





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
  fatInitDefault(); //fatMountSimple("sd", &__io_wiisd); is not required - this is done by fatInitDefault()

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
  
  
  srand((int)time(NULL));
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_ShowCursor(SDL_DISABLE);

  
#ifdef WIN
  screen = SDL_SetVideoMode(screenx, screeny, screenb, SDL_DOUBLEBUF);
  SDL_WM_SetCaption( GAMEVERSIONSTRING, NULL );
#endif

#ifdef WII
  // CONFLICT WITH SDL?  SDL also gives X Y ?
  //VIDEO_Init();
  // not calling VIDEO_Init() gives 640x480. 
  // calling it gives 320x200 with flicker (no double buf)


  //NOTE: SDL internally calls VIDEO_Init(); and the following
  // VIDEO_GetPreferredMode().  the height and width must
  // be available in an SDL variable or from an SDL function!

  
  rmode = VIDEO_GetPreferredMode(NULL);
  /*
  viTVMode  mode and type of TV
  fbWidth width of external framebuffer
  efbHeight height of embedded framebuffer
  xfbHeight height of external framebuffer
  viXOrigin x starting point of first pixel to draw on VI
  viYOrigin y starting point of first pixel to draw on VI
  viWidth width of configured VI
  viHeight  height of configured VI
  */
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    
    debug_log.open("debug", std::ios_base::app);
    debug_log << "VIDEO_GetPreferredMode(NULL) returns:"
      << "\nrmode->viTVMode:" << rmode->viTVMode \
      << "\nrmode->fbWidth:" << rmode->fbWidth \
      << "\nrmode->efbHeight:" << rmode->efbHeight \
      << "\nrmode->xfbHeight:" << rmode->xfbHeight \
      << "\nrmode->viXOrigin:" << rmode->viXOrigin \
      << "\nrmode->viYOrigin:" << rmode->viYOrigin \
      << "\nrmode->viWidth:" << rmode->viWidth \
      << "\nrmode->viHeight:" << rmode->viHeight \
      << endl;
    debug_log.close();
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

/*
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "begin allocate wiidash rgb stashes of size at timestamp " << SDL_GetTicks() << "... " << endl;
    debug_log.close();
  }
*/
  pWDrgb1 = (Uint8*)malloc(rmode->viWidth * rmode->viHeight * 3 * sizeof(Uint8));
  pWDrgb2 = (Uint8*)malloc(rmode->viWidth * rmode->viHeight * 3 * sizeof(Uint8));
  /*
  pWDrgb1 = (Uint8***)malloc(rmode->viWidth * sizeof(Uint8**));
  pWDrgb2 = (Uint8***)malloc(rmode->viWidth * sizeof(Uint8**));
  for (int i = 0; i < rmode->viWidth; i++)
  {
    pWDrgb1[i] = (Uint8**)malloc(rmode->viHeight * sizeof(Uint8*));
    pWDrgb2[i] = (Uint8**)malloc(rmode->viHeight * sizeof(Uint8*));
    for (int j = 0; j < rmode->viHeight; j++)
    {
      pWDrgb1[i][j] = (Uint8*)malloc(3 * sizeof(Uint8));
      pWDrgb2[i][j] = (Uint8*)malloc(3 * sizeof(Uint8));
    }
  }
   if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done allocaat timestamp " << SDL_GetTicks() << "... " << endl;
    debug_log.close();
    
  }
  */
 
  screen = SDL_SetVideoMode(rmode->viWidth, rmode->viHeight, 16, SDL_DOUBLEBUF);
  
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
  /*
  for (int i = 0; i < rmode->viHeight; i++)
  {
    for (int j = 0; j < rmode->viHeight; j++)
    {
      free(pWDrgb1[i][j]);
      free(pWDrgb2[i][j]);
    }
  }
  for (int i = 0; i < rmode->viHeight; i++)
  {
    free(pWDrgb1[i]);
    free(pWDrgb2[i]);
  }
  */
  free(pWDrgb1);
  free(pWDrgb2);
#ifdef WII

  // http://www.codemii.com/2009/03/02/tutorial-10-using-the-filesystem/
  fatUnmount(0); 
#endif
}
