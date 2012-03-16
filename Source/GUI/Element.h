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

namespace Gooey
{

class Element
{
private:



public:
  
  const static Uint32 noColour = -1;

  Element();
  virtual ~Element(){};
  Element(int _x, int _y, int _w, int _h, bool v, bool a, bool c, string tx, int t = -1);

  int tag;
  int x;
  int y;
  int w;
  int h;
  bool visible;
  bool active;
  bool clicked;
  Uint32 colour;
  string text;
  virtual bool Clicked(int testx, int testy) = 0;
  virtual void CursorAt(int testx, int testy) = 0;
  void SetColour(Uint32 c);
};


}

#endif
