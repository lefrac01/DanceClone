//      PlatformGraphicsAgent.cpp
//      
//      Copyright 2012 Carl Lefrançois <carl.lefrancois@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include "PlatformGraphicsAgent.h"

//TODO: static tools class
#include "../../../../Generic/charstuff.h"

PlatformGraphicsAgent::PlatformGraphicsAgent() :
  screen(NULL),
  rmode(NULL),
  pWDrgb1(NULL),
  pWDrgb2(NULL)
{
}

bool PlatformGraphicsAgent::Init()
{
  SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);

/*
//TODO: per-platform agent subclassing
#ifdef WIN
  screen = SDL_SetVideoMode(screenx, screeny, screenb, SDL_DOUBLEBUF);
  SDL_WM_SetCaption( GAMEVERSIONSTRING, NULL );
#endif
*/

//TODO: per-platform agent subclassing
//#ifdef WII

  rmode = VIDEO_GetPreferredMode(NULL);
/*
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
*/
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

  screen = SDL_SetVideoMode(rmode->viWidth, rmode->viHeight, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  
  //TODO: elements used by GUIDash...
  pWDrgb1 = (Uint8*)malloc(rmode->viWidth * rmode->viHeight * 3 * sizeof(Uint8));
  pWDrgb2 = (Uint8*)malloc(rmode->viWidth * rmode->viHeight * 3 * sizeof(Uint8));
  
  WPAD_SetVRes(WPAD_CHAN_ALL,rmode->viWidth,rmode->viHeight);  
  WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);  
  
//TODO: per-platform agent subclassing
//#endif
  
  
WDbackground      = SDL_DisplayFormat(screen);
WDbackgroundblurred   = SDL_DisplayFormat(screen);
WDfontimage       = LoadOptimizeAlpha("Media/WiiDash/Font.png");
#ifdef WII
  while(!WDfontimage){
  WDfontimage     = LoadOptimizeAlpha("Media/WiiDash/Font.png");
  fatInitDefault();}
#endif
WDbuttonimage       = LoadOptimizeAlpha("Media/WiiDash/Button.png");
WDarrowsimage       = LoadOptimize("Media/WiiDash/Arrows.png");
WDborderimage       = LoadOptimize("Media/WiiDash/Border.png");
WDtopbottomborderimage  = LoadOptimize("Media/WiiDash/TopBottomBorder.png");
WDcursorimage       = LoadOptimizeAlpha("Media/WiiDash/Cursor.png");
WDfontimagecolored    = LoadOptimizeAlpha("Media/WiiDash/Font.png");

for(int a=0; a<95; a++){
WDfontframes[a].x=a*15;
WDfontframes[a].y=0;
WDfontframes[a].w=15;
WDfontframes[a].h=21;}
for(int b=0; b<2; b++)for(int a=0; a<9; a++){
WDbuttonframes[a+b*9].x=a*21;
WDbuttonframes[a+b*9].y=b*21;
WDbuttonframes[a+b*9].w=21;
WDbuttonframes[a+b*9].h=21;}
for(int a=0; a<4; a++){
WDarrowsframes[a].x=a*16;
WDarrowsframes[a].y=0;
WDarrowsframes[a].w=16;
WDarrowsframes[a].h=16;}
for(int a=0; a<16; a++){
WDcursorframes[a].x=a*96;
WDcursorframes[a].y=0;
WDcursorframes[a].w=96;
WDcursorframes[a].h=96;}
  
  return true;
}

int PlatformGraphicsAgent::ScreenHeight()
{
  return rmode->viHeight;
}

int PlatformGraphicsAgent::ScreenWidth()
{
  return rmode->viWidth;
}

void PlatformGraphicsAgent::Pump()
{
  SDL_Flip(screen);
}

SDL_Surface* PlatformGraphicsAgent::LoadOptimize(string filename)
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;
  loadedImage = IMG_Load(filename.c_str()); 
  if (loadedImage != NULL)
  {
    optimizedImage = SDL_DisplayFormat(loadedImage);
    Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF);
    SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);
    SDL_FreeSurface(loadedImage);
  }
  return optimizedImage;
}

SDL_Surface* PlatformGraphicsAgent::LoadOptimizeAlpha(string filename)
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;
  loadedImage = IMG_Load(filename.c_str()); 
  if (loadedImage != NULL)
  {
    optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
    SDL_FreeSurface(loadedImage);
  }
  return optimizedImage;
}

void PlatformGraphicsAgent::ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
  SDL_Rect offset;offset.x = x; offset.y = y;
  SDL_BlitSurface(source, clip, destination, &offset);
}

void PlatformGraphicsAgent::Cleanup()
{
  
  //TODO: elements used by GUIDash...
  if (pWDrgb1)
  {
    free(pWDrgb1);
    pWDrgb1 = NULL;
  }
  if (pWDrgb2)
  {
    free(pWDrgb2);
    pWDrgb2 = NULL;
  }
  
  //screen(NULL),  SDL_Quit does this?
  //rmode(NULL),  ??
}

void PlatformGraphicsAgent::DrawSpriteText(int posx,int posy,char* texttosprite,int leftmiddleright)
{
  int textlength=strlen(texttosprite);
  int textnumber;
  int letterprintx=0;
  int letterprinty=posy;
  if(leftmiddleright==1)letterprintx=posx;
  if(leftmiddleright==2)letterprintx=posx-textlength*11/2;
  if(leftmiddleright==3)letterprintx=posx-textlength*11;
  for(int b=0;b<textlength;b++){
    textnumber=getnumberfromchar(texttosprite[b]);
    ApplySurface(letterprintx,letterprinty,WDfontimage,screen,&WDfontframes[textnumber]);
    letterprintx+=11;
  }
}
