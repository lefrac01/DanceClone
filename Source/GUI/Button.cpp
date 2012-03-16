//      Button.cpp
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

#include "Button.h"

namespace Gooey
{
  
Button::Button(string s, int t) :
  Element(0, 0, 0, 0, true, true, false, s, t),
  state(NORMAL)
{
}

Button::Button(string s, int _x, int _y, int _w, int _h, int t) :
  Element(_x, _y, _w, _h, true, true, false, s, t),
  state(NORMAL)
{
  //TODO: this is from orig drawing code (still used!) the extents of the
  // button are modified to allow for the button edge graphics so use this
  // for the moment for click detection
  //if(useCursor[a] && sys.input.cursorx[a]>x-14 && sys.input.cursory[a]>y-14 && sys.input.cursorx[a]<x+w+13 && sys.input.cursory[a]<y+h+13)
  testExtent.x = x-14;
  testExtent.y = y-14;
  testExtent.w = w+14;
  testExtent.h = h+20;
}

Button::~Button()
{
}

bool Button::Clicked(int testx, int testy)
{
  // is point in extent?
  LOG(DEBUG_GUTS, "Button::Clicked() testing my rect: x: " << TestExtent().x << " y: " << TestExtent().y << " w: " << TestExtent().w << " h: " << TestExtent().h << " tag:" << tag << endl)
  if (active && PointInSDLRect(testx, testy, TestExtent()))
  {
    LOG(DEBUG_GUTS, "Button::Clicked() point in my rect  testx: " << testx << " testy: " << testy << " x: " << x << " y: " << y << " tag:" << tag << endl)
    clicked = true;
    state = DOWN;
    return true;
  }
  
  return false;
}

void Button::CursorAt(int testx, int testy)
{
  // is point in extent?
  LOG(DEBUG_GUTS, "Button::CursorAt() testing my rect: x: " << TestExtent().x << " y: " << TestExtent().y << " w: " << TestExtent().w << " h: " << TestExtent().h << " tag:" << tag << endl)
  if (active && PointInSDLRect(testx, testy, TestExtent()))
  {
    LOG(DEBUG_GUTS, "Button::CursorAt() point in my rect  testx: " << testx << " testy: " << testy << " x: " << x << " y: " << y << " tag:" << tag << endl)
    if (!clicked) state = HOVER;
  }
  else
  {
    if (!clicked) state = NORMAL;
  }
}


SDL_Rect& Button::TestExtent()
{
  return testExtent;
}


}
