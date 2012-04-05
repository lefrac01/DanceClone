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

#include "../Platform/OS.h" // which platform is defined...
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
  int numDifficulties;
  static const char* difficultyText[];
  long songStartOffset;
  
  long goalOffset;
  int maxPlayers;
  int playerArrowColumnWidth;
  int playerArrowFieldMargin;
  
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

  float pixelsPerMsAt1Bpm;

  int bannerSize;
  float bannerMiniZoomFactor;

  int maxRecordFile;
  
  void Init(string ConfigFilePath = "");
};

}

#endif
