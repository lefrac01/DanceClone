//      Element.h
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

#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <string>
using std::string;

#include <SDL.h>

#include "FontBitmap.h"

namespace Gooey
{

class Element
{
public:
  enum OffsetMode
  {
    Top     = (1u << 1),
    Bottom  = (1u << 2),
    Left    = (1u << 3),
    Right   = (1u << 4),
    HCenter = (1u << 5),
    VCenter = (1u << 6),
  };

enum Font
  {
    DefaultFont,
    AeroliteSky,
    CardewThree,
    Cryta,
    D3Euronism,
    Future,
    Research,
    NUM_FONTS
  };


  
  const static Uint32 noColour = -1;

  Element();
  virtual ~Element(){};
  Element(int _x, int _y, int _w, int _h, bool v, bool a, string tx = "", int t = -1);

  int tag;
  int x;
  int y;
  int w;
  int h;
  unsigned int offsetMode;
  bool visible;
  bool active;
  bool clicked;
  int clickedBy;
  Uint32 colour;
  string text;
  Font font;
  int pointSize;
  virtual bool Clicked(int testx, int testy, int channel) = 0;
  virtual void CursorAt(int testx, int testy) = 0;
  void SetColour(Uint32 c);
};


}

#endif
