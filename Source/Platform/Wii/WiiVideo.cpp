//      WiiVideo.cpp
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

#include "WiiVideo.h"

namespace Platform
{

WiiVideo::WiiVideo() :
  rmode(NULL)
{
}

bool WiiVideo::Init()
{
  LOG(DEBUG_MINOR, "Platform::Video::Init()" << endl)
  //TODO: there is a way to reinit adding support for just video...
  //SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

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

  screenWidth = rmode->viWidth;
  screenHeight = rmode->viHeight;
  screen = SDL_SetVideoMode(screenWidth, screenHeight, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  
  return (screen != NULL);
}

void WiiVideo::Pump()
{
  LOG(DEBUG_GUTS, "WiiVideo::Pump" << endl)
  Video::Pump();
}

void WiiVideo::Cleanup()
{
  LOG(DEBUG_MINOR, "WiiVideo::Cleanup" << endl)
  Video::Cleanup();
}


}
