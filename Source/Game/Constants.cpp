//TODO: .ini for rating times
//      Constants.cpp
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

#include "Constants.h"

namespace DanceClone
{

Constants constants;

//TODO: game drawing constants like ms per pixel at 1 bpm and goal offset!
void Constants::Init(string ConfigFilePath)
{
  LOG(DEBUG_MINOR, "DanceClone::Constants::Init()" << endl)
  arrowsHitAnimMs = 200.0;
  numDifficulties = 5;
  songStartOffset = 100;

  goalOffset = 0;
  maxPlayers = 2;
  preStartDelay = 3000;
  songAbortDelay = 3000;
  jumpAllowDelay = 100;
  booDelay = 200;
  goodDelay = 200;
  greatDelay = 100;
  perfectDelay = 50;
  marvellousDelay = 25;
  freezeFailDelay = 250;
  freezeLengthAllow = 25;
  // how many pixels before the end of a freeze arrow the player is
  //  allowed to lift off without failing the freeze

  musicFileRoot = "Music/";

  /*
  #ifdef WII
    musicFileRoot = "/apps/DanceClone/Music/";
  #endif
  #ifdef LINUX
    musicFileRoot = "Music/";
  #endif
  #ifdef WIN
    musicFileRoot = "Music/";
  #endif
  */
  baseFileButtonTag = 1024;
  
  pixelsPerMsAt1Bpm = 0.0;
//    #endif  
}

}
