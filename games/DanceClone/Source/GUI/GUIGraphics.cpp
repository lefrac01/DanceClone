//      GUIGraphics.cpp
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

#include "GUIGraphics.h"

namespace Gooey
{

Graphics::Graphics(OS& os) :
  sys(os),
  cursorImage(NULL),
  fontImage(NULL),
  buttonImage(NULL),
  arrowsImage(NULL),
  fontImageColored(NULL)
{
}

//TODO: detail when returning false
bool Graphics::Init()
{
  LOG(DEBUG_BASIC, "Gooey::Graphics::Init()" << endl)

  cursorImage = sys.vid.LoadOptimizeAlpha("Media/WiiDash/Cursor.png");;
  fontImage = sys.vid.LoadOptimizeAlpha("Media/WiiDash/Font.png");;
  buttonImage = sys.vid.LoadOptimizeAlpha("Media/WiiDash/Button.png");;
  fontImageColored = sys.vid.LoadOptimizeAlpha("Media/WiiDash/Font.png");;
  arrowsImage = sys.vid.LoadOptimizeAlpha("Media/WiiDash/Arrows.png");;
  
  //WDfontimage       = LoadOptimizeAlpha("Media/WiiDash/Font.png");
  
//TODO: move safe fat init to earlier place in WiiOS...
//#ifdef WII
//  while(!WDfontimage){
//  WDfontimage     = LoadOptimizeAlpha("Media/WiiDash/Font.png");
//  fatInitDefault();}
//#endif
//WDbuttonimage       = LoadOptimizeAlpha("Media/WiiDash/Button.png");
//WDarrowsimage       = LoadOptimize("Media/WiiDash/Arrows.png");
//WDcursorimage       = LoadOptimizeAlpha("Media/WiiDash/Cursor.png");
//WDfontimagecolored    = LoadOptimizeAlpha("Media/WiiDash/Font.png");

  for(int a = 0; a < 95; a++)
  {
    fontFrames[a].x = a*15;
    fontFrames[a].y = 0;
    fontFrames[a].w = 15;
    fontFrames[a].h = 21;
  }
  for(int b = 0; b < 2; b++)
  {
    for(int a = 0; a < 9; a++)
    {
      buttonFrames[a + b*9].x = a*21;
      buttonFrames[a + b*9].y = b*21;
      buttonFrames[a + b*9].w = 21;
      buttonFrames[a + b*9].h = 21;
    }
  }
  for(int a = 0; a < 4; a++)
  {
    arrowsFrames[a].x = a*16;
    arrowsFrames[a].y = 0;
    arrowsFrames[a].w = 16;
    arrowsFrames[a].h = 16;
  }
  for(int a = 0; a < 16; a++)
  {
    cursorFrames[a].x = a*96;
    cursorFrames[a].y = 0;
    cursorFrames[a].w = 96;
    cursorFrames[a].h = 96;
  }

  return (cursorImage && fontImage && buttonImage && fontImageColored && arrowsImage);
}

void Graphics::Cleanup()
{
  if (cursorImage)
  {
    SDL_FreeSurface(cursorImage);
    cursorImage = NULL;
  }  
  if (fontImage)
  {
    SDL_FreeSurface(fontImage);
    fontImage = NULL;
  }  
  if (buttonImage)
  {
    SDL_FreeSurface(buttonImage);
    buttonImage = NULL;
  }  
  if (fontImageColored)
  {
    SDL_FreeSurface(fontImageColored);
    fontImageColored = NULL;
  }
  if (arrowsImage)
  {
    SDL_FreeSurface(arrowsImage);
    arrowsImage = NULL;
  }
}

}
