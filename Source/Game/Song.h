//NOTE: stepmania file format at http://www.stepmania.com/wiki/The_.SM_file_format
//
//TODO: implement #STOPS:
//
//TODO: support loading from different kinds of step data files.  currently only one version of .SM

//      Song.h
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
#ifndef SONG_H
#define SONG_H

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;
//#include <cmath>

#include "SDL.h"

#include "../Platform/LOG.H"
#include "../Platform/Tools.h"

#include "Arrow.h"
#include "Constants.h"

namespace DanceClone
{

class BpmChange
{
public:
  float beat;
  float timestamp;
  float bpm;
  BpmChange(float b, float t, float bp);
};

class BeatTick
{
public:
  long  yPos;
  float timestamp;
  float beat;
  BeatTick(long y, float t, float b);
};


class Song
{
private:
  bool ReadStepData();
  bool ParseBpms(const string& s, vector<BpmChange>& v);
  string SMValGetString(const string& s);
 

public:
  Song();
  bool loaded;
  bool prepared;
  string filename;
  string bannerImageFilename;
  string backgroundImageFilename;
  float beat0Offset;
  vector<string> stepFileLines;
  vector< vector<Arrow> > arrows;
  vector<BpmChange> bpmChanges;
  vector<bool> difficultyAvailable;
  vector<BeatTick> beatTicks;
  SDL_Surface* backgroundImage;
  
  void Init();
  bool Load(string& f);
  bool Prepare();
  string Path();
  string Name();
  string StepFilePath();
  string ScorePath();
  string BannerImagePath();
  string BackgroundImagePath();
  bool DifficultyIsAvailable(int difficulty);  
};

}

#endif
