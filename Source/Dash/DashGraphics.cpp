//      Graphics.cpp
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

#include "DashGraphics.h"

DashGraphics::DashGraphics(OS& os) :
  sys(os),
  background(NULL),
  backgroundBlurred(NULL),
  borderImage(NULL),
  topBottomBorderImage(NULL),
  cursorImage(NULL)
{
}

bool DashGraphics::Init()
{
  LOG(DEBUG_MINOR, "DashGraphics::Init()" << endl)

  background = SDL_DisplayFormat(sys.vid.screen);
  backgroundBlurred = SDL_DisplayFormat(sys.vid.screen);
  
  
  string borderImagePath ="Media/WiiDash/Border.png";
  string topBottomBorderImagePath = "Media/WiiDash/TopBottomBorder.png";
  string cursorImageImagePath = "Media/WiiDash/Cursor.PNG";

  borderImage = sys.vid.LoadOptimize(borderImagePath.c_str());
  if (!borderImage) LOG(DEBUG_BASIC, "failed to load \"" << borderImagePath << "\"" << endl)
  
  topBottomBorderImage = sys.vid.LoadOptimize(topBottomBorderImagePath.c_str());
  if (!topBottomBorderImage) LOG(DEBUG_BASIC, "failed to load \"" << topBottomBorderImagePath << "\"" << endl)
  
  cursorImage = sys.vid.LoadOptimizeAlpha(cursorImageImagePath.c_str());
  if (!cursorImage) LOG(DEBUG_BASIC, "failed to load \"" << cursorImageImagePath << "\"" << endl)
  
  for(int a = 0; a < 16; a++)
  {
    cursorFrames[a].x = a*96;
    cursorFrames[a].y = 0;
    cursorFrames[a].w = 96;
    cursorFrames[a].h = 96;
  }

  return (background && backgroundBlurred && borderImage && topBottomBorderImage && cursorImage);
}

void DashGraphics::Cleanup()
{
  LOG(DEBUG_MINOR, "DashGraphics::Cleanup()" << endl)
  if (borderImage)
  {
    SDL_FreeSurface(borderImage);
    borderImage = NULL;
  }
  if (topBottomBorderImage)
  {
    SDL_FreeSurface(topBottomBorderImage);
    topBottomBorderImage = NULL;
  }
  if (cursorImage)
  {
    SDL_FreeSurface(cursorImage);
    cursorImage = NULL;
  }
  if (background)
  {
    SDL_FreeSurface(background);
    background = NULL;
  }
  if (backgroundBlurred)
  {
    SDL_FreeSurface(backgroundBlurred);
    backgroundBlurred = NULL;
  }
}
