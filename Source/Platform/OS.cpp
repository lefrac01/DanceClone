//      OS.cpp
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

#include "OS.h"

namespace Platform
{
DirectoryEntry::DirectoryEntry(char* f, bool isf, bool sf, int extra) :
filename(f),
folder(isf),
statfailed(sf),
extrainfoint(extra)
{
}


OS::OS(Video& v, Input& i) :
  vid(v),
  input(i)
{
}

//TODO: bool
void OS::Init()
{
  srand((int)time(NULL));

  vid.Init();
  input.Init();
  
  SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);
}

void OS::Pump()
{
  LOG(DEBUG_GUTS, " OS::Pump" << endl)
  SDL_PumpEvents();
  vid.Pump();
  input.Update();
}

void OS::Cleanup()
{
  SDL_Quit();
}

}
