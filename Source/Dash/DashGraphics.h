//      Graphics.h
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
#ifndef DASHGRAPHICS_H
#define DASHGRAPHICS_H

#include <string>
using std::string;
#include <SDL_rotozoom.h>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <exception>
using std::exception;
//#include <gccore.h>

#include "../Platform/OS.h"
using Platform::OS;


class DashGraphics
{
private:

  DashGraphics();
  OS& sys;

  
public:

  DashGraphics(OS& os);

  SDL_Surface *background;
  SDL_Surface *backgroundBlurred;
  SDL_Surface *borderImage;
  SDL_Surface *topBottomBorderImage;
  
  //TODO: fix this
  SDL_Surface *cursorImage;
  SDL_Rect cursorFrames[16];

  bool Init();
  void Cleanup();
};


#endif
