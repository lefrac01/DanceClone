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
}

vector<Button>& Screen::Buttons()
{
  return buttons;
}

vector<Image>& Screen::Images()
{
  return images;
}

void Screen::Add(Element& e)
{
  try 
  {
    Button& b = dynamic_cast<Button&>(e);
    buttons.push_back(b);
    LOG(DEBUG_MINOR, "Screen::Add() button b: x: " << b.x << " y: " << b.y << " w: " << b.w << " h: " << b.h << " text: " << b.text << " tag: " << b.tag << endl)
  }
  catch (...){}
  try 
  {
    Image& i = dynamic_cast<Image&>(e);
    images.push_back(i);
    LOG(DEBUG_MINOR, "Screen::Add() image i: x: " << i.x << " y: " << i.y << " w: " << i.w << " h: " << i.h << " text: " << i.text << " tag: " << i.tag << endl)
  }
  catch (...){}
  
}

}
