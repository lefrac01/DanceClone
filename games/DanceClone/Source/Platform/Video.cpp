//      Video.cpp
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

#include "Video.h"

namespace Platform
{

Video::Video() :
  screen(NULL),
  rmode(NULL)
{
}

bool Video::Init()
{
  LOG(DEBUG_BASIC, "Platform::Video::Init()" << endl)
  //TODO: there is a way to reinit adding support for just video...
  //SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  //SDL_ShowCursor(SDL_DISABLE);

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
  LOG(DEBUG_BASIC, "VIDEO_GetPreferredMode(NULL) returns:"
      << "\nrmode->viTVMode:" << rmode->viTVMode \
      << "\nrmode->fbWidth:" << rmode->fbWidth \
      << "\nrmode->efbHeight:" << rmode->efbHeight \
      << "\nrmode->xfbHeight:" << rmode->xfbHeight \
      << "\nrmode->viXOrigin:" << rmode->viXOrigin \
      << "\nrmode->viYOrigin:" << rmode->viYOrigin \
      << "\nrmode->viWidth:" << rmode->viWidth \
      << "\nrmode->viHeight:" << rmode->viHeight \
      << endl)

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
  
//TODO: per-platform agent subclassing
//#endif

  return true;
}

int Video::ScreenHeight()
{
  return rmode->viHeight;
}

int Video::ScreenWidth()
{
  return rmode->viWidth;
}

void Video::Pump()
{
  LOG(DEBUG_GUTS, "Video::Pump" << endl)
  SDL_Flip(screen);
}

SDL_Surface* Video::LoadOptimize(string filename)
{
  LOG(DEBUG_MINOR, "Video::LoadOptimize(" << filename << ")" << endl)
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

SDL_Surface* Video::LoadOptimizeAlpha(string filename)
{
  LOG(DEBUG_MINOR, "Video::LoadOptimizeAlpha(" << filename << ")" << endl)
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

void Video::ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
  // can't log, too many tiny blits with little meaning
  //LOG(DEBUG_GUTS, "Video::ApplySurface(" << x << ", " << y << ")" << endl)
  SDL_Rect offset;offset.x = x; offset.y = y;
  SDL_BlitSurface(source, clip, destination, &offset);
}

void Video::Cleanup()
{
  LOG(DEBUG_MINOR, "Video::Cleanup" << endl)
  //screen(NULL),  SDL_Quit does this?
  //rmode(NULL),  ??
}

Uint32 Video::GetPixel(SDL_Surface *surface,int x,int y){
  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  switch(bpp) {
    case 1:
      return *p;
    case 2:
      return *(Uint16 *)p;
    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
      return *(Uint32 *)p;
    default:
      return 0;
  }
}

void Video::PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch(bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16 *)p = pixel;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void Video::PutPixel16(SDL_Surface *surface, int x, int y, Uint32 pixel){
  *((Uint16 *)surface->pixels+y*surface->pitch/2+x)=pixel;
}

}
