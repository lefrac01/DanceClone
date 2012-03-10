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

Element::Element(int t) :
  type(TEXT),
  tag(t),
  x(0),
  y(0),
  w(0),
  h(0),
  visible(true),
  active(true),
  clicked(false),
  text("")
{
}

Element::Element(int _x, int _y, int _w, int _h, bool v, bool a, bool c, string tx, int t) :
  type(TEXT),
  tag(t),
  x(_x),
  y(_y),
  w(_w),
  h(_h),
  visible(v),
  active(a),
  clicked(c),
  text(tx)
{
}


}
