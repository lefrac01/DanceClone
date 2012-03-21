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

BeatTick::BeatTick(long y, float t, float b) : 
  yPos(y),
  timestamp(t),
  beat(b)
{
}

Song::Song() :
  loaded(false),
  filename(""),
  beat0Offset(0),
  arrows(constants.numDifficulties),
  difficultyAvailable(constants.numDifficulties, false)
{
}

void Song::Init()
{
  LOG(DEBUG_MINOR, "DanceClone::Song::Init()" << endl)
  filename = "";
  bannerImageFilename = "";
  backgroundImageFilename = "";
  
  std::fill(difficultyAvailable.begin(), difficultyAvailable.end(), false);
  
  for (int i = 0; i < constants.numDifficulties; ++i)
  {
    arrows[i].clear();
  }
  
  bpmChanges.clear();
  beatTicks.clear();
  
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

  if (ReadStepData())
  {
    loaded = true;
  }
  else
  {
    LOG(DEBUG_MINOR, "Song::Load() failed to detect valid step data for current mode" << endl)
    loaded = false;
  }
  return loaded;
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

string Song::StepFilePath()
{
  string temp = Path();
  temp = temp.substr(0, temp.size()-4);
  temp += ".sm";
  return temp;
}

string Song::ScorePath()
{
  string temp = Path();
  temp = temp.substr(0, temp.size()-4);
  temp += ".score"; 
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

bool Song::ReadStepData()
{
  LOG(DEBUG_BASIC, "Song::ReadStepData()" << endl)

  // read file
  LOG(DEBUG_MINOR, "indata.open(" << StepFilePath() << ")" << endl)

  ifstream indata;
  indata.open(StepFilePath().c_str());
  string fileLine;
  while (getline(indata, fileLine)) stepFileLines.push_back(fileLine);
  indata.close();
  
  LOG(DEBUG_DETAIL, "got file contents of " << stepFileLines.size() << " lines. " << endl)
  
  // analyse contents
  string bannerTag = "#BANNER:";
  string backgoundTag = "#BACKGROUND:";
  string bpmTag = "#BPMS:";
  string offsetTag = "#OFFSET:";
  string notesTag = "#NOTES:";
  unsigned int currentLine = 0;
  while (currentLine < stepFileLines.size())
  {
    if (stepFileLines[currentLine].find(bannerTag) != string::npos)
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
      ParseBpms(stepFileLines[currentLine], bpmChanges);
    }
    else if (stepFileLines[currentLine].find(offsetTag) != string::npos)
    {
      beat0Offset = -1000.0 * atof(SMValGetString(stepFileLines[currentLine]).c_str());
      LOG(DEBUG_DETAIL, "ReadStepData() found " << offsetTag << beat0Offset << endl)
    }
    else if (stepFileLines[currentLine].find(notesTag) != string::npos)
    {
      LOG(DEBUG_DETAIL, "ReadStepData() found #NOTES:" << endl)
      
      if (bpmChanges.size() == 0)
      {
        //error unable to process note data without BPM info!
        LOG(DEBUG_BASIC, "ERROR: ReadStepData() no bpmChanges " << endl)
      }
      else
      {
        // identify difficulty
        unsigned int noteLineIndex = currentLine + 1;
        int currentDifficulty = -1;
        while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
        {
          if (stepFileLines[noteLineIndex].find("Beginner:") != string::npos)
          {
            currentDifficulty = 0;
            break;
          }
          else if (stepFileLines[noteLineIndex].find("Easy:") != string::npos)
          {
            currentDifficulty = 1;
            break;
          }
          else if (stepFileLines[noteLineIndex].find("Medium:") != string::npos)
          {
            currentDifficulty = 2;
            break;
          }
          else if (stepFileLines[noteLineIndex].find("Hard:") != string::npos)
          {
            currentDifficulty = 3;
            break;
          }
          else if (stepFileLines[noteLineIndex].find("Challenge:") != string::npos)
          {
            currentDifficulty = 4;
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
          LOG(DEBUG_DETAIL, "ReadStepData() found difficulty " << currentDifficulty << endl)
          
          // confirm mode is dance-single
          bool isDanceSingle = false;
          noteLineIndex = currentLine + 1;
          while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
          {
            if (stepFileLines[noteLineIndex].find("dance-single:") != string::npos)
            {
              isDanceSingle = true;
              break;
            }
            ++noteLineIndex;
          }
          if (!isDanceSingle)
          {
            LOG(DEBUG_BASIC, "ReadStepData() skipping note data not for dance-single for difficulty " << currentDifficulty << endl)
          }
          else
          {
            LOG(DEBUG_DETAIL, "ReadStepData() found valid dance-single note data for difficulty" << currentDifficulty << endl)
            difficultyAvailable[currentDifficulty] = true;
            // read note data
          }            
        }
      }
    }
    
    ++currentLine;
    LOG(DEBUG_GUTS, "ReadStepData() current line incremented to " << currentLine << endl)
  }

  LOG(DEBUG_MINOR, "ReadStepData() done" << endl)
  
  return bpmChanges.size() > 0 && (
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
        if (stepFileLines[noteLineIndex].find("Beginner:") != string::npos)
        {
          currentDifficulty = 0;
          break;
        }
        else if (stepFileLines[noteLineIndex].find("Easy:") != string::npos)
        {
          currentDifficulty = 1;
          break;
        }
        else if (stepFileLines[noteLineIndex].find("Medium:") != string::npos)
        {
          currentDifficulty = 2;
          break;
        }
        else if (stepFileLines[noteLineIndex].find("Hard:") != string::npos)
        {
          currentDifficulty = 3;
          break;
        }
        else if (stepFileLines[noteLineIndex].find("Challenge:") != string::npos)
        {
          currentDifficulty = 4;
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
        LOG(DEBUG_DETAIL, "found difficulty " << currentDifficulty << endl)
        
        // confirm mode is dance-single
        bool isDanceSingle = false;
        noteLineIndex = currentLine + 1;
        while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
        {
          if (stepFileLines[noteLineIndex].find("dance-single:") != string::npos)
          {
            isDanceSingle = true;
            break;
          }
          ++noteLineIndex;
        }
        if (!isDanceSingle)
        {
          LOG(DEBUG_BASIC, "ERROR: no steps found for dance-single" << endl)
        }
        else
        {
          LOG(DEBUG_DETAIL, "confirmed data is for dance-single" << endl)
          
          // found valid note data.  
          float currentTime = beat0Offset;
          // a check is already done to be sure there is at least one bpm change...
          float currentPosition = beat0Offset * constants.pixelsPerMsAt1Bpm * bpmChanges.front().bpm;
          LOG(DEBUG_DETAIL, "starting with currentTime " << currentTime << endl)

          int currentBpmIndex = -1; // reused variable name... ick
          float currentBeat = 0;
          float current16thBeat = 0;

          // calculate beat ticks preceding beat 0 back to the beginning of the song minus sound sync offset
          // this is needed for the arrow animation to work before the first beat
          float one16thOffset = 240000 / bpmChanges[0].bpm / 16 / 4;
          float temp16thTime = currentTime;
          float temp16thPosition = currentPosition;

          // store beat ticks.  these serve several purposes and I'm getting foggy as to what they are
          // one has to exist for each note I think, and I'm adding a requirement for all 16th note ticks
          // to be present as well.  they are used to translate beat to timestamp and vice versa
          
          // NOTE: simple insertion into vector would be too inefficient.  C++ STL vector insertion moves
          // all elements after insertion point so it is much better to create a temp vector using push_back
          // and then flip the contents into the real working variable
          vector<BeatTick> initialBeatTicks;
          while (temp16thTime > 0 - constants.songStartOffset)
          {
            temp16thTime -= one16thOffset;
            temp16thPosition -= one16thOffset * constants.pixelsPerMsAt1Bpm * bpmChanges[0].bpm;
            current16thBeat -= 1.0 / 16.0;  //TODO: optimise out calculation
            LOG(DEBUG_GUTS, "beatTicks+=(" << (long)temp16thPosition << ", " <<  temp16thTime << ", " <<  current16thBeat << ")" << endl)
            initialBeatTicks.push_back(BeatTick((long)temp16thPosition, temp16thTime, current16thBeat));
          }
          for (int l = initialBeatTicks.size()-1; l >= 0; l--)
          {
            beatTicks.push_back(BeatTick(initialBeatTicks[l].yPos, initialBeatTicks[l].timestamp, initialBeatTicks[l].beat));
          }
          initialBeatTicks.clear();
          current16thBeat = 0;

          difficultyAvailable[currentDifficulty] = true;
          
          // read note data
          
          //TODO: confirm (?) it seems like note meta-data all starts with whitespace while note data,
          // beat dividers and end marker start on character 0
          noteLineIndex = currentLine + 1;
          while (noteLineIndex < stepFileLines.size() && stepFileLines[noteLineIndex].find(";") == string::npos)
          {
            if (stepFileLines[noteLineIndex][0] >= '0' && stepFileLines[noteLineIndex][0] <= '9')
            {
              // found first line of note data
              break;
            }
            noteLineIndex++;
          }  
          if (stepFileLines[noteLineIndex].find(";") != string::npos)
          {
            LOG(DEBUG_BASIC, "ERROR: didn't find terminating ; while parsing notes starting at line " << noteLineIndex << endl)
          }
          else
          {
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
                LOG(DEBUG_BASIC, "WARN: strange ticksPerMeasure (<4 or >192): " << ticksPerMeasure << " in data block starting at " << StepFilePath().c_str() << ":" << noteLineIndex << endl)
                //skip forward past this data
                noteLineIndex = measureLineIndex;
              }
              else
              {
                LOG(DEBUG_DETAIL, "detected ticks per measure " << ticksPerMeasure << endl)
                
                // process each line of measure data, adding arrows to
                // song data at current difficulty
                measureLineIndex = noteLineIndex;
                while (measureLineIndex < stepFileLines.size() && stepFileLines[measureLineIndex].find_first_of(";,") == string::npos)
                {
                  LOG(DEBUG_DETAIL, "beat tick start. currentBeat: " << currentBeat << " currentTime: " << currentTime << " currentPosition: " << currentPosition << " line: " << noteLineIndex << endl)
                  
                  // set BPM according to current beat and list of BPMs that apply to each beat
                  while (currentBpmIndex + 1 < (int)bpmChanges.size()
                    &&  ((bpmChanges[currentBpmIndex + 1].beat < currentBeat) || (float_same(bpmChanges[currentBpmIndex + 1].beat,currentBeat)))
                  )
                  {
                    // there is a BPM change on this beat.
                    currentBpmIndex++;
                    
                    // now that timestamp is known, update it
                    //NOTE: this works but it weakens the code
                    bpmChanges[currentBpmIndex].timestamp = currentTime;
                    //#LOG(DEBUG_BASIC, "TIMESHTAMPING:  currentBpmIndex:" << currentBpmIndex << " currentTime: " << currentTime << " currentBeat " << currentBeat << endl)
                    
                    LOG(DEBUG_DETAIL, "beat change detected on beat " << currentBeat << " new bpm is " << bpmChanges[currentBpmIndex].bpm << " at index " << currentBpmIndex << endl << "timestamped bpmChange with " << currentTime << endl)
                  }
                  
                  // detect note type
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
                  
                  LOG(DEBUG_DETAIL, "detected note type " << noteType << " on tick " << measureLineIndex-noteLineIndex << " at line " << measureLineIndex << endl)
                  
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
                      
                      if (stepFileLines[measureLineIndex][0] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(0, (long)currentTime, (long)currentPosition, 0, Arrow::HOLD));}
                      if (stepFileLines[measureLineIndex][1] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(1, (long)currentTime, (long)currentPosition, 0, Arrow::HOLD));}
                      if (stepFileLines[measureLineIndex][2] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(2, (long)currentTime, (long)currentPosition, 0, Arrow::HOLD));}
                      if (stepFileLines[measureLineIndex][3] == '2') {holdFound = true; arrows[currentDifficulty].push_back(Arrow(3, (long)currentTime, (long)currentPosition, 0, Arrow::HOLD));}
                      
                      if (stepFileLines[measureLineIndex][0] == '1') arrows[currentDifficulty].push_back(Arrow(0, (long)currentTime, (long)currentPosition, 0, holdFound ? Arrow::HOLD : noteType));
                      if (stepFileLines[measureLineIndex][1] == '1') arrows[currentDifficulty].push_back(Arrow(1, (long)currentTime, (long)currentPosition, 0, holdFound ? Arrow::HOLD : noteType));
                      if (stepFileLines[measureLineIndex][2] == '1') arrows[currentDifficulty].push_back(Arrow(2, (long)currentTime, (long)currentPosition, 0, holdFound ? Arrow::HOLD : noteType));
                      if (stepFileLines[measureLineIndex][3] == '1') arrows[currentDifficulty].push_back(Arrow(3, (long)currentTime, (long)currentPosition, 0, holdFound ? Arrow::HOLD : noteType));

                      // find start of hold arrows and calculate length
                      for (int d = 0; d < 4; d++)
                      {
                        if (stepFileLines[measureLineIndex][d] == '3') 
                        {
                          LOG(DEBUG_DETAIL, "searching for start of hold note which ends on line " << measureLineIndex << endl)
                          for (int i = arrows[currentDifficulty].size() - 1; i >= 0; --i)
                          {
                            if (arrows[currentDifficulty][i].direction == d)
                            {
                              LOG(DEBUG_DETAIL, "detected start of hold note at arrow index " << i << ". length=" << (long)currentPosition - (long)arrows[currentDifficulty][i].yPos << endl)
                              arrows[currentDifficulty][i].length = (long)currentPosition - (long)arrows[currentDifficulty][i].yPos;
                              break;
                            }
                          }
                        }
                      }
                    }
                  }
                  
                  float temp16thTime = currentTime;
                  float temp16thPosition = currentPosition;
                  
                  // ms per measure = (1000*60/bpm)*4
                  // ms per tick = ms per measure / ticksPerMeasure
                  float time_offset = 240000 / bpmChanges[currentBpmIndex].bpm / ticksPerMeasure;
                  float one16thOffset = 240000 / bpmChanges[currentBpmIndex].bpm / 16 / 4;
                  
                  currentTime += time_offset;

                  currentPosition += time_offset * constants.pixelsPerMsAt1Bpm * bpmChanges[currentBpmIndex].bpm;
                  // same as
                  //currentPosition += 240 / ticksPerMeasure;  
                  // but only if screen height = 480
                  
                  // but why?  is there some other factor that is needed?
                  // surely, since in DDR as BPM goes up, space between arrows increases!

                  measureLineIndex++;
                  
                  currentBeat += 1.0/(ticksPerMeasure/4);  //ho! ticks per measure is 4x ticks per beat.

                  // compensate for float imprecision
                  if (fabs(currentBeat - (long)currentBeat) <= 0.01)
                  {
                    // reset float on integral beats
                    currentBeat = (long)currentBeat;
                  }

                  // store beat ticks.  at least 16th note ticks must be present.
                  // used to translate beat to timestamp and vice versa for animation code.
                  while (current16thBeat < currentBeat)
                  {
                    beatTicks.push_back(BeatTick((long)temp16thPosition, temp16thTime, current16thBeat));
                    temp16thTime += one16thOffset;
                    temp16thPosition += one16thOffset * constants.pixelsPerMsAt1Bpm * bpmChanges[currentBpmIndex].bpm;
                    current16thBeat += 1.0/16.0;  //TODO: optimise out calculation
                  }
                  
                  // for measures where the ticks per measure are not aligned with 16th (ie 12, 24)
                  // or there are more than 16 ticks per measure
                  // float comparison...
                  if (!float_same(currentBeat, current16thBeat))
                  //if (currentBeat != current16thBeat)    // should be possible using -mepsilon command-line parameter but that gives me an error :(
                  {
                    beatTicks.push_back(BeatTick((long)currentPosition, currentTime, currentBeat));
                  }
                  
                  LOG(DEBUG_DETAIL, "done processing beat line. new arrow count=" << arrows[currentDifficulty].size() << endl)
                }
                
                // found end of measure.  continue note processing from here
                noteLineIndex = measureLineIndex;
                LOG(DEBUG_DETAIL, "done processing beat block.  updated note line index " << noteLineIndex << endl)
                
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

              // the measure has been processed.
              
              ++noteLineIndex;
              LOG(DEBUG_DETAIL, "done processing measure.  currentBeat: " << currentBeat << " noteLineIndex: " << noteLineIndex << endl)
            }
            
            // skip past this processed data
            LOG(DEBUG_DETAIL, "current line skipping from " << currentLine << " to " << noteLineIndex << endl)
            currentLine = noteLineIndex;
          }
        }
      }
    }
    
    ++currentLine;
    LOG(DEBUG_DETAIL, "current line incremented to " << currentLine << endl)
  }
  
  int num_beatTicks = beatTicks.size();
  for (int i = 0; i < num_beatTicks; i++)
  {
    LOG(DEBUG_DETAIL, "b_ts["<<i<<"]="<<beatTicks[i].yPos<<","<<beatTicks[i].timestamp<<","<<beatTicks[i].beat<<endl)
  }
  
  //KLUDGE:
  // beat ticks are only inserted into the vector when reading note data. 
  // if the note data does not extend to the end of the song, it is 
  // possible that a BPM change will happen on a beat that is not
  // covered by the beat tick vector.  bugs frame time calcs.
  if (beatTicks.size() > 0 && bpmChanges.size() > 0)
  {
    float lastBeatTickBeat = beatTicks.back().beat;
    while(bpmChanges.back().beat > lastBeatTickBeat)
    {
      LOG(DEBUG_BASIC, "WARN: popping extra bpm change!" << endl)
      bpmChanges.pop_back();
    }
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
  //#BPMS:<int beat>=<float BPM>[,<int beat>=<float BPM>];
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
        //NOTE: timestamp will be added later while parsing note data
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

}
