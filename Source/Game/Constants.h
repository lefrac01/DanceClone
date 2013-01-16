//      Constants.h
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
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "../Platform/OS.h"
#include "../Platform/LOG.H"

namespace DanceClone
{
class Constants;
extern Constants constants;
  
class Constants
{
private:

  
public:

  float arrowsHitAnimMs;
  int homeArrowAnimDelay;
  int numDifficulties;
  static const char* difficultyText[];
  int internalMp3BufferDelay;
  int internalWavBufferDelay;
    // once a function is called to play sound,
    // a silence-filled buffer is the initial input source.  user data
    // begins to play once this silence has been consumed by the playback
    // process, so to get precise synchronisation, call all sound
    // functions this many milliseconds before the sound should be heard.
    // note this is a system-dependant value that can be calculated based
    // on buffer size.  all audio is 16 bit stereo at 48000 Hz so 
    // buffer size / 4 / 48000 * 1000 gives the number of ms delay.
    
  
  int goalOffset;
  int maxPlayers;
  int playerArrowColumnWidth;
  int playerArrowFieldMargin;
  
  int preStartDelay;
  int songStartAllow;
  unsigned int songAbortDelay;
  int jumpAllowDelay;
  int booDelay;
  unsigned int goodDelay;
  unsigned int greatDelay;
  unsigned int perfectDelay;
  unsigned int marvellousDelay;
  unsigned int freezeFailDelay;
  int freezeLengthAllow;
  float marvellousEnergy;
  float perfectEnergy;
  float greatEnergy;
  float goodEnergy;
  float booEnergy;
  float missEnergy;
  float okEnergy;
  float ngEnergy;
  int energyFailAbortDelay;
  
  string musicFileRoot;
  int baseFileButtonTag;

  float pixelsPerMsAt1Bpm;

  int bannerSize;
  float bannerMiniZoomFactor;

  int maxRecordFile;
  
  void Init(string ConfigFilePath = "");
};

}

#endif
