//      FontBitmap.h
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

#ifndef FONT_BITMAP_H
#define FONT_BITMAP_H

#include <map>
using std::map;

#include "SDL.h"
#include <SDL_ttf.h>

#include "../Platform/LOG.H"
#include "../Platform/OS.h"
using Platform::OS;

namespace Gooey
{
  
struct FontBitmapKey
{
  string filename;
  int pointSize;
  Uint32 colour;
  
  FontBitmapKey(string f, int p, Uint32 c) : filename(f), pointSize(p), colour(c) {};
};
struct FontBitmapKeyComp
{
  // should a be placed before b in a strict weak ordering
  bool operator() (const FontBitmapKey& a, const FontBitmapKey& b)
  {
    if (a.filename < b.filename) return true;
    else if (a.filename > b.filename) return false;
    else if (a.pointSize < b.pointSize) return true;
    else if (a.pointSize > b.pointSize) return false;
    else if (a.colour < b.colour) return true;
    else return false;
  }
};

class FontBitmap;
typedef map<FontBitmapKey, FontBitmap*, FontBitmapKeyComp> FontBitmapCacheMap;

class FontBitmap
{
private:

  FontBitmap();
  
  static const char* fontCharsAll[];
  static const char* fontCharsNumeric[];
  
  OS& sys;
  
public:

  FontBitmap(OS& os, string f, int p, Uint32 c);
  ~FontBitmap();

  string filename;
  int pointSize;
  Uint32 colour;
  int height;
  TTF_Font* ttfFont;
  map<char, SDL_Rect> charFrames;
  bool ready;
  //  bool numeric;   // possible optimisation rendering and storing only numbers for fonts not using other chars
  SDL_Surface* surface;
  
  bool CharExists(char c);
  SDL_Rect* FrameFromChar(char c);
};


}

#endif
