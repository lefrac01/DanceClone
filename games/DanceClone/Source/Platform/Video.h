//      Video.h
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

#include <string>
using std::string;
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <exception>
using std::exception;
#include <gccore.h>

#include "LOG.H"

namespace Platform
{


class Video
{
private:

 
public:
  SDL_Surface* screen;
  GXRModeObj* rmode;

  Video();
  bool Init();
  void Cleanup();
  SDL_Surface* LoadOptimize(string filename);
  SDL_Surface* LoadOptimizeAlpha(string filename);
  void ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
  void Pump();
  int ScreenHeight();
  int ScreenWidth();
  Uint32 GetPixel(SDL_Surface *surface,int x,int y);
  void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
  void PutPixel16(SDL_Surface *surface, int x, int y, Uint32 pixel);
};

}
