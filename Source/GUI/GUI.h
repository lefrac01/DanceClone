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

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "../Platform/OS.h"
using Platform::OS;
using Platform::InputChannel;
#include "GUIGraphics.h"
#include "../Platform/Tools.h"
#include "Container.h"
#include "Element.h"


namespace Gooey
{

class GUI
{
private:
  
  GUI();
  OS& sys;
  Graphics gfx;
  FontBitmap defaultFontBitmap;
  FontBitmapCacheMap fontBitmapCache;
  FontBitmap* GetCachedFont(string filename, int pointSize, Uint32 colour);
  
  
public:

  Container screen;
  bool hideCursor;
  vector<string> fonts;
  int cursorRenderXOffset;
  int cursorRenderYOffset;
  float cursorRenderZoom;
  
  GUI(OS& os);
  bool Init();
  void Update();
  void Cleanup();
  void DrawButton(Button& b);
  bool DoButton(int x, int y,int w,int h,bool center,bool clickable,char* text);
  void DrawText(int x, int y, string text);
  void DrawText(int x, int y, string text, Element::Font f, Uint32 colour, int pointSize);
  void DrawLabel(Label& l);
  void SavePngScreenshot(); //TODO: fix
  void SetScreen(Container& c);
  void Render(Container& c);
};

}
#endif
