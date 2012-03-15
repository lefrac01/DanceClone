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
  LOG(DEBUG_MINOR, "Sound::Init()" << endl)
  #ifdef LINUX
  music = NULL;
  Mix_OpenAudio(0,0,0,0);
  #endif
  #ifdef WIN
  music = NULL;
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
  #endif
}

void Sound::Cleanup()
{
  LOG(DEBUG_MINOR, "Sound::Cleanup()" << endl)
  StopMusic();
  FreeMusic();
  #ifdef LINUX
  Mix_CloseAudio();
  #endif
  #ifdef WIN
  Mix_CloseAudio();
  #endif
}

void Sound::PrepMusic(string path)
{
  LOG(DEBUG_MINOR, "Sound::PrepMusic()" << endl)
  #ifdef LINUX
    music = Mix_LoadMUS(path.c_str());
  #endif
  #ifdef WII
    ASND_Init();
    MP3Player_Init();
    FILE *BGM = 0;
    //long lSize;
    //size_t result;
    //BGM = fopen(temptext, "rb");
    BGM = fopen(path.c_str(), "rb");
    fseek(BGM, 0, SEEK_END);
    mp3LSize = ftell (BGM);
    rewind(BGM);
    mp3Buffer = (char*)malloc(sizeof(char)*mp3LSize);
    //result = fread (buffer,1,lSize,BGM);
    fread(mp3Buffer, 1, mp3LSize, BGM);
    fclose(BGM);
  #endif
  #ifdef WIN
    music = Mix_LoadMUS(path.c_str());
  #endif
}

void Sound::StartMusic()
{
  LOG(DEBUG_MINOR, "Sound::StartMusic()" << endl)
  #ifdef LINUX
    Mix_PlayMusic(music,0);
  #endif
  #ifdef WII
    MP3Player_PlayBuffer(mp3Buffer, mp3LSize, NULL);
    free(mp3Buffer);
  #endif
  #ifdef WIN
    Mix_PlayMusic(music,0);
  #endif
}

void Sound::StopMusic()
{
  LOG(DEBUG_MINOR, "Sound::StopMusic()" << endl)
  #ifdef LINUX
  Mix_HaltMusic();
  #endif
  #ifdef WII
  MP3Player_Stop();
  #endif
  #ifdef WIN32
  Mix_HaltMusic();
  #endif
}

void Sound::FreeMusic()
{
  LOG(DEBUG_MINOR, "Sound::FreeMusic()" << endl)
  #ifdef LINUX
  Mix_FreeMusic(music);
  #endif
  #ifdef WII
    free(mp3Buffer);
  #endif
  #ifdef WIN32
  Mix_FreeMusic(music);
  #endif
}

bool Sound::MusicFinished()
{
  LOG(DEBUG_GUTS, "Sound::MusicFinished()" << endl)
  #ifdef LINUX
  if(!Mix_PlayingMusic())
  {
    Mix_FreeMusic(music);
    return true;
  }
  #endif
  #ifdef WII
  if(!MP3Player_IsPlaying())
  {
    MP3Player_Stop();
    return true;
  }
  #endif
  #ifdef WIN
  if(!Mix_PlayingMusic())
  {
    Mix_FreeMusic(music);
    return true;
  }
  #endif
  
  return false;
}

  
}
