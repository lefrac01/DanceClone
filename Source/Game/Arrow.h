//      Arrow.h
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
#ifndef ARROW_H
#define ARROW_H

#include "../Platform/LOG.H"

namespace DanceClone
{

class Arrow
{
public:
  enum Rating
  {
    RATING_NONE,
    MISS,
    GOOD,
    GREAT,
    PERFECT,
    MARVELLOUS,
    NUM_RATINGS
  };
  
  enum FreezeRating
  {
    FREEZE_NONE,
    FREEZE_FAILED,
    FREEZE_OK,
    NUM_FREEZE_RATINGS
  };

  enum NoteType
  {
    NOTE_TYPE_NONE,
    HOLD,
    QUARTER,              // 1 per beat, 4 per measure
    EIGHTH,               // 2 per beat, 8 per measure
    QUARTER_TRIPLET,      // 3 per beat, 12 per measure
    SIXTEENTH,            // 4 per beat, 16 per measure
    EIGHTH_TRIPLET,       // 6 per beat, 24 per measure
    THIRTYSECOND,         // 8 per beat, 32 per measure
    SIXTEENTH_TRIPLET,    // 12 per beat, 48 per measure
    SIXTYFOURTH,          // 16 per beat, 64 per measure
    THIRTYSECOND_TRIPLET, // 24 per beat, 96 per measure
    SIXTYFOURTH_TRIPLET,  // 48 per beat, 192 per measure
  };

private:


public:
  int direction;
  long time;
  long yPos;
  long length;
  int type; // for drawing based on note value
  Rating rating;
  FreezeRating freezeRating;
  bool jump;
  bool hidden;
  long animStartTime;
  Arrow(int d, long t, long y, long l, int ty);

  void Init();
};

}

#endif
