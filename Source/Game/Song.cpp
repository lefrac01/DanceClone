//TODO: load the different modes: dance-single, dance-double for start
// all at once. for now only loading dance-single

//      Song.cpp
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

#include "Song.h"

namespace DanceClone
{

BpmChange::BpmChange(float b, float t, float bp) : 
  beat(b),
  timestamp(t),
  bpm(bp)
{
}


Song::Song() :
  loaded(false),
  prepared(false),
  filename(""),
  title(""),
  bannerImageFilename(""),
  backgroundImageFilename(""),
  beat0Offset(0),
  length(-1),
  arrows(constants.numDifficulties),
  radarValues(constants.numDifficulties),
  records(constants.numDifficulties),
  fullCombos(constants.numDifficulties),
  difficultyAvailable(constants.numDifficulties, false),
  stepRatings(constants.numDifficulties, -1),
  backgroundImage(NULL)
{
}

Song::Song(const Song& b) :
  loaded(b.loaded),
  prepared(b.prepared),
  filename(b.filename),
  title(b.title),
  bannerImageFilename(b.bannerImageFilename),
  backgroundImageFilename(b.backgroundImageFilename),
  beat0Offset(b.beat0Offset),
  length(b.length),
  backgroundImage(NULL)
{
  stepFileLines = b.stepFileLines;
  arrows = b.arrows;
  radarValues = b.radarValues;
  records = b.records;
  fullCombos = b.fullCombos;
  bpmChanges = b.bpmChanges;
  difficultyAvailable = b.difficultyAvailable;
  stepRatings = b.stepRatings;
  if (b.backgroundImage)
  {
    backgroundImage = SDL_DisplayFormat(b.backgroundImage);
  }
}

Song& Song::operator= (const Song& b)
{
  loaded = b.loaded;
  prepared = b.prepared;
  filename = b.filename;
  title = b.title;
  bannerImageFilename = b.bannerImageFilename;
  backgroundImageFilename = b.backgroundImageFilename;
  beat0Offset = b.beat0Offset;
  length = b.length;
  stepFileLines = b.stepFileLines;
  arrows = b.arrows;
  radarValues = b.radarValues;
  records = b.records;
  fullCombos = b.fullCombos;
  bpmChanges = b.bpmChanges;
  difficultyAvailable = b.difficultyAvailable;
  stepRatings = b.stepRatings;
  backgroundImage = NULL;
  if (b.backgroundImage)
  {
    backgroundImage = SDL_DisplayFormat(b.backgroundImage);
  }
  return *this;
}
  
void Song::Init()
{
  LOG(DEBUG_MINOR, "DanceClone::Song::Init()" << endl)
  filename = "";
  title = "";
  bannerImageFilename = "";
  backgroundImageFilename = "";
  
  std::fill(difficultyAvailable.begin(), difficultyAvailable.end(), false);
  std::fill(stepRatings.begin(), stepRatings.end(), false);

  for (int i = 0; i < constants.numDifficulties; ++i)
  {
    arrows[i].clear();
    radarValues[i].resize(NumRadarValues, -1.0);
    records[i].resize(constants.maxRecordFile, -1.0);
    fullCombos[i].resize(constants.maxRecordFile, -1.0);
  }

  bpmChanges.clear();

  beat0Offset = 0;
  length = -1;
  
  loaded = false;
  prepared = false;
}

bool Song::Load(string& f)
{
  LOG(DEBUG_BASIC, "Song::Load(" << f << ")" << endl)
  if (loaded)
  {
    Init();
  }
  
  filename = f;

  if (ReadBaseData())
  {
    loaded = true;
  }
  else
  {
    LOG(DEBUG_MINOR, "Song::Load() failed to detect valid step data for current mode" << endl)
    loaded = false;
  }
  
  ReadRecords();
  
  return loaded;
}

void Song::VerifyRadarData()
{
  bool needRadarValues = false;
  for (int d = 0; d < constants.numDifficulties && !needRadarValues; ++d)
  {
    if (difficultyAvailable[d])
    {
      for (unsigned int i = 0; i < radarValues[d].size(); ++i)
      {
        if (radarValues[d][i] < 0)
        {
          LOG(DEBUG_DETAIL, "Song::VerifyRadarData() calling prep because radarValues[d][i] neg.  d: " << d << " i: " << i << endl)
          needRadarValues = true;
          break;
        }
      }
    }
  }
  if (needRadarValues)
  {
    Prepare();
  }
}

string Song::Path()
{
  string temp = constants.musicFileRoot;
  temp += filename;
  return temp;
}

string Song::Name()
{
  string temp = filename.substr(0, filename.size()-4);
  return temp;
}

string Song::Title()
{
  if (title == "")
  {
    return Name();
  }
  return title;
}

string Song::StepFilePath()
{
  string temp = Path();
  temp = temp.substr(0, temp.size()-4);
  temp += ".sm";
  return temp;
}

string Song::RecordPath()
{
  string temp = Path();
  temp = temp.substr(0, temp.size()-4);
  temp += ".records"; 
  return temp;
}

string Song::BannerImagePath()
{
  string temp = constants.musicFileRoot;
  temp += bannerImageFilename; 
  return temp;
}

string Song::BackgroundImagePath()
{
  string temp = constants.musicFileRoot;
  temp += backgroundImageFilename; 
  return temp;
}

float Song::MinBPM()
{
  float bpm = -1;
  for (unsigned int i = 0; i < bpmChanges.size(); ++i)
  {
    if (bpm == -1)
    {
      bpm = bpmChanges[i].bpm;
    }
    else if (bpmChanges[i].bpm < bpm)
    {
      bpm = bpmChanges[i].bpm;
    }
  }
  return bpm;
}

float Song::MaxBPM()
{
  float bpm = -1;
  for (unsigned int i = 0; i < bpmChanges.size(); ++i)
  {
    if (bpm == -1)
    {
      bpm = bpmChanges[i].bpm;
    }
    else if (bpmChanges[i].bpm > bpm)
    {
      bpm = bpmChanges[i].bpm;
    }
  }
  return bpm;
}

float Song::GetRecord(int recordFile, int difficulty)
{
  float record = -1;
  if (recordFile >= 0  && recordFile < constants.maxRecordFile && difficulty >= 0 && difficulty < constants.numDifficulties)
  {
    record = records[difficulty][recordFile];
  }
  return record;
}

void Song::SetRecord(int recordFile, int difficulty, float record)
{
  if (recordFile >= 0  && recordFile < constants.maxRecordFile && difficulty >= 0 && difficulty < constants.numDifficulties)
  {
    records[difficulty][recordFile] = record;
  }
}

bool Song::GetFullCombo(int recordFile, int difficulty)
{
  int fc = 0;
  if (recordFile >= 0  && recordFile < constants.maxRecordFile && difficulty >= 0 && difficulty < constants.numDifficulties)
  {
    fc = fullCombos[difficulty][recordFile];
  }
  return fc == 1;
}

void Song::SetFullCombo(int recordFile, int difficulty)
{
  if (recordFile >= 0  && recordFile < constants.maxRecordFile && difficulty >= 0 && difficulty < constants.numDifficulties)
  {
    fullCombos[difficulty][recordFile] = 1;
  }
}

void Song::ReadRecords()
{
  LOG(DEBUG_MINOR, "Song::ReadRecords()" << endl)
  
  ifstream indata;
  indata.open(RecordPath().c_str());
  int r = -1;
  while (indata.good())
  {
    ++r;
    int d = -1;
    while (indata.good() && ++d < constants.numDifficulties)
    {
      indata >> records[d][r];
      indata >> fullCombos[d][r];
    }
  }  
  indata.close();
  
  LOG(DEBUG_DETAIL, "Song::ReadRecords(\""<<RecordPath()<<"\") obtained:" << endl)
  for (int r = 0; r < constants.maxRecordFile; ++r)
  {
    for (int d = 0; d < constants.numDifficulties; ++d)
    {
      LOG(DEBUG_DETAIL, "["<<r<<":"<<d<<"] " << records[d][r] << endl)
    }
  }
}

void Song::WriteRecords()
{
  LOG(DEBUG_DETAIL, "Song::WriteRecords()" << endl)
  
  ofstream outdata;
  outdata.open(RecordPath().c_str());
  for (int r = 0; r < constants.maxRecordFile; ++r)
  {
    for (int d = 0; d < constants.numDifficulties; ++d)
    {
      outdata << records[d][r] << endl;
      outdata << fullCombos[d][r] << endl;
    }
  }
  outdata.close();
}

bool Song::ReadBaseData()
{
  LOG(DEBUG_MINOR, "Song::ReadStepData()" << endl)

  // read file
  LOG(DEBUG_MINOR, "indata.open(" << StepFilePath() << ")" << endl)

  ifstream indata;
  indata.open(StepFilePath().c_str());
  string fileLine;
  while (getline(indata, fileLine)) stepFileLines.push_back(fileLine);
  indata.close();
  
  LOG(DEBUG_DETAIL, "got file contents of " << stepFileLines.size() << " lines. " << endl)
  
  // analyse contents
  string titleTag = "#TITLE:";
  string bannerTag = "#BANNER:";
  string backgoundTag = "#BACKGROUND:";
  string bpmTag = "#BPMS:";
  long bpmLine = -1;
  bool bpmsOk = false;
  string offsetTag = "#OFFSET:";
  bool foundOffset = false;
  string notesTag = "#NOTES:";
  unsigned int currentLine = 0;
  while (currentLine < stepFileLines.size())
  {
    if (stepFileLines[currentLine].find(titleTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "ReadStepData() found " << titleTag << endl)
      title = SMValGetString(stepFileLines[currentLine]);
    }
    else if (stepFileLines[currentLine].find(bannerTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "ReadStepData() found " << bannerTag << endl)
      bannerImageFilename = SMValGetString(stepFileLines[currentLine]);
    }
    else if (stepFileLines[currentLine].find(backgoundTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "ReadStepData() found " << backgoundTag << endl)
      backgroundImageFilename = SMValGetString(stepFileLines[currentLine]);
    }
    else if (stepFileLines[currentLine].find(bpmTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "ReadStepData() found " << bpmTag << endl)
      bpmLine = currentLine;
    }
    else if (stepFileLines[currentLine].find(offsetTag) != string::npos)
    {
      foundOffset = true;
      beat0Offset = -1000.0 * atof(SMValGetString(stepFileLines[currentLine]).c_str());
      LOG(DEBUG_DETAIL, "ReadStepData() found " << offsetTag << beat0Offset << endl)
    }
    else if (stepFileLines[currentLine].find(notesTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "ReadStepData() found #NOTES:" << endl)
      
      // identify difficulty
      unsigned int noteLineIndex = currentLine + 1;
      int currentDifficulty = -1;
      string stepRatingText = "";
      string grooveRadarText = "";
      while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
      {
        // look for dance-single data
        if (stepFileLines[noteLineIndex].find("dance-single:") != string::npos)
        {
          // difficulty is 2 lines down
          if (stepFileLines[noteLineIndex+2].find("Beginner:") != string::npos)
          {
            currentDifficulty = 0;
          }
          else if (stepFileLines[noteLineIndex+2].find("Easy:") != string::npos)
          {
            currentDifficulty = 1;
          }
          else if (stepFileLines[noteLineIndex+2].find("Medium:") != string::npos)
          {
            currentDifficulty = 2;
          }
          else if (stepFileLines[noteLineIndex+2].find("Hard:") != string::npos)
          {
            currentDifficulty = 3;
          }
          else if (stepFileLines[noteLineIndex+2].find("Challenge:") != string::npos)
          {
            currentDifficulty = 4;
          }
          // step rating is 3 lines down
          stepRatingText = stepFileLines[noteLineIndex+3];
          // groove radar values are 4 lines down
          grooveRadarText = stepFileLines[noteLineIndex+4];
          break;
        }
        ++noteLineIndex;
      }
      
      if (currentDifficulty == -1)
      {
        //error
      }
      else
      {
        LOG(DEBUG_DETAIL, "ReadStepData() found dance-single note data for difficulty" << currentDifficulty << endl)
        difficultyAvailable[currentDifficulty] = true;
        
        int stepRating = -1;
        if (stepRatingText.find(":") != string::npos)
        {
          string temp = stepRatingText.substr(stepRatingText.find_first_of("012345679"), stepRatingText.size() - stepRatingText.find_last_of("012345679"));
          stepRating = atoi(temp.c_str());
        }
        stepRatings[currentDifficulty] = stepRating;
        
        // e.g. groove radar data:
        // 0.062,0.000,0.036,0.009,0.000:
        // "voltage", "stream", "chaos", "freeze", and "air"
        if (grooveRadarText.find("0.000,0.000,0.000,0.000,0.000") == string::npos)
        {
          string subValue = "";
          size_t valStart = 0;
          size_t valEnd = 0;
          unsigned int radarValueIndex = 0;
          while ( radarValueIndex < radarValues.size()
              &&  (valStart = grooveRadarText.find_first_of("0123456789.", valStart)) != string::npos 
              &&  (valEnd   = grooveRadarText.find_first_of(",:", valStart)) != string::npos
              ) {
            subValue = grooveRadarText.substr(valStart, valEnd - valStart);
            radarValues[currentDifficulty][radarValueIndex] = atof(subValue.c_str());
            LOG(DEBUG_DETAIL, "got radar value " << radarValues[currentDifficulty][radarValueIndex] << " at index " << radarValueIndex << " from string " << grooveRadarText << endl)
            ++radarValueIndex;
            valStart = valEnd;
          }
        }
        
        // if all radar values were not read from file, they will be 
        // calculated from step data.
      }
    }
    
    ++currentLine;
    LOG(DEBUG_GUTS, "ReadStepData() current line incremented to " << currentLine << endl)
  }

  // parse bpm changes using offset
  if (foundOffset && bpmLine != -1)
  {
    bpmsOk = ParseBpms(stepFileLines[bpmLine], bpmChanges);
  }


  LOG(DEBUG_MINOR, "ReadStepData() done" << endl)
  
  return bpmsOk && (
       difficultyAvailable[0]
    || difficultyAvailable[1]
    || difficultyAvailable[2]
    || difficultyAvailable[3]
    || difficultyAvailable[4]
  );
}


bool Song::Prepare()
{
  LOG(DEBUG_BASIC, "Song::Prepare() for song " << filename << endl)

  if (prepared)
  {
    return true;
  }
  
  // analyse contents
  string notesTag = "#NOTES:";
  unsigned int currentLine = 0;
  while (currentLine < stepFileLines.size())
  {
    if (stepFileLines[currentLine].find(notesTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "found #NOTES:" << endl)
      
      // identify difficulty
      unsigned int noteLineIndex = currentLine + 1;
      int currentDifficulty = -1;
      while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
      {
        // look for dance-single data
        if (stepFileLines[noteLineIndex].find("dance-single:") != string::npos)
        {
          // difficulty is 2 lines down
          if (stepFileLines[noteLineIndex+2].find("Beginner:") != string::npos)
          {
            currentDifficulty = 0;
          }
          else if (stepFileLines[noteLineIndex+2].find("Easy:") != string::npos)
          {
            currentDifficulty = 1;
          }
          else if (stepFileLines[noteLineIndex+2].find("Medium:") != string::npos)
          {
            currentDifficulty = 2;
          }
          else if (stepFileLines[noteLineIndex+2].find("Hard:") != string::npos)
          {
            currentDifficulty = 3;
          }
          else if (stepFileLines[noteLineIndex+2].find("Challenge:") != string::npos)
          {
            currentDifficulty = 4;
          }
          break;
        }
        ++noteLineIndex;
      }
      
      if (currentDifficulty == -1)
      {
        //error
      }
      else
      {
        LOG(DEBUG_DETAIL, "found dance-single data for difficulty " << currentDifficulty << endl)
        
        noteLineIndex = currentLine + 1;
        
        long currentMeasure = 0;
        difficultyAvailable[currentDifficulty] = true;
        
        // find first line of note data
        noteLineIndex = currentLine + 1;
        while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
        {
          if (stepFileLines[noteLineIndex][0] >= '0' && stepFileLines[noteLineIndex][0] <= '9')
          {
            break;
          }
          noteLineIndex++;
        }  
        // read all note data until terminating ;
        while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
        {
          // start of one measures's worth of data
          int ticksPerMeasure = 0;
          
          // detect how many rows of data are given for this measure
          unsigned int measureLineIndex = noteLineIndex;
          while (measureLineIndex < stepFileLines.size() && stepFileLines[measureLineIndex].find_first_of(";,") == string::npos)
          {
            measureLineIndex++;
          }
          ticksPerMeasure = measureLineIndex - noteLineIndex;
          // if this value is < 4 or > 192 then the data is not standard 
          if (ticksPerMeasure < 4 || ticksPerMeasure > 192)
          {
            LOG(DEBUG_BASIC, "WARN: ignoring data with strange ticksPerMeasure (<4 or >192): " << ticksPerMeasure << " in data block starting at " << StepFilePath().c_str() << ":" << noteLineIndex << endl)
            //skip forward past this data
            noteLineIndex = measureLineIndex;
          }
          else
          {
            LOG(DEBUG_DETAIL, "detected ticks per measure " << ticksPerMeasure << endl)
            
            // process each line of measure data, adding arrows to
            // song data at current difficulty
            long currentBeat = 0;
            float currentBeatFraction = 0.0;
            measureLineIndex = noteLineIndex;
            while (measureLineIndex < stepFileLines.size() && stepFileLines[measureLineIndex].find_first_of(";,") == string::npos)
            {
              currentBeatFraction = (measureLineIndex-noteLineIndex) / (ticksPerMeasure/4.0) - (float)currentBeat;
              if (float_same(currentBeatFraction, 1.0))
              {
                ++currentBeat;
                currentBeatFraction -= 1.0;
              }

              LOG(DEBUG_DETAIL, "tick start. line: " << measureLineIndex << " currentBeat: " << currentMeasure*4.0 + currentBeat + currentBeatFraction << endl)
              
              // detect note type for notes on this line
              Arrow::NoteType noteType = Arrow::NOTE_TYPE_NONE;
              switch(ticksPerMeasure)
              {
                case 4:
                  noteType = Arrow::QUARTER;
                break;
                case 8:
                  if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::QUARTER;
                  else noteType = Arrow::EIGHTH;
                break;
                case 12:
                  if ((measureLineIndex-noteLineIndex) % 3 == 0) noteType = Arrow::QUARTER;
                  else noteType = Arrow::QUARTER_TRIPLET;
                break;
                case 16:
                  if ((measureLineIndex-noteLineIndex) % 4 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::EIGHTH;
                  else noteType = Arrow::SIXTEENTH;
                break;
                case 24:
                  if ((measureLineIndex-noteLineIndex) % 6 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 3 == 0) noteType = Arrow::EIGHTH;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::QUARTER_TRIPLET;
                  else noteType = Arrow::EIGHTH_TRIPLET;
                break;
                case 32:
                  if ((measureLineIndex-noteLineIndex) % 8 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 4 == 0) noteType = Arrow::EIGHTH;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::SIXTEENTH;
                  else noteType = Arrow::THIRTYSECOND;
                break;
                case 48:
                  if ((measureLineIndex-noteLineIndex) % 12 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 6 == 0) noteType = Arrow::EIGHTH;
                  else if ((measureLineIndex-noteLineIndex) % 3 == 0) noteType = Arrow::QUARTER_TRIPLET;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::EIGHTH_TRIPLET;
                  else noteType = Arrow::SIXTEENTH_TRIPLET;
                break;
                case 64:
                  if ((measureLineIndex-noteLineIndex) % 16 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 8 == 0) noteType = Arrow::EIGHTH;
                  else if ((measureLineIndex-noteLineIndex) % 4 == 0) noteType = Arrow::SIXTEENTH;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::THIRTYSECOND;
                  else noteType = Arrow::SIXTYFOURTH;
                break;
                case 96:
                  if ((measureLineIndex-noteLineIndex) % 24 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 12 == 0) noteType = Arrow::EIGHTH;
                  else if ((measureLineIndex-noteLineIndex) % 6 == 0) noteType = Arrow::QUARTER_TRIPLET;
                  else if ((measureLineIndex-noteLineIndex) % 3 == 0) noteType = Arrow::EIGHTH_TRIPLET;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::SIXTEENTH_TRIPLET;
                  else noteType = Arrow::THIRTYSECOND_TRIPLET;
                break;
                case 192:
                  if ((measureLineIndex-noteLineIndex) % 48 == 0) noteType = Arrow::QUARTER;
                  else if ((measureLineIndex-noteLineIndex) % 24 == 0) noteType = Arrow::EIGHTH;
                  else if ((measureLineIndex-noteLineIndex) % 12 == 0) noteType = Arrow::QUARTER_TRIPLET;
                  else if ((measureLineIndex-noteLineIndex) % 6 == 0) noteType = Arrow::EIGHTH_TRIPLET;
                  else if ((measureLineIndex-noteLineIndex) % 3 == 0) noteType = Arrow::SIXTEENTH_TRIPLET;
                  else if ((measureLineIndex-noteLineIndex) % 2 == 0) noteType = Arrow::THIRTYSECOND_TRIPLET;
                  else noteType = Arrow::SIXTYFOURTH_TRIPLET;
                break;
                default:
                  // unhandled data.  fail elegantly
                  break;
              }
              
              LOG(DEBUG_GUTS, "using note type " << noteType << " on tick " << measureLineIndex-noteLineIndex << " at line " << measureLineIndex << endl)
              
              if (noteType != Arrow::NOTE_TYPE_NONE)
              {
                // handle exception that all notes on a line with a hold note
                // are hold notes
                bool holdFound = false;
                
                if (stepFileLines[measureLineIndex].size() < 4)
                {
                  //error
                  LOG(DEBUG_DETAIL, "WARN: note data line too short on line " << measureLineIndex << endl)
                }
                else
                {
                  LOG(DEBUG_DETAIL, "detecting note class and direction on line " << measureLineIndex << endl)
                  
                  if (stepFileLines[measureLineIndex][0] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(0, (currentMeasure*4.0 + currentBeat + currentBeatFraction), Arrow::HOLD));}
                  if (stepFileLines[measureLineIndex][1] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(1, (currentMeasure*4.0 + currentBeat + currentBeatFraction), Arrow::HOLD));}
                  if (stepFileLines[measureLineIndex][2] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(2, (currentMeasure*4.0 + currentBeat + currentBeatFraction), Arrow::HOLD));}
                  if (stepFileLines[measureLineIndex][3] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(3, (currentMeasure*4.0 + currentBeat + currentBeatFraction), Arrow::HOLD));}

                  //NOTE: this behaviour could be configurable.  some users may want notes on the same line as a hold note to be
                  // the standard type and not a zero-length hold.
                  if (stepFileLines[measureLineIndex][0] == '1') arrows[currentDifficulty].push_back(Arrow(0, (currentMeasure*4.0 + currentBeat + currentBeatFraction), holdFound ? Arrow::HOLD : noteType));
                  if (stepFileLines[measureLineIndex][1] == '1') arrows[currentDifficulty].push_back(Arrow(1, (currentMeasure*4.0 + currentBeat + currentBeatFraction), holdFound ? Arrow::HOLD : noteType));
                  if (stepFileLines[measureLineIndex][2] == '1') arrows[currentDifficulty].push_back(Arrow(2, (currentMeasure*4.0 + currentBeat + currentBeatFraction), holdFound ? Arrow::HOLD : noteType));
                  if (stepFileLines[measureLineIndex][3] == '1') arrows[currentDifficulty].push_back(Arrow(3, (currentMeasure*4.0 + currentBeat + currentBeatFraction), holdFound ? Arrow::HOLD : noteType));

                  // find start of hold arrows and set end beat
                  for (int d = 0; d < 4; d++)
                  {
                    if (stepFileLines[measureLineIndex][d] == '3') 
                    {
                      LOG(DEBUG_DETAIL, "searching for start of hold note which ends on line " << measureLineIndex << endl)
                      for (int i = arrows[currentDifficulty].size() - 1; i >= 0; --i)
                      {
                        if (arrows[currentDifficulty][i].direction == d)
                        {
                          LOG(DEBUG_DETAIL, "detected start of hold note at arrow index " << i << ". beatLength=" << (currentMeasure*4.0 + currentBeat + currentBeatFraction) - arrows[currentDifficulty][i].beat << endl)
                          arrows[currentDifficulty][i].beatLength = (currentMeasure*4.0 + currentBeat + currentBeatFraction) - arrows[currentDifficulty][i].beat;
                          break;
                        }
                      }
                    }
                  }
                }
              }
              
              measureLineIndex++;

              LOG(DEBUG_DETAIL, "done processing note line. new arrow count=" << arrows[currentDifficulty].size() << endl)
            }
            
            // end of measure.
            noteLineIndex = measureLineIndex;
            
            // was this the last measure?  if so, continue directly to
            // termination condition evaluation since otherwise the
            // noteLineIndex increment at the end of the loop will
            // put the index past the terminator and screw up detection.
            // else we want the increment because the loop start code
            // expects to be pointing on the first data line, not
            // the separator (,)
            if (stepFileLines[noteLineIndex].find(";") != string::npos)
            {
              LOG(DEBUG_DETAIL, "detected end of last measure at index " << noteLineIndex << endl)
              continue;
            }
          }

          // prepare processing for next measure.
          ++currentMeasure;
          
          ++noteLineIndex;
          LOG(DEBUG_DETAIL, "done processing measure " << currentMeasure << ".  updated note line index to " << noteLineIndex << endl)
        }
        
        // skip past this processed data
        LOG(DEBUG_DETAIL, "current line skipping from " << currentLine << " to " << noteLineIndex << endl)
        currentLine = noteLineIndex;
      }
    }
    
    ++currentLine;
    LOG(DEBUG_DETAIL, "current line incremented to " << currentLine << endl)
  }
  
  // pre-render song using the same calculations as those used during actual
  // render.  increment a millisecond counter since during render the precision
  // is at best to the millisecond.  calculate current time and y position of
  // each arrow as the current beat reaches it
  long lastArrowTime = 0;
  for (int d = 0; d < constants.numDifficulties; ++d)
  {
    long currentMs = beat0Offset - constants.preStartDelay;
    map<float, vector<int> > holdArrowsToFill;
    int currentBpmChange = 0;
    float pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * bpmChanges[0].bpm;
    long viewportOffset = beat0Offset * pixelsPerMsAtCurrentBpm;
    LOG(DEBUG_DETAIL, "song prerender phase, song " << filename << " init vp to " << viewportOffset << endl)  
    
    float pixelsLeftToScroll = 0.0;
    unsigned int baseArrow = 0;
    long currentMeasure = 0;
    int currentBeat = 0;
    float currentBeatFraction = 0.0;
    float partialBeatFraction = 0.0;
    long lastBeatTime = beat0Offset;
    long beatTimeElapsedAtPreviousBPMs = 0;
    float beatFractionAtPreviousBPMs = 0.0;

    LOG(DEBUG_DETAIL, "SONG::looping through " << arrows[d].size() << " arrows " << endl)
    unsigned long startTick = SDL_GetTicks();
    
    LOG(DEBUG_DETAIL, "starting iter at currentMs = " << currentMs << " with pixels per = " << pixelsPerMsAtCurrentBpm << endl)  
    while (baseArrow < arrows[d].size() || holdArrowsToFill.size() > 0)
    {
      long partialFrameTimeBegin = currentMs - 1;
      long frameTimeEnd = currentMs;
      int frameEndBpmChange = currentBpmChange;
      while ( frameEndBpmChange+1 < (int)bpmChanges.size() 
        && bpmChanges[frameEndBpmChange+1].timestamp <= frameTimeEnd)
      {
        LOG(DEBUG_DETAIL, "SONG::inc frameEndBpmChange because next bpm change timestamp "   << bpmChanges[frameEndBpmChange+1].timestamp << " < end of current frame "   << frameTimeEnd << endl)

        ++frameEndBpmChange;

        long partialFrameTimeEnd = bpmChanges[frameEndBpmChange].timestamp;
        
        // process viewport scroll
        float pixelsToScroll = (partialFrameTimeEnd - partialFrameTimeBegin) * pixelsPerMsAtCurrentBpm;
        pixelsToScroll += pixelsLeftToScroll;  
        long wholePixelsToScroll = (long)pixelsToScroll;
        pixelsLeftToScroll = pixelsToScroll - wholePixelsToScroll;
        viewportOffset += wholePixelsToScroll;
        LOG(DEBUG_GUTS, "bpm change vp offset += " << wholePixelsToScroll << "because pfte (" << partialFrameTimeEnd << ") - pftb(" << partialFrameTimeBegin<<") " << endl)  
        
        long timeElapsedAtCurrentBPM = partialFrameTimeEnd - lastBeatTime - beatTimeElapsedAtPreviousBPMs;
        currentBeatFraction = partialBeatFraction + (timeElapsedAtCurrentBPM / Song::MsPerBeatFromBPM(bpmChanges[currentBpmChange].bpm) + beatFractionAtPreviousBPMs);
        LOG(DEBUG_DETAIL, "SONG oh 1  at time " << currentMs << " currentBeatFraction:" << currentBeatFraction << " timeElapsedAtCurrentBPM: " << timeElapsedAtCurrentBPM << endl)
        if ((currentBeatFraction + 0.00001) > 1)
        {
          LOG(DEBUG_DETAIL, "SONG::beat fraction wrap during partial frame" << endl)
          ++currentBeat;
          partialBeatFraction = currentBeatFraction - 1.0;
          currentBeatFraction -= 1.0;
          lastBeatTime = partialFrameTimeEnd;
          beatTimeElapsedAtPreviousBPMs = 0;
          beatFractionAtPreviousBPMs = 0.0;
          if (currentBeat >= 4)
          {
            ++currentMeasure;
            currentBeat = 0;
          }
        }

        beatTimeElapsedAtPreviousBPMs += partialFrameTimeEnd - lastBeatTime - beatTimeElapsedAtPreviousBPMs;
        beatFractionAtPreviousBPMs = currentBeatFraction;

        // update variables needed to calculate next partial
        partialFrameTimeBegin = partialFrameTimeEnd;

        LOG(DEBUG_DETAIL, "SONG::partial frame updated, new vars partialFrameTimeBegin: " << partialFrameTimeBegin << " frameEndBpmChange: " << frameEndBpmChange << endl)

        // optimise pixels per second scroll calculation
        pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * bpmChanges[frameEndBpmChange].bpm;
        LOG(DEBUG_DETAIL, "SONG::A new pixelsPerMsAtCurrentBpm:" << pixelsPerMsAtCurrentBpm << endl)
        
        currentBpmChange = frameEndBpmChange;
      }
      
      // process final partial frame (entire frame in case where no bpm changes
      // occurred during frame)
      // process viewport scroll
      float pixelsToScroll = (frameTimeEnd - partialFrameTimeBegin) * pixelsPerMsAtCurrentBpm;
      // add partial pixels left from last scroll's truncation
      pixelsToScroll += pixelsLeftToScroll;  
      long wholePixelsToScroll = (long)pixelsToScroll;
      pixelsLeftToScroll = pixelsToScroll - wholePixelsToScroll;
      viewportOffset += wholePixelsToScroll;
      LOG(DEBUG_GUTS, "reg vp offset += " << wholePixelsToScroll << "because fte (" << frameTimeEnd << ") - pftb(" << partialFrameTimeBegin<<") " << endl)  
      
      long timeElapsedAtCurrentBPM = frameTimeEnd - lastBeatTime - beatTimeElapsedAtPreviousBPMs;
      currentBeatFraction = partialBeatFraction + (timeElapsedAtCurrentBPM / Song::MsPerBeatFromBPM(bpmChanges[currentBpmChange].bpm) + beatFractionAtPreviousBPMs);
      if ((currentBeatFraction + 0.00001) > 1)
      {
        LOG(DEBUG_DETAIL, "SONG::beat fraction wrap during full frame" << endl)
        ++currentBeat;
        partialBeatFraction = currentBeatFraction - 1.0;
        currentBeatFraction -= 1.0;
        lastBeatTime = frameTimeEnd;
        beatTimeElapsedAtPreviousBPMs = 0;
        beatFractionAtPreviousBPMs = 0.0;
        if (currentBeat >= 4)
        {
          ++currentMeasure;
          currentBeat = 0;
        }
      }

      LOG(DEBUG_GUTS, "SONG:: at time " << currentMs << " viewp: " << viewportOffset << " currentBeat: " << (currentMeasure*4.0 + currentBeat + currentBeatFraction) << " currentBeatFraction:" << currentBeatFraction << " timeElapsedAtCurrentBPM: " << timeElapsedAtCurrentBPM << endl)

      while (baseArrow < arrows[d].size() && arrows[d][baseArrow].beat <= (currentMeasure*4.0 + currentBeat + currentBeatFraction))
      {
        arrows[d][baseArrow].time = currentMs;
        arrows[d][baseArrow].yPos = viewportOffset;
        LOG(DEBUG_DETAIL, "SONG::arrow @ " << baseArrow << ", beat " << arrows[d][baseArrow].beat << " got time = " << arrows[d][baseArrow].time << " yPos = " << arrows[d][baseArrow].yPos << endl)
        if (!float_same(arrows[d][baseArrow].beatLength, 0))
        {
          LOG(DEBUG_DETAIL, "SONG:: putting hold arrow # " << baseArrow << " of length " << arrows[d][baseArrow].beatLength << " into hold arrow fulfillment map at beat " << currentMeasure*4.0 + currentBeat + currentBeatFraction + arrows[d][baseArrow].beatLength << endl)
          holdArrowsToFill[currentMeasure*4.0 + currentBeat + currentBeatFraction + arrows[d][baseArrow].beatLength].push_back(baseArrow);
          LOG(DEBUG_DETAIL, "new hold arrow map size: " << holdArrowsToFill.size() << endl)
        }
        ++baseArrow;
      }
      
      map<float, vector<int> >::iterator it;
      while (   (it = holdArrowsToFill.begin()) != holdArrowsToFill.end() 
              && it->first <= currentMeasure*4.0 + currentBeat + currentBeatFraction
            )
      {
        LOG(DEBUG_DETAIL, "hold arrow map element found when comparing key to currentBeat " << currentMeasure*4.0 + currentBeat + currentBeatFraction << " it's first is " << it->first <<  endl)
        while (it->second.size() > 0)
        {
          arrows[d][it->second.back()].length = viewportOffset - arrows[d][it->second.front()].yPos;
          LOG(DEBUG_DETAIL, "SONG::holdarrow @ " << it->second.back() << ", beat " << arrows[d][it->second.back()].beat << " got length = " << arrows[d][it->second.back()].length << " at current time " << currentMs << endl)
          it->second.pop_back();
        }
        holdArrowsToFill.erase(it);
        LOG(DEBUG_DETAIL, "hold arrow removed from map.  new size: " << holdArrowsToFill.size() << endl)
      }
      
      ++currentMs;
    }
    LOG(DEBUG_BASIC, "SONG:: DONE looping through " << arrows[d].size() << " arrows.  it took " << (SDL_GetTicks() - startTick) / 1000.0 << " seconds " << endl)
    
    if (currentMs > lastArrowTime)
    {
      lastArrowTime = currentMs;
    }
  }
  
  //NOTE: this is not the real length!  only from beat0 to the start of the last arrow,
  // but good enough for the groove radar calculations.  any calculations depending on
  // a real value should get a better value than this.
  length = (lastArrowTime - beat0Offset) / 1000.0;
  LOG(DEBUG_DETAIL, "last arrow time: " << length << endl)
  
  // verify that all groove radar values are filled in.  if not, 
  // calculate values based on step data and write to SM file.
  bool doWrite = false;
  for (int d = 0; d < constants.numDifficulties; ++d)
  {
    bool needRadarValues = false;
    if (difficultyAvailable[d])
    {
      for (unsigned int i = 0; i < radarValues[d].size(); ++i)
      {
        if (radarValues[d][i] < 0)
        {
          needRadarValues = true;
          break;
        }
      }
    }
    if (needRadarValues)
    {
      radarValues[d][Stream] = arrows[d].size() / (float)length / 7.0;
      LOG(DEBUG_DETAIL, "radarValues[" << d << "][Stream] = " << radarValues[d][Stream] << " from formula arrows[d].size(): " << arrows[d].size() << " / length: " << length << " / 7.0 " << endl)
    
      long numJumps = 0;
      long numHolds = 0;
      long numChaotic = 0;
      long lastArrowTime = -1;
      bool inJump = false;
      for (unsigned int j = 0; j < arrows[d].size(); ++j)
      {
        if (arrows[d][j].time != lastArrowTime)
        {
          if (inJump)
          {
            inJump = false;
            ++numJumps;
          }
          lastArrowTime = arrows[d][j].time;
        }
        else
        {
          //two or more arrows with same time
          inJump = true;
        }
        if (arrows[d][j].length > 0) ++numHolds;
        if (arrows[d][j].type > Arrow::EIGHTH) ++numChaotic;
      }
      // count jump on last step
      if (inJump)
      {
        ++numJumps;
      }
      LOG(DEBUG_DETAIL, "counted " << numJumps << " jumps and " << numHolds << " holds and " << numChaotic << " notes smaller than 8th " << endl)
      
      
      radarValues[d][Air] = numJumps / length;
      LOG(DEBUG_DETAIL, "Air = " << (numJumps / length) << endl)
     
      radarValues[d][Freeze] = numHolds / length;
      LOG(DEBUG_DETAIL, "Freeze = " << (numHolds / length) << endl)
      
      radarValues[d][Chaos] = numChaotic / length / 2.0;
      LOG(DEBUG_DETAIL, "Chaos = " << (numChaotic / length / 2.0) << endl)
      
      int windowStartBeat = 0;
      int windowBeatLength = 8;
      unsigned int windowEndArrow = 0;
      unsigned int densestArrowCount = 0;

      for (unsigned int windowStartArrow = 0; windowStartArrow < arrows[d].size(); ++windowStartArrow)
      {
        if (arrows[d][windowStartArrow].beat >= windowStartBeat)
        {
          // find window end arrow
          windowEndArrow = windowStartArrow;
          while(windowEndArrow < arrows[d].size() && arrows[d][windowEndArrow].beat < windowStartBeat + windowBeatLength)
          {
            ++windowEndArrow;
          }
          
          if (densestArrowCount < windowEndArrow - windowStartArrow)
          {
            densestArrowCount = windowEndArrow - windowStartArrow;
          }
          
          ++windowStartBeat;
        }
      }
      
      float maxDensity = densestArrowCount / (float)windowBeatLength;
      LOG(DEBUG_DETAIL, "maxDensity = " << maxDensity << endl)
      float averageBPS = arrows[d][arrows[d].size()-1].beat / length;
      LOG(DEBUG_DETAIL, "averageBPS = " << averageBPS << " (max beat: " << arrows[d][arrows[d].size()-1].beat << " / length: " << length << endl)
      
      radarValues[d][Voltage] = maxDensity * averageBPS / 10.0;
      LOG(DEBUG_DETAIL, "Voltage = " << (maxDensity * averageBPS / 10.0) << endl)
      
      // write updated data to file.
      
      // find relevant #NOTES: section
      string notesTag = "#NOTES:";
      unsigned int currentLine = 0;
      bool done = false;
      while (!done && currentLine < stepFileLines.size())
      {
        if (stepFileLines[currentLine].find(notesTag) != string::npos)
        {
          LOG(DEBUG_DETAIL, "ReadStepData() found #NOTES:" << endl)

          unsigned int noteLineIndex = currentLine + 1;
          bool foundSection = false;
          while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
          {
            // look for dance-single data
            if (stepFileLines[noteLineIndex].find("dance-single:") != string::npos)
            {
              // difficulty is 2 lines down
              if (stepFileLines[noteLineIndex+2].find("Beginner:") != string::npos)
              {
                if (d == 0)
                {
                  foundSection = true;
                  break;
                }
              }
              else if (stepFileLines[noteLineIndex+2].find("Easy:") != string::npos)
              {
                if (d == 1)
                {
                  foundSection = true;
                  break;
                }
              }
              else if (stepFileLines[noteLineIndex+2].find("Medium:") != string::npos)
              {
                if (d == 2)
                {
                  foundSection = true;
                  break;
                }
              }
              else if (stepFileLines[noteLineIndex+2].find("Hard:") != string::npos)
              {
                if (d == 3)
                {
                  foundSection = true;
                  break;
                }
              }
              else if (stepFileLines[noteLineIndex+2].find("Challenge:") != string::npos)
              {
                if (d == 4)
                {
                  foundSection = true;
                  break;
                }
              }
            }
            ++noteLineIndex;
          }
          
          if (foundSection)
          {
            // groove radar values are 4 lines down
            char newGrooveRadarText[100];
            sprintf(newGrooveRadarText, "     %1.3f,%1.3f,%1.3f,%1.3f,%1.3f:", radarValues[d][0], radarValues[d][1], radarValues[d][2], radarValues[d][3], radarValues[d][4]);
            stepFileLines[noteLineIndex+4] = newGrooveRadarText;
            doWrite = true;
            break;
          }
        }
        ++currentLine;
      }
    }
  }
  if (doWrite)
  {
    // write out updated step file data
    ofstream outdata;
    outdata.open((StepFilePath()).c_str());
    currentLine = 0;
    while (currentLine < stepFileLines.size())
    {
      outdata << stepFileLines[currentLine] << endl;
      ++currentLine;
    }
    outdata.close();
  }
  
  LOG(DEBUG_MINOR, "leaving read step data" << endl)
  
  //TODO: these conditions are all true before this function is called.  
  // detect other errors so if this function succeeds, prepared really = true
  prepared = bpmChanges.size() > 0 && (
       difficultyAvailable[0]
    || difficultyAvailable[1]
    || difficultyAvailable[2]
    || difficultyAvailable[3]
    || difficultyAvailable[4]
  );
  
  return prepared;
}


bool Song::ParseBpms(const string& s, vector<BpmChange>& v)
{
  LOG(DEBUG_DETAIL, "parseBpms()" << endl)
  
  v.clear();
    
  //NOTE: sample and definition of SM BPMS tag
  //#BPMS:0=140.215,1=140.208,14=140.215,16=140.208,43=140.215;
  //
  //#BPMS:<float beat>=<float BPM>[,<float beat>=<float BPM>];
  //beat 0 must be defined
  
  size_t startPos = 0;
  size_t endPos = 0;
 
  while ( (startPos = s.find_first_of(":,", startPos)) != string::npos)
  {
    // search for end of first part of bpm token
    ++startPos;
    endPos = s.find_first_of("=", startPos);
    if (endPos != string::npos)
    {
      string bpmBeatText = s.substr(startPos, endPos - startPos);

      // search for end of second part of bpm token
      startPos = endPos + 1;
      endPos = s.find_first_of(",;", startPos);
      if (endPos != string::npos)
      {
        // found complete bpm token
        string bpmFloatText = s.substr(startPos, endPos - startPos);
        
        // add to array of bpm changes
        v.push_back(BpmChange(atof(bpmBeatText.c_str()), 0, atof(bpmFloatText.c_str())));

        LOG(DEBUG_DETAIL, atof(bpmFloatText.c_str()) << " at " << atof(bpmBeatText.c_str()) << endl)
      }
      else
      {
        LOG(DEBUG_BASIC, "ERROR: invalid BPMS data!" << endl)
      }
    }
  }
  
  if (v.size() == 0)
  {
    LOG(DEBUG_BASIC, "ERROR: failed to extract BPMS!" << endl)
    return false;
  }

  // check for initial bpm
  if (!float_same(v.front().beat, 0))
  {
    LOG(DEBUG_BASIC, "ERROR: did not find BPM for beat 0.  first bpm change on beat " << v.front().beat << endl)
    return false;
  }

  // calculate timestamps
  float tempTime = beat0Offset;
  v[0].timestamp = tempTime;
  LOG(DEBUG_DETAIL, "new tempTime " << tempTime << " going to timestamp for bpmChange at i=0 " << endl)
  for (unsigned int i = 0; i < v.size(); ++i)
  {
    if ((i+1) < v.size())
    {
      tempTime += MsPerBeatFromBPM(v[i].bpm) * (v[i+1].beat - v[i].beat);
      LOG(DEBUG_DETAIL, "new tempTime " << tempTime << " going to timestamp for bpmChange at i=" << i+1 << " calc # beats: " << (v[i+1].beat - v[i].beat) << endl)
      v[i+1].timestamp = tempTime;
    }
  }

  LOG(DEBUG_DETAIL, "parseBpms() finished.  extracted " << v.size() << " bpm changes" << endl)
  return v.size() > 0;
}

string Song::SMValGetString(const string& s)
{
  LOG(DEBUG_DETAIL, "SMValGetString(" << s << ")" << endl)
  
  string val = "";
    
  size_t startPos = 0;
  size_t endPos = 0;
 
  startPos = s.find(":");
  if (startPos != string::npos)
  {
    ++startPos;
    // search for end of token
    endPos = s.find(";", startPos);
    if (endPos != string::npos)
    {
      // found complete offset token
      val = s.substr(startPos, endPos-startPos);
      LOG(DEBUG_DETAIL, "SMValGetString extracted text " << val << endl)
    }      
    else
    {
      LOG(DEBUG_BASIC, "ERROR: SMValGetString didn't find token terminator! (;)" << endl)
    }
  }
  else
  {
    LOG(DEBUG_BASIC, "ERROR: SMValGetString didn't find token label terminator! (:)" << endl)
  }

  return val;
}

bool Song::DifficultyIsAvailable(int difficulty)
{
  LOG(DEBUG_DETAIL, "Game::DifficultyIsAvailable " << difficulty << endl)
  
  if (difficulty < 0 || difficulty >= constants.numDifficulties) return false;
  LOG(DEBUG_DETAIL, "diff returning " << difficultyAvailable[difficulty] << endl)

  return difficultyAvailable[difficulty];
}

float Song::MsPerBeatFromBPM(float bpm)
{
  LOG(DEBUG_GUTS, "new MsPerBeatFromBPM(" << bpm << " returning  " << 60000.0/bpm << endl)
  return 60000.0/bpm;
}


}
