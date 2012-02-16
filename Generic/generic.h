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

const int screenx=640;
const int screeny=480;
const int screenb=0;
int framecap=60;

void init()
{
  srand((int)time(NULL));
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_ShowCursor(SDL_DISABLE);
#ifdef WIN
  screen = SDL_SetVideoMode(screenx, screeny, screenb, SDL_DOUBLEBUF);
  //screen = SDL_SetVideoMode(screenx,screeny,16,SDL_FULLSCREEN);
  SDL_WM_SetCaption( "DanceClone 0.52", NULL );
#endif
#ifdef WII
  screen = SDL_SetVideoMode(screenx, screeny, 16, SDL_DOUBLEBUF);
  WPAD_Init();
  WPAD_SetVRes(WPAD_CHAN_ALL, 640, 480);
  WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);  
  SYS_SetResetCallback(WiiResetPressed);
  SYS_SetPowerCallback(WiiPowerPressed);
  WPAD_SetPowerButtonCallback(WiimotePowerPressed);
  fatInitDefault(); //fatMountSimple("sd", &__io_wiisd); is not required - this is done by fatInitDefault()
#endif
}

void cleanup()
{
#ifdef WII
  // http://www.codemii.com/2009/03/02/tutorial-10-using-the-filesystem/
  fatUnmount(0); 
#endif
}
