//      Element.cpp
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

#include "Element.h"

namespace Gooey
{

Element::Element() :
  tag(-1),
  x(-1),
  y(-1),
  w(-1),
  h(-1),
  offsetMode(Top | Left),
  visible(true),
  active(true),
  clicked(false),
  clickedBy(-1),
  colour(noColour),
  text(""),
  font(DefaultFont),
  pointSize(24)
{
}

Element::Element(int _x, int _y, int _w, int _h, bool v, bool a, string tx, int t) :
  tag(t),
  x(_x),
  y(_y),
  w(_w),
  h(_h),
  offsetMode(Top | Left),
  visible(v),
  active(a),
  clicked(false),
  clickedBy(-1),
  colour(noColour),
  text(tx),
  font(DefaultFont),
  pointSize(24)
{
}

void Element::SetColour(Uint32 c)
{
  colour = c;
}

}
