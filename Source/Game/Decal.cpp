//      Decal.cpp
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

#include "Decal.h"

namespace DanceClone
{

Decal::Decal(int p, SDL_Surface* s, AnimType ty, long start, int d) :
  player(p),
  surface(s),
  animType(ty),
  animStartTime(start),
  animDuration(d),
  frameDuration(-1),
  lastFrameLingers(false)
{
}

Decal::Decal(const Decal& b) :
  player(b.player),
  surface(b.surface),
  animType(b.animType),
  animStartTime(b.animStartTime),
  animDuration(b.animDuration),
  frameDuration(b.frameDuration),
  lastFrameLingers(b.lastFrameLingers)
{
  frameRects = b.frameRects;
}

const Decal& Decal::operator=(const Decal& b)
{
  player = b.player;
  surface = b.surface;
  animType = b.animType;
  animStartTime = b.animStartTime;
  animDuration = b.animDuration;
  frameDuration = b.frameDuration;
  lastFrameLingers = b.lastFrameLingers;
  frameRects = b.frameRects;
  return *this;
}

// calculate the frame to use from the vector of frame rects based on 
// animation start time, duration, frame duration, animation type
// and current time passed in.
// note the use of truncation to int after division means that if the
// division is by the number of available frames, the last frame
// will be returned only if the time passed in is exacty the maximum.
SDL_Rect* Decal::CurrentFrameRect(long currentTime)
{
  SDL_Rect* result = NULL;
  
  if (currentTime < animStartTime + animDuration)
  {
    int rectIndex = -1;
    switch(animType)
    {
    case Decal::Loop:
      if (frameDuration > 0)
      {
        // if only index count less than 2, % will crash so handle seperately
        if (frameRects.size() < 2)
        {
          rectIndex = 0;
        }
        else
        {
          rectIndex = ((int)((frameRects.size() - 1) * ((currentTime - animStartTime) / (float)frameDuration))) % (frameRects.size() - 1);
        }
      }
      break;
    case Decal::Once:
      if (frameDuration > 0)
      {
        rectIndex = (int)((frameRects.size() - 1) * ((currentTime - animStartTime) / (float)frameDuration));
        if (rectIndex >= (int)frameRects.size())
        {
          if (lastFrameLingers)
          {
            rectIndex = frameRects.size() - 1;
          }
          else
          {
            rectIndex = -1;
          }
        }
      }
      else
      {
        // frame duration is calculated so each frame is displayed the same length of time
        // using frameRects.size() or else the last frame index is generated only when the 
        // time passed in reaches maximum
        rectIndex = (int)(frameRects.size() * ((currentTime - animStartTime) / (float)animDuration));
        if (rectIndex >= (int)frameRects.size())
        {
          rectIndex = frameRects.size() - 1;
        }
      }
      break;
    case Decal::Static:
      rectIndex = 0;
      break;
    }
    
    if (rectIndex >= 0 && rectIndex < (int)frameRects.size())
    {
      result = frameRects[rectIndex];
    }
  }
  return result;
}

}
