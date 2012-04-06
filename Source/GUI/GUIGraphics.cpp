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
  buttonImage(NULL),
  arrowsImage(NULL)
{
}

//TODO: detail when returning false
bool Graphics::Init()
{
  LOG(DEBUG_MINOR, "Gooey::Graphics::Init()" << endl)

  string cursorImagePath = "Media/Gui/Cursor.PNG";
  string buttonImagePath = "Media/Gui/Button.png";
  string arrowsImagePath = "Media/Gui/Arrows.png";
  
  
  cursorImage = sys.vid.LoadOptimizeAlpha(cursorImagePath.c_str());
  if (!cursorImage) LOG(DEBUG_BASIC, "failed to load \"" << cursorImagePath << "\"" << endl)

  buttonImage = sys.vid.LoadOptimizeAlpha(buttonImagePath.c_str());
  if (!buttonImage) LOG(DEBUG_BASIC, "failed to load \"" << buttonImagePath << "\"" << endl)

  arrowsImage = sys.vid.LoadOptimizeAlpha(arrowsImagePath.c_str());
  if (!arrowsImage) LOG(DEBUG_BASIC, "failed to load \"" << arrowsImagePath << "\"" << endl)


  for(int b = 0; b < 3; b++)
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

  return (cursorImage && buttonImage && arrowsImage);
}

void Graphics::Cleanup()
{
  if (cursorImage)
  {
    SDL_FreeSurface(cursorImage);
    cursorImage = NULL;
  }  
  if (buttonImage)
  {
    SDL_FreeSurface(buttonImage);
    buttonImage = NULL;
  }  
  if (arrowsImage)
  {
    SDL_FreeSurface(arrowsImage);
    arrowsImage = NULL;
  }
}

}
