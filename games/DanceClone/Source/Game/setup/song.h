//NOTE: stepmania file format at http://www.stepmania.com/wiki/The_.SM_file_format
//
//TODO: implement #STOPS:
//
//TODO: think.  if editing steps is to be allowed, the internal data
// must retain beat info.  easy enough to add that in at the load phase...
// think ahead to the save phase which is on the wishlist

#ifndef SONG_H

#include <string>
using std::string;
#include <vector>
using std::vector;

class arrow
{
public:
  int direction;
  long time;
  long ypos;
  int length;
  int type; // for drawing based on note value
  arrow(int d, long t, long y, int l, int ty) {direction = d; time = t; ypos = y; length = l; type = ty;};
};

class bpm_change
{
public:
  float beat;
  float timestamp;
  float bpm;
  bpm_change(float b, float t, float bp) {beat = b; timestamp = t; bpm = bp;};
};

class beat_tick
{
public:
  long  ypos;
  float timestamp;
  float beat; // easier considering ticks per measure can vary
  beat_tick(long y, float t, float o) {ypos = y; timestamp = t; beat = o;};
};

class song
{
private:
  bool read_step_data();
  vector<bpm_change> parse_bpms(const string& s); // a) don't care about the copy  b) this ideally belongs elsewhere
  long parse_offset(const string& s); // same as above
  
public:
  bool initialised;
  string filename;
  long beat_0_offset;
  vector< vector<arrow> > song_arrows;
  vector<bpm_change> bpm_changes;
  vector<bool> difficulty_available;
  vector<beat_tick> beat_ticks;

  song();
  void init();
  bool load(string f);
  string path();
  string name();
  string step_file_path();
  string score_path();
  bool difficulty_is_available(int difficulty);
};

song::song() :
initialised(false),
filename(""),
beat_0_offset(0),
song_arrows (NUM_DIFFICULTIES),
difficulty_available (NUM_DIFFICULTIES, false)
{
}

void song::init()
{
  // init song data
  filename = "";
  std::fill(difficulty_available.begin(), difficulty_available.end(), false);
  
  for (int i=0; i<NUM_DIFFICULTIES; i++)
  {
    song_arrows[i].clear();
  }
  
  bpm_changes.clear();
  beat_ticks.clear();
  initialised = false;
}

bool song::load(string f)
{
  if (initialised)
  {
    init();
  }
  
  // prepare song data
  filename = f;
  if (read_step_data())
  {
    initialised = true;
  }
  else
  {
    initialised = false;
  }
  return initialised;
}

string song::path()
{
  string temp = "Music/";
  temp += filename;
  return temp;
}

string song::name()
{
  string temp = filename.substr(0, filename.size()-4);
  return temp;
}

string song::step_file_path()
{
  string temp = path();
  temp = temp.substr(0, temp.size()-4);
  temp += ".sm";  
  return temp;
}

string song::score_path()
{
  string temp = path();
  temp = temp.substr(0, temp.size()-4);
  temp += ".score";  
  return temp;
}

bool song::read_step_data()
{
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "beginning read step data" << endl;
    debug_log.close();
  }


  // read file
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "indata.open(" << step_file_path() << ")" << endl;
    debug_log.close();
  }
  ifstream indata;
  indata.open(step_file_path().c_str());
  string file_line;
  vector<string> file_lines;
  while (getline(indata, file_line)) file_lines.push_back(file_line);
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "got file contents of " << file_lines.size() << " lines. " << endl;
    debug_log.close();
  }  
  
  
  // analyse contents
  string bpm_tag = "#BPMS:";
  string offset_tag = "#OFFSET:";
  string notes_tag = "#NOTES:";
  unsigned int current_line = 0;
  while (current_line < file_lines.size())
  {
    if (file_lines[current_line].find(bpm_tag) != string::npos)
    {
      // found bpm data
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found #BPMS:" << endl;
        debug_log.close();
      }
      bpm_changes = parse_bpms(file_lines[current_line]);
    }
    else if (file_lines[current_line].find(offset_tag) != string::npos)
    {
      // found offset data
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found #OFFSET:" << endl;
        debug_log.close();
      }
      beat_0_offset = parse_offset(file_lines[current_line]);
    }
    else if (file_lines[current_line].find(notes_tag) != string::npos)
    {
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found #NOTES:" << endl;
        debug_log.close();
      }
      
      // found note data
      if (bpm_changes.size() == 0)
      {
        //error unable to process note data without BPM info!
        debug_log.open("debug", std::ios_base::app);
        debug_log << "ERROR: no bpm_changes " << endl;
        debug_log.close();
      }
      else
      {
        // identify difficulty
        unsigned int note_line_index = current_line+1;
        int current_difficulty = -1;
        while (note_line_index < file_lines.size() && file_lines[note_line_index].find(";") == string::npos)
        {
          if (file_lines[note_line_index].find("Beginner:") != string::npos)
          {
            current_difficulty = 0;
            break;
          }
          else if (file_lines[note_line_index].find("Easy:") != string::npos)
          {
            current_difficulty = 1;
            break;
          }
          else if (file_lines[note_line_index].find("Medium:") != string::npos)
          {
            current_difficulty = 2;
            break;
          }
          else if (file_lines[note_line_index].find("Hard:") != string::npos)
          {
            current_difficulty = 3;
            break;
          }
          else if (file_lines[note_line_index].find("Challenge:") != string::npos)
          {
            current_difficulty = 4;
            break;
          }
          note_line_index++;
        }
        
        if (current_difficulty == -1)
        {
          //error
        }
        else
        {
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "found difficulty " << current_difficulty << endl;
            debug_log.close();
          }
          
          // confirm mode is dance-single
          bool dance_single = false;
          note_line_index = current_line+1;
          while (note_line_index < file_lines.size() && file_lines[note_line_index].find(";") == string::npos)
          {
            if (file_lines[note_line_index].find("dance-single:") != string::npos)
            {
              dance_single = true;
              break;
            }
            note_line_index++;
          }
          if (!dance_single)
          {
            //error
          }
          else
          {
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              debug_log.open("debug", std::ios_base::app);
              debug_log << "confirmed data is for dance-single" << endl;
              debug_log.close();
            }          
            
            // found valid note data.  
            float current_time = beat_0_offset;
            // a check is already done to be sure there is at least one bpm change...
            float current_position = beat_0_offset * pixels_per_ms_at_1_bpm * bpm_changes.front().bpm;
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              debug_log.open("debug", std::ios_base::app);
              debug_log << "starting with current_time " << current_time << endl;
              debug_log.close();
            }
            int current_bpm_index = -1; // reused variable name... ick
            float current_beat = 0;
        
            //TODO: move this down to also detect errors like lack of BPM
            difficulty_available[current_difficulty] = true;
            
            // read note data
            
            //TODO: confirm (?) it seems like note meta-data all starts with whitespace while note data,
            // beat dividers and end marker start on character 0
            note_line_index = current_line+1;
            while (note_line_index < file_lines.size() && file_lines[note_line_index].find(";") == string::npos)
            {
              if (file_lines[note_line_index][0] >= '0' && file_lines[note_line_index][0] <= '9')
              {
                // found first line of note data
                break;
              }
              note_line_index++;
            }  
            if (file_lines[note_line_index].find(";") != string::npos)
            {
              //error
            }
            else
            {
              // read all note data until terminating ;
              while (note_line_index < file_lines.size() && file_lines[note_line_index].find(";") == string::npos)
              {
                // start of one measures's worth of data
                int ticks_per_measure = 0;
                
                // detect how many rows of data are given for this measure
                unsigned int measure_line_index = note_line_index;
                while (measure_line_index < file_lines.size() && file_lines[measure_line_index].find_first_of(";,") == string::npos)
                {
                  measure_line_index++;
                }
                ticks_per_measure = measure_line_index - note_line_index;
                // if this value is < 4 or > 48 then the file is not standard SM 5
                if (ticks_per_measure < 4 || ticks_per_measure > 48)
                {
                  //error
                  debug_log.open("debug", std::ios_base::app);
                  debug_log << "WARN: non-standard ticks_per_measure: " << ticks_per_measure << " in data block starting at " << note_line_index << endl;
                  debug_log.close();
                }
                else
                {
                  if (DEBUG_LEVEL >= DEBUG_DETAIL)
                  {
                    debug_log.open("debug", std::ios_base::app);
                    debug_log << "detected ticks per measure " << ticks_per_measure << endl;
                    debug_log.close();
                  }

                  
                  // process each line of measure data, adding arrows to
                  // song data at current difficulty
                  measure_line_index = note_line_index;
                  while (measure_line_index < file_lines.size() && file_lines[measure_line_index].find_first_of(";,") == string::npos)
                  {
                    if (DEBUG_LEVEL >= DEBUG_DETAIL)
                    {
                      debug_log.open("debug", std::ios_base::app);
                      debug_log << "beat tick start. current_beat: " << current_beat << " current_time: " << current_time << " current_position: " << current_position << " line: " << note_line_index << endl;
                      debug_log.close();
                    }
                    beat_ticks.push_back(beat_tick((long)current_position, current_time, current_beat));
                    
                    // set BPM according to current beat and list of BPMs that apply to each beat
                    while (current_bpm_index+1 < (int)bpm_changes.size()
                      &&  (bpm_changes[current_bpm_index+1].beat <= current_beat)
                    )
                    {
                      // there is a BPM change on this beat.
                      current_bpm_index++;
                      
                      // now that timestamp is known, update it
                      //NOTE: this works but it weakens the code
                      bpm_changes[current_bpm_index].timestamp = current_time;
                      
                      if (DEBUG_LEVEL >= DEBUG_DETAIL)
                      {
                        debug_log.open("debug", std::ios_base::app);
                        debug_log << "beat change detected on beat " << current_beat << " new bpm is " << bpm_changes[current_bpm_index].bpm << " at index " << current_bpm_index << endl;
                        debug_log << "timestamped bpm_change with " << current_time << endl;
                        debug_log.close();
                      }
                    }
                    
                    // detect note type
                    // beat_line_index is current tick
                    // mod, div, love, luck! time && effort == goodness
                    int note_type = -1;
                    switch(ticks_per_measure)
                    {
                      case 4:
                        note_type = NOTE_TYPE_QUARTER;
                      break;
                      case 8:
                        if ((measure_line_index-note_line_index) % 2 == 0) note_type = NOTE_TYPE_QUARTER;
                        else note_type = NOTE_TYPE_EIGHTH;
                      break;
                      case 12:
                        if ((measure_line_index-note_line_index) % 3 == 0) note_type = NOTE_TYPE_QUARTER;
                        else note_type = NOTE_TYPE_TWELFTH;
                      break;
                      case 16:
                        if ((measure_line_index-note_line_index) % 4 == 0) note_type = NOTE_TYPE_QUARTER;
                        else if ((measure_line_index-note_line_index) % 2 == 0) note_type = NOTE_TYPE_EIGHTH;
                        else note_type = NOTE_TYPE_SIXTEENTH;
                      break;
                      default:
                        // unhandled data.  fail elegantly
                        break;
                    }
                    
                    if (DEBUG_LEVEL >= DEBUG_DETAIL)
                    {
                      debug_log.open("debug", std::ios_base::app);
                      debug_log << "detected note type " << note_type << " on tick " << measure_line_index-note_line_index << " at line " << measure_line_index << endl;
                      debug_log.close();
                    }
                    
                    if (note_type != -1)
                    {
                      // handle exception that all notes on a line with a hold note
                      // are hold notes
                      bool hold_found = false;
                      
                      if (file_lines[measure_line_index].size() < 4)
                      {
                        //error
                      }
                      else
                      {
                        if (DEBUG_LEVEL >= DEBUG_DETAIL)
                        {
                          debug_log.open("debug", std::ios_base::app);
                          debug_log << "detecting note class and direction on line " << measure_line_index << endl;
                          debug_log.close();
                        }
                        
                        
                        if (file_lines[measure_line_index][0] == '2') {hold_found = true; song_arrows[current_difficulty].push_back(arrow(0, (long)current_time, (long)current_position, 0, NOTE_TYPE_HOLD));}
                        if (file_lines[measure_line_index][1] == '2') {hold_found = true; song_arrows[current_difficulty].push_back(arrow(1, (long)current_time, (long)current_position, 0, NOTE_TYPE_HOLD));}
                        if (file_lines[measure_line_index][2] == '2') {hold_found = true; song_arrows[current_difficulty].push_back(arrow(2, (long)current_time, (long)current_position, 0, NOTE_TYPE_HOLD));}
                        if (file_lines[measure_line_index][3] == '2') {hold_found = true; song_arrows[current_difficulty].push_back(arrow(3, (long)current_time, (long)current_position, 0, NOTE_TYPE_HOLD));}
                        
                        if (file_lines[measure_line_index][0] == '1') song_arrows[current_difficulty].push_back(arrow(0, (long)current_time, (long)current_position, 0, hold_found ? NOTE_TYPE_HOLD : note_type));
                        if (file_lines[measure_line_index][1] == '1') song_arrows[current_difficulty].push_back(arrow(1, (long)current_time, (long)current_position, 0, hold_found ? NOTE_TYPE_HOLD : note_type));
                        if (file_lines[measure_line_index][2] == '1') song_arrows[current_difficulty].push_back(arrow(2, (long)current_time, (long)current_position, 0, hold_found ? NOTE_TYPE_HOLD : note_type));
                        if (file_lines[measure_line_index][3] == '1') song_arrows[current_difficulty].push_back(arrow(3, (long)current_time, (long)current_position, 0, hold_found ? NOTE_TYPE_HOLD : note_type));

                        // find start of hold arrows and calculate length
                        for (int d = 0; d < 4; d++)
                        {
                          if (file_lines[measure_line_index][d] == '3') 
                          {
                            if (DEBUG_LEVEL >= DEBUG_DETAIL)
                            {
                              debug_log.open("debug", std::ios_base::app);
                              debug_log << "searching for start of hold note which ends on line " << measure_line_index << endl;
                              debug_log.close();
                            }
                            for (int i = song_arrows[current_difficulty].size()-1; i > 0; i--)
                            {
                              if (song_arrows[current_difficulty][i].direction == d)
                              {
                                if (DEBUG_LEVEL >= DEBUG_DETAIL)
                                {
                                  debug_log.open("debug", std::ios_base::app);
                                  debug_log << "detected start of hold note at arrow index " << i << endl;
                                  debug_log.close();
                                }
                                song_arrows[current_difficulty][i].length = (long)current_time - song_arrows[current_difficulty][i].time;
                                break;
                              }
                            }
                          }
                        }
                      }
                    }
                    
                    // ms per measure = (1000*60/bpm)*4
                    // ms per tick = ms per measure / ticks_per_measure
                    float time_offset = 240000 / bpm_changes[current_bpm_index].bpm / ticks_per_measure;
                    current_time += time_offset;

                    current_position += time_offset * pixels_per_ms_at_1_bpm * bpm_changes[current_bpm_index].bpm;
                    // same as
                    //current_position += 240 / ticks_per_measure;  
                    // but only if screen height = 480
                    
                    // but why?  is there some other factor that is needed?
                    // surely, since in DDR as BPM goes up, space between arrows increases!

                    measure_line_index++;
                    
                    current_beat += 1.0/ticks_per_measure;
                    
                    if (DEBUG_LEVEL >= DEBUG_DETAIL)
                    {
                      debug_log.open("debug", std::ios_base::app);
                      debug_log << "done processing beat line. new arrow count=" << song_arrows[current_difficulty].size() << endl;
                      debug_log.close();
                    }
                  }
                  
                  // found end of measure.  continue note processing from here
                  note_line_index = measure_line_index;
                  if (DEBUG_LEVEL >= DEBUG_DETAIL)
                  {
                    debug_log.open("debug", std::ios_base::app);
                    debug_log << "done processing beat block.  updated note line index " << note_line_index << endl;
                    debug_log.close();
                  }
                  
                  // was this the last measure?  if so, continue directly to
                  // termination condition evaluation since otherwise the
                  // note_line_index increment at the end of the loop will
                  // put the index past the terminator and screw up detection.
                  // else we want the increment because the loop start code
                  // expects to be pointing on the first data line, not
                  // the separator (,)
                  if (file_lines[note_line_index].find(";") != string::npos)
                  {
                    if (DEBUG_LEVEL >= DEBUG_DETAIL)
                    {
                      debug_log.open("debug", std::ios_base::app);
                      debug_log << "detected end of last measure at index " << note_line_index << endl;
                      debug_log.close();
                    }
                    continue;
                  }
                }

                // the measure has been processed.
                
                note_line_index++;
                if (DEBUG_LEVEL >= DEBUG_DETAIL)
                {
                  debug_log.open("debug", std::ios_base::app);
                  debug_log << "done processing measure.  current_beat: " << current_beat << " note_line_index: " << note_line_index << endl;
                  debug_log.close();
                }
              }
              
              // skip past this processed data
              if (DEBUG_LEVEL >= DEBUG_DETAIL)
              {
                debug_log.open("debug", std::ios_base::app);
                debug_log << "current line skipping from " << current_line << " to " << note_line_index << endl;
                debug_log.close();
              }
              current_line = note_line_index;
            }
          }
        }
      }
    }
    
    current_line++;
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "current line incremented to " << current_line << endl;
      debug_log.close();
    }
  }
  
  indata.close();


  

  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "leaving read step data" << endl;
    debug_log.close();
  }
  
  return bpm_changes.size() > 0 && (
       difficulty_available[0]
    || difficulty_available[1]
    || difficulty_available[2]
    || difficulty_available[3]
    || difficulty_available[4]
  );
}


vector<bpm_change> song::parse_bpms(const string& s)
{
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "parse_bpms()" << endl;
    debug_log.close();
  }
  
  vector<bpm_change> bpm_change_data;
    
  //NOTE: sample and definition of SM BPMS tag
  //#BPMS:0=140.215,1=140.208,14=140.215,16=140.208,43=140.215;
  //
  //#BPMS:<int beat>=<float BPM>[,<int beat>=<float BPM>];
  //beat 0 must be defined
  
  size_t start_pos = 0;
  size_t end_pos = 0;
 
  while ( (start_pos = s.find_first_of(":,", start_pos)) != string::npos)
  {
    // search for end of first part of bpm token
    start_pos++;
    end_pos = s.find_first_of("=", start_pos);
    if (end_pos != string::npos)
    {
      string bpm_beat = s.substr(start_pos, end_pos - start_pos);

      // search for end of second part of bpm token
      start_pos = end_pos+1;
      end_pos = s.find_first_of(",;", start_pos);
      if (end_pos != string::npos)
      {
        // found complete bpm token
        string bpm_float = s.substr(start_pos, end_pos-start_pos);
        
        // add to array of bpm changes
        //NOTE: timestamp will be added later while parsing note data
        bpm_change_data.push_back(bpm_change(atof(bpm_beat.c_str()), 0, atof(bpm_float.c_str())));

        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << atof(bpm_float.c_str()) << " at " << atof(bpm_beat.c_str()) << endl;
          debug_log.close();
        }
      }
      else
      {
        #ifdef LOG_ERRORS
        error_log.open("errors", std::ios_base::app);
        error_log << "ERROR: invalid BPMS data!" << endl;
        error_log.close();
        #endif
      }
    }
  }
  
  #ifdef LOG_ERRORS
  if (bpm_change_data.size() == 0)
  {
    error_log.open("errors", std::ios_base::app);
    error_log << "ERROR: failed to extract BPMS!" << endl;
    error_log.close();
  }
  #endif  

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "parse_bpms() finished.  extracted " << bpm_change_data.size() << " bpm changes" << endl;
    debug_log.close();
  }  
  return bpm_change_data;
}

long song::parse_offset(const string& s)
{
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "parse_offset()" << endl;
    debug_log.close();
  }
  
  long offset_data = 0;
    
  size_t start_pos = 0;
  size_t end_pos = 0;
 
  start_pos = s.find(":");
  if (start_pos != string::npos)
  {
    start_pos++;
    // search for end of offset token
    end_pos = s.find(";", start_pos);
    if (end_pos != string::npos)
    {
      // found complete offset token
      string offset_long = s.substr(start_pos, end_pos-start_pos);
        
      offset_data = (long)(atof(offset_long.c_str())*1000.0);
      if (offset_data < 0) offset_data *= -1;
      
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "extracted long offset: " << offset_data << " from text (" << offset_long << ")" << endl;
        debug_log.close();
      }
    }      
    else
    {
      #ifdef LOG_ERRORS
      error_log.open("errors", std::ios_base::app);
      error_log << "ERROR: invalid BPMS data!" << endl;
      error_log.close();
      #endif
    }
  }

  return offset_data;
}

bool song::difficulty_is_available(int difficulty)
{
  if (difficulty < 0 || difficulty >= NUM_DIFFICULTIES) return false;
  return difficulty_available[difficulty];
}

#endif