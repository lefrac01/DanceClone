//      Sound.cpp
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

#include "Sound.h"

namespace DanceClone
{
  
void Sound::Init()
{
  LOG(DEBUG_MINOR, "DanceClone::Sound::Init()" << endl)
  #ifdef WIN
  Mix_Music* music = NULL;
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
  #endif
}

}
