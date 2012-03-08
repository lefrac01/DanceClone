//      GUI.h
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
#ifndef GUI_H
#define GUI_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "../Platform/OS.h"
using Platform::OS;
#include "GUIGraphics.h"
#include "../Platform/Tools.h"

namespace Gooey
{

class GUI
{
private:
  
  GUI();
  OS& sys;
  Graphics gfx;
  bool useCursors;
  bool useCursor[4];
  
public:

  GUI(OS& os);
  bool Init();
  void Update();
  void Cleanup();
  void DrawSpriteText(int posx,int posy,char* texttosprite,int leftmiddleright);
  void DrawButton(int x, int y,int w,int h,bool glow);
  bool DoButton(int x, int y,int w,int h,bool center,bool clickable,char* text);
  int SpriteTextWrapped(int posx,int posy,char* texttosprite,int length);
  void SpriteText(int posx,int posy,char* texttosprite,int leftmiddleright);
  void SpriteTextColored(int posx,int posy,char* texttosprite,int leftmiddleright);
  void SetSpriteTextColored(Uint32 color);
  void SavePngScreenshot();
  //void WiiDash_savebmpscreenshot();
};

}
#endif