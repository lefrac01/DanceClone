//      Screen.h
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

#ifndef GUI_SCREEN_H
#define GUI_SCREEN_H

#include <vector>
using std::vector;

#include "../Platform/LOG.H"

#include "Element.h"
#include "Image.h"
#include "Button.h"
#include "Container.h"

namespace Gooey
{
  
class Screen
{
private:

  vector<Image> images;
  vector<Button> buttons;
  
public:
  ~Screen();
  vector<Image>& Images();
  vector<Button>& Buttons();
  //vector<Element*>& Elements();
  void Add(Element& e);
};


}

#endif
