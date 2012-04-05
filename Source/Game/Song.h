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
#include <map>
using std::map;

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;

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

class Song
{
public:
  enum RadarValues
  {
    Voltage,
    Stream,
    Chaos,
    Freeze,
    Air,
    NumRadarValues
  };

private:
  bool ReadBaseData();
  void ReadRecords();
  bool ParseBpms(const string& s, vector<BpmChange>& v);
  string SMValGetString(const string& s);
 

public:
  Song();
  Song(const Song& s);
  Song& operator= (const Song& b);
  
  bool loaded;
  bool prepared;
  string filename;
  string title;
  string bannerImageFilename;
  string backgroundImageFilename;
  float beat0Offset;
  float length;
  vector<string> stepFileLines;
  vector< vector<Arrow> > arrows;
  vector< vector<float> > radarValues;
  vector< vector<float> > records;
  vector< vector<int> > fullCombos;
  vector<BpmChange> bpmChanges;
  vector<bool> difficultyAvailable;
  vector<int> stepRatings;
  SDL_Surface* backgroundImage;
  
  void Init();
  bool Load(string& f);
  void VerifyRadarData();
  bool Prepare();
  string Path();
  string Name();
  string Title();
  string StepFilePath();
  string RecordPath();
  string BannerImagePath();
  string BackgroundImagePath();
  float MinBPM();
  float MaxBPM();
  bool DifficultyIsAvailable(int difficulty);  
  void WriteRecords();
  float GetRecord(int recordFile, int difficulty);
  void SetRecord(int recordFile, int difficulty, float record);
  bool GetFullCombo(int recordFile, int difficulty);
  void SetFullCombo(int recordFile, int difficulty);
  static float MsPerBeatFromBPM(float bpm);
};

}

#endif
