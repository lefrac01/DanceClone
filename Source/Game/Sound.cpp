//NOTE: WAV files should use the following format:
// sample rate 48000 
// format WAV(Microsoft) Signed 16 bit PCM
//
// MP3 files are less picky.  A good bitrate is 192 kpbs.  More is 
// an unnecessary drain on the CPU.

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

Sound::Sound() :
soundPaths(NUM_SOUNDS),
sounds(NUM_SOUNDS)
{
}

bool Sound::Init()
{
  LOG(DEBUG_MINOR, "Sound::Init()" << endl)

  
  #ifdef LINUX
  music = NULL;
  Mix_OpenAudio(0,0,0,0);
  #endif
  #ifdef WII
  mp3Buffer = NULL;
  SND_Init(INIT_RATE_48000);  // note: SND_xxx is the same as ASND_xxx
  SND_Pause(0);   
  MP3Player_Init();
  #endif
  #ifdef WIN
  music = NULL;
  Mix_OpenAudio(0,0,0,0);
  #endif

 
  
  initialised = true;

  soundPaths[WoodBlock] = "Media/Game/woodblock.wav";
  soundPaths[HandClap] = "Media/Game/handclap.wav";
  soundPaths[RecordScratch] = "Media/Game/recordscratch.wav";
  soundPaths[MenuNav] = "Media/Game/menunav.wav";
  soundPaths[Select] = "Media/Game/select.wav";
  soundPaths[Switch] = "Media/Game/switch.wav";

  for (unsigned int i = 0; i < sounds.size(); ++i)
  {
    if (!sounds[i].Load(soundPaths[i]))
    {
      initialised = false;
      LOG(DEBUG_BASIC, "Sound::Init() returning false due to uninitialised sound at index " << i << " (file: " << soundPaths[i] << ")" << endl)
      break;
    }
  }
  
  return initialised;
}

void Sound::Cleanup()
{
  LOG(DEBUG_MINOR, "Sound::Cleanup()" << endl)
  
  #ifdef WII
  ASND_Pause(1);  //pause
  ASND_End();
  #endif
  
  StopMusic();
  #ifdef LINUX
  if (music)
  {
    Mix_FreeMusic(music);
    music = NULL;
  }
  Mix_CloseAudio();
  #endif
  #ifdef WII
  if(mp3Buffer)
  {
    free(mp3Buffer);
    mp3Buffer = NULL;
  }
  #endif
  #ifdef WIN
  if (music)
  {
    Mix_FreeMusic(music);
    music = NULL;
  }
  Mix_CloseAudio();
  #endif
}

void Sound::PrepMusic(string path)
{
  LOG(DEBUG_MINOR, "Sound::PrepMusic()" << endl)
  #ifdef LINUX
    music = Mix_LoadMUS(path.c_str());
    ready = music != NULL;
  #endif
  #ifdef WII

    FILE* BGM = 0;
    size_t result = 0;
    BGM = fopen(path.c_str(), "rb");
    if (BGM)
    {
      fseek(BGM, 0, SEEK_END);
      mp3LSize = ftell(BGM);
      rewind(BGM);
      mp3Buffer = (char*)malloc(sizeof(char)*mp3LSize);
      if (mp3Buffer)
      {
        result = fread(mp3Buffer, 1, mp3LSize, BGM);
        if (!result)
        {
          free(mp3Buffer);
          mp3Buffer = NULL;
        }
      }
      fclose(BGM);
    }
    ready = result != 0;
  #endif
  #ifdef WIN
    music = Mix_LoadMUS(path.c_str());
    ready = music != NULL;
  #endif
}

void Sound::StartMusic()
{
  LOG(DEBUG_MINOR, "Sound::StartMusic()" << endl)
  if (initialised && ready)
  {
    #ifdef LINUX
      Mix_PlayMusic(music,0);
    #endif
    #ifdef WII
      MP3Player_PlayBuffer(mp3Buffer, mp3LSize, NULL);
      free(mp3Buffer);
      mp3Buffer = NULL;
    #endif
    #ifdef WIN
      Mix_PlayMusic(music,0);
    #endif
  }
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

bool Sound::MusicFinished()
{
  LOG(DEBUG_GUTS, "Sound::MusicFinished()" << endl)
  #ifdef LINUX
  if(!Mix_PlayingMusic())
  {
    if (music)
    {
      Mix_FreeMusic(music);
      music = NULL;
    }
    return true;
  }
  #endif
  #ifdef WII
  if(!MP3Player_IsPlaying())
  {
    LOG(DEBUG_MINOR, "Sound::MusicFinished() (WII) detected !MP3Player_IsPlaying" << endl)
    MP3Player_Stop();
    return true;
  }
  #endif
  #ifdef WIN
  if(!Mix_PlayingMusic())
  {
    if (music)
    {
      Mix_FreeMusic(music);
      music = NULL;
    }
    return true;
  }
  #endif
  
  return false;
}


int Sound::PlaySample(Sample s)
{
  int channel = -1;
  if (sounds[s].initialised)
  {
    #ifdef LINUX
    //NOTE: this is not the recommended way of playing sounds under Linux
    // I am only doing this because this program is targeted at the WII
    // and Linux is a testing platform.  Use Mix_LoadWAV...
    channel = Mix_PlayChannel(-1, Mix_QuickLoad_RAW(sounds[s].data, sounds[s].length), 0) ;   // -1 : use first available channel, 0 no looping
    
    #endif
    #ifdef WII
    channel = ASND_GetFirstUnusedVoice();
    ASND_SetVoice(channel, sounds[s].channelCount, sounds[s].sampleRate, 0, sounds[s].data, sounds[s].length , 0xff, 0xff, NULL);
    #endif
  }
  return channel;
} 

void Sound::StopSoundChannel(int c)
{
  #ifdef WII
  ASND_StopVoice(c);
  #endif
}

}
