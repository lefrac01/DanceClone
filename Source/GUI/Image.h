//      Image.h
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

#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#include <SDL.h>

#include "../Platform/LOG.H"

#include "Element.h"

namespace Gooey
{
  
class Image : public Element
{
private:
  Image();
  
public:
  virtual ~Image(){};
  Image(SDL_Surface* s);
  Image(SDL_Surface* s, int _x, int _y, int _w, int _h, int t = -1);
  SDL_Surface* surface;
  bool Clicked(int testx, int testy);
  void CursorAt(int testx, int testy);
};


}

#endif
