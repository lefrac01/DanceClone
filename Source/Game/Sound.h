//      Sound.h
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

#include "../Platform/LOG.H"
#include "../Platform/OS.h"

#ifdef WII
#include "SDL_mixer.h"
#include <mp3player.h>
#include <asndlib.h>
#else
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
#endif
#include "WAVSample.h"

namespace DanceClone
{

class Sound
{
public:

  enum Sample
  {
    WoodBlock,
    HandClap,
    RecordScratch,
    MenuNav,
    Select,
    Switch,
    NUM_SOUNDS
  };


private:

  #ifdef LINUX
  Mix_Music* music;
  #endif
  #ifdef WIN32
  Mix_Music* music;
  #endif
  char* mp3Buffer;
  long mp3LSize;


public:
  Sound();
  vector<string> soundPaths;
  vector<WAVSample> sounds;

  bool initialised;
  bool ready;
  
  bool Init();
  void Cleanup();
  void PrepMusic(string path);
  void StartMusic();
  void StopMusic();
  bool MusicFinished();
  int PlaySample(Sample s);
  void StopSoundChannel(int c);
};

}
