//      GUIGraphics.h // Gah.  using 2 namespaces in same project compiled with make is not working.
// at least at my level of competence, the makefile is dumping all .o files into the same build
// dir and overwriting the Gooey::Graphics .o with the DanceClone::Graphics one :(
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
#ifndef GOOEYGRAPHICS_H
#define GOOEYGRAPHICS_H

#include <string>
using std::string;
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <exception>
using std::exception;
#include <gccore.h>

#include "../Platform/OS.h"
using Platform::OS;

namespace Gooey
{

class Graphics 
{
private:

  Graphics();
  OS& sys;
  
public:
/*
  SDL_Surface *WDcursorimage;
  SDL_Rect WDcursorframes[16];
  SDL_Surface *WDfontimage;
  SDL_Rect WDfontframes[95];
  SDL_Surface *WDbuttonimage;
  SDL_Rect WDbuttonframes[18];
  SDL_Surface *WDarrowsimage;
  SDL_Rect WDarrowsframes[4];
  SDL_Surface *WDfontimagecolored;
  */
  SDL_Surface *cursorImage;
  SDL_Rect cursorFrames[16];
  SDL_Surface *fontImage;
  SDL_Rect fontFrames[95];
  SDL_Surface *buttonImage;
  SDL_Rect buttonFrames[18];
  SDL_Surface *arrowsImage;
  SDL_Rect arrowsFrames[4];
  SDL_Surface *fontImageColored;
  
  Graphics(OS& os);
  bool Init();
  void Cleanup();
};

}

#endif
