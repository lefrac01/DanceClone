//      SongMenuItem.cpp
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

#include "SongMenuItem.h"


namespace DanceClone
{

SongMenuItem::SongMenuItem(OS& os, Song& s) :
sys(os),
song(s),
banner(NULL),
bannerMini(NULL)
{
  if (song.bannerImageFilename != "")
  {
    banner = sys.vid.LoadOptimize(song.BannerImagePath());
    if (banner)
    {
      // if necessary, resize the banner image to match app banner size
      if (banner->w != constants.bannerSize || banner->h != constants.bannerSize)
      {
        SDL_Surface* temp = banner;
        banner = zoomSurface(temp, (float)constants.bannerSize / banner->w, (float)constants.bannerSize / banner->h, 0);
        SDL_FreeSurface(temp);
      }
      bannerMini = zoomSurface(banner, constants.bannerMiniZoomFactor, constants.bannerMiniZoomFactor, 0);
    }
    else
    {
      LOG(DEBUG_BASIC, "WARN: failed to read banner image " << song.BannerImagePath() << " for song " << song.Name() << endl)
    }
  }
}

SongMenuItem::~SongMenuItem()
{
  if (banner)
  {
    SDL_FreeSurface(banner);
    banner = NULL;
  }
  if (bannerMini)
  {
    SDL_FreeSurface(bannerMini);
    bannerMini = NULL;
  }
}

SongMenuItem::SongMenuItem(const SongMenuItem& b) : 
sys(b.sys),
song(b.song),
banner(NULL),
bannerMini(NULL)
{
  if (b.banner)
  {
    banner = SDL_DisplayFormat(b.banner);
  }
  if (b.bannerMini)
  {
    bannerMini = SDL_DisplayFormat(b.bannerMini);
  }
}

SongMenuItem& SongMenuItem::operator=(const SongMenuItem& b)
{
  song = b.song;
  if (b.banner) banner = SDL_DisplayFormat(b.banner);
  if (b.bannerMini) bannerMini = SDL_DisplayFormat(b.bannerMini);
  return *this;
}

}
