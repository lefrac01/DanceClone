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
#include <string>
using std::string;

#include "../Platform/LOG.H"

namespace DanceClone
{
  
class Constants
{
private:

  
public:

  float arrowsHitAnimMs;
  int numDifficulties;
  long songStartOffset;
  
  long goalOffset;
  int maxPlayers;

  int preStartDelay;
  int songAbortDelay;
  int jumpAllowDelay;
  int booDelay;
  int goodDelay;
  int greatDelay;
  int perfectDelay;
  int marvellousDelay;
  int freezeFailDelay;
  int freezeLengthAllow;

  string musicFileRoot;
  int baseFileButtonTag;

/*
//TODO: enums
#define NOTE_TYPE_HOLD 1              
#define NOTE_TYPE_QUARTER 2           // 1 per beat, 4 per measure
#define NOTE_TYPE_EIGHTH 3            // 2 per beat, 8 per measure
#define NOTE_TYPE_QUARTER_TRIPLET 4   // 3 per beat, 12 per measure
#define NOTE_TYPE_SIXTEENTH 5         // 4 per beat, 16 per measure
#define NOTE_TYPE_EIGHTH_TRIPLET 6    // 6 per beat, 24 per measure
#define NOTE_TYPE_THIRTYSECOND 7      // 8 per beat, 32 per measure
#define NOTE_TYPE_SIXTEENTH_TRIPLET 8 // 12 per beat, 48 per measure
#define NOTE_TYPE_SIXTYFOURTH 9       // 16 per beat, 64 per measure
#define NOTE_TYPE_THIRTYSECOND_TRIPLET 10  // 24 per beat, 96 per measure
#define NOTE_TYPE_SIXTYFOURTH_TRIPLET 11   // 48 per beat, 192 per measure

#define RATING_NONE -1
#define RATING_MISS 0
#define RATING_GOOD 1
#define RATING_GREAT 2
#define RATING_PERFECT 3
#define RATING_MARVELLOUS 4

#define FREEZE_RATING_NONE -1
#define FREEZE_RATING_FAILED 0
#define FREEZE_RATING_OK 1
*/


  void Init(string ConfigFilePath = "");
};

}
