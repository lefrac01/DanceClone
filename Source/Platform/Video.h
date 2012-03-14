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
#ifndef VIDEO_H
#define VIDEO_H

#include <string>
using std::string;
#include <SDL_rotozoom.h>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <exception>
using std::exception;
//#include <gccore.h>

#include "LOG.H"

namespace Platform
{


class Video
{
private:
  signed char Bresenhamix;
  signed char Bresenhamiy;
  int Bresenhamdelta_x;
  int Bresenhamdelta_y;
  int Bresenhamerror;
  
  
protected:

  int screenWidth;
  int screenHeight;

  
public:

  SDL_Surface* screen;

  Video();
  virtual bool Init() = 0;
  virtual void Cleanup();
  SDL_Surface* LoadOptimize(string filename);
  SDL_Surface* LoadOptimizeAlpha(string filename);
  void ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
  virtual void Pump();
  int ScreenHeight();
  int ScreenWidth();
  Uint32 GetPixel(SDL_Surface *surface,int x,int y);
  void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
  void PutPixel16(SDL_Surface *surface, int x, int y, Uint32 pixel);
  void BresenhamDrawLine(int x1,int y1,int x2,int y2,SDL_Surface *surface,Uint32 pixel);
  bool Bresenham(int &x1,int &y1,int &x2,int &y2);
  void BresenhamStart(int &x1,int &y1,int x2,int y2);
};

}

#endif
