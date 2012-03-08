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
  topBottomBorderImage(NULL)
{
}

bool DashGraphics::Init()
{
  LOG(DEBUG_BASIC, "DashGraphics::Init()" << endl)

//WDborderimage       = LoadOptimize("Media/WiiDash/Border.png");
//WDtopbottomborderimage  = LoadOptimize("Media/WiiDash/TopBottomBorder.png");
//WDbackground
//WDbackgroundblurred
  background = SDL_DisplayFormat(sys.vid.screen);
  backgroundBlurred = SDL_DisplayFormat(sys.vid.screen);
  borderImage = sys.vid.LoadOptimize("Media/WiiDash/Border.png");
  topBottomBorderImage = sys.vid.LoadOptimize("Media/WiiDash/TopBottomBorder.png");
  

  return (background && backgroundBlurred && borderImage && topBottomBorderImage);
}

void DashGraphics::Cleanup()
{
}
