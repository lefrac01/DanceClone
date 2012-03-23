//      SongMenuItem.h
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

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <SDL_rotozoom.h>
//#include <SDL_image.h>
#include <SDL.h>

#include "../Platform/OS.h"
using Platform::OS;
#include "../Platform/LOG.H"

#include "Constants.h"
#include "Song.h"

namespace DanceClone
{

class SongMenuItem
{
private:
  OS& sys;
  Constants& constants;
  SongMenuItem();
  
public:
  SongMenuItem(OS& os, Constants& c, Song& s);
  SongMenuItem(const SongMenuItem& b);
  SongMenuItem& operator=(const SongMenuItem& b);
  ~SongMenuItem();
  
  Song& song;
  SDL_Surface* banner;
  SDL_Surface* bannerMini;

  void Prepare();
  void Cleanup();
};

}


