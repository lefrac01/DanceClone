//      Screen.cpp
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

#include "Screen.h"

namespace Gooey
{

Screen::~Screen()
{
  while (elements.size() > 0) 
  {
    delete elements.back();
    elements.pop_back();
  }
}

vector<Element*>& Screen::Elements()
{
  return elements;
}

void Screen::Add(Element* e)
{
  elements.push_back(e);
  LOG(DEBUG_MINOR, "Screen::Added() e: x: " << e->x << " y: " << e->y << " w: " << e->w << " h: " << e->h << " text: " << e->text << " tag: " << e->tag << endl)
  LOG(DEBUG_MINOR, "new element count: " << elements.size() << endl)
}

}
