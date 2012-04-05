//      LinuxVideo.cpp
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

#include "LinuxVideo.h"

namespace Platform
{

LinuxVideo::LinuxVideo()
{
  screenWidth = 640;
  screenHeight = 480;
}

bool LinuxVideo::Init()
{
  LOG(DEBUG_MINOR, "Platform::Video::Init()" << endl)
  //TODO: there is a way to reinit adding support for just video...
  //SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  //SDL_ShowCursor(SDL_DISABLE);


  screen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_DOUBLEBUF);
  //SDL_WM_SetCaption( GAMEVERSIONSTRING, NULL );


  return true;
}

void LinuxVideo::Pump()
{
  LOG(DEBUG_GUTS, "LinuxVideo::Pump" << endl)
  Video::Pump();
}

void LinuxVideo::Cleanup()
{
  LOG(DEBUG_MINOR, "LinuxVideo::Cleanup" << endl)
  Video::Cleanup();
}


}
