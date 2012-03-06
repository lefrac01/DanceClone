//TODO: need freeze arrow length in pixels and in ms
//TODO: need freeze arrow length in pixels and in ms
//
//TODO: fix current_song assignment.  not tight enough
//
//TODO: base class compatible_step_file with derivative sm_file
// to which vector of string can be passed to various parse functions
// that fill the passed members.  e.g. find_lines_offset(), parse_offset()

// play_data
//
#include "song.h"
#include "player_data.h"

#define MAX_PLAYERS 1

class play_data
{
private:

public:
  song current_song;
  int current_beat_tick;
  int num_beat_ticks;
  int current_bpm_change;
  int num_bpm_changes;
  long song_start_time;
  long song_time;
  long frame_time; //TEMP
  long viewport_offset;
  float ms_per_pixel_at_1_bpm;
  float ms_per_pixel_at_current_bpm;
  float pixels_left_to_scroll;
  vector<player_data> current_player_data;
  int num_players;
  
  play_data();
  bool init();
  void initial_frame();
  void frame();
  void partial_frame(long begin, long end);
  void read_player_controls(int p);
  void rate_arrows(int p);
};

play_data::play_data() :
current_player_data(MAX_PLAYERS)
{
}

bool play_data::init()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    //#log.open("debug", std::ios_base::app);
    log << "play_data init() " << endl;
    //#log.close();
  }
  current_beat_tick = -1;
  current_bpm_change = -1;
  song_start_time = WDgametime+(SDL_GetTicks()-WDruntime);
  song_time = 0;
  ms_per_pixel_at_1_bpm = 120.0*4000.0/rmode->viHeight;
  num_players = MAX_PLAYERS;
  
  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    current_player_data[i].init();
  }
  
  //TODO: receive players vector and use to augment this:
  player_data& pd = current_player_data[0];
  pd.arrows = current_song.song_arrows[difficulty];
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    //#log.open("debug", std::ios_base::app);
    log << "copied " << current_player_data[0].arrows.size() << " arrows" << " for difficulty " << difficulty << endl;
    //#log.close();
  }
  pd.num_arrows = pd.arrows.size();
  if (pd.num_arrows)
  {
    pd.next_offscreen_arrow = 0;
  }
 
  num_beat_ticks = current_song.beat_ticks.size();
  if (num_beat_ticks > 0)
  {
    current_beat_tick = 0;
  }
  num_bpm_changes = current_song.bpm_changes.size();
  if (num_bpm_changes > 0)
  {
    current_bpm_change = 0;
    ms_per_pixel_at_current_bpm = ms_per_pixel_at_1_bpm / current_song.bpm_changes.front().bpm;
  }
  
  if (current_song.initialised && current_beat_tick == 0 && current_bpm_change == 0)
  {
    if (DEBUG_LEVEL >= DEBUG_MINOR)
    {
      //#log.open("debug", std::ios_base::app);
      log << "play_data init() exiting with initial bpm: " << current_song.bpm_changes.front().bpm << endl;
      //#log.close();
    }
    return true;
  }
  
  //#log.open("debug", std::ios_base::app);
  log << "ERROR: missing BPM" << endl;
  //#log.close();
  return false;
}

void play_data::initial_frame()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    //#log.open("debug", std::ios_base::app);
    log << "initial frame" << endl;
    //#log.close();
  }

  // add an offset to song start time.  if offset is positive,
  // first frame will calculate a negative current song time, allowing
  // actual song playback to be on sync if it starts a little later
  // than song start function call
  song_start_time = SDL_GetTicks() + SONG_START_OFFSET + PLAYER_PREP_OFFSET;
  
  // viewport offset starts increasingly negative as song offset goes up,
  // so that after offset time it is at 0.  this synchronises rating and drawing
  viewport_offset = - ((SONG_START_OFFSET+PLAYER_PREP_OFFSET) / ms_per_pixel_at_current_bpm);
  
  // not taking into account partial pixels for initial offset. 
  // fractional pixels are only important over time
  pixels_left_to_scroll = 0.0;
}

void play_data::frame()
{
  // process song time
  long old_song_time = song_time;
  song_time = SDL_GetTicks() - song_start_time;

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "frame old_song_time: " << old_song_time << " song_time:" << song_time << endl;
    //#log.close();
  }

 
  // process partial frames
  long partial_frame_time_begin = old_song_time;
  long frame_time_end = song_time;
  
//log << "fte: " << frame_time_end << endl; //TEMPLOG:
  
  // process beat ticks
  int frame_end_beat_tick = current_beat_tick;
  while (
    frame_end_beat_tick+1 < num_beat_ticks 
    &&  frame_time_end 
        >= current_song.beat_ticks[frame_end_beat_tick+1].timestamp
  )
  {
//log << "inc febt " << endl; //TEMPLOG:
    frame_end_beat_tick++;
  }

  // process bpm changes
  int frame_end_bpm_change = current_bpm_change;
  if (frame_end_beat_tick != current_beat_tick)
  {
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      //#log.open("debug", std::ios_base::app);
      log << "new beat tick(s) before new time: " << song_time << " (curre: " << current_beat_tick << " new: " << frame_end_beat_tick << ")" << endl;
      //#log.close();
    }
    
    // a new beat will be encountered.  check for bpm change
    for (int i = current_beat_tick+1; i <= frame_end_beat_tick; i++)
    {
      //NOTE: account for possibility of more than one bpm change per
      // tick, but only use the last bpm change.
      // for robustness because I'm unsure of SM file format specs.
      //NOTE: beat 0 bpm change is not detected.  this is ok, beat 0
      // bpm is assigned at song init so is not actually a change in bpm.
      while (
        frame_end_bpm_change+1 < num_bpm_changes 
        &&  current_song.beat_ticks[i].beat 
            >= current_song.bpm_changes[frame_end_bpm_change+1].beat
      )
      {
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          //#log.open("debug", std::ios_base::app);
          log << "inc frame_end_bpm_change because " \
            << endl << "current_song.beat_ticks[i].beat " << current_song.beat_ticks[i].beat \
            << " >= current_song.bpm_changes[frame_end_bpm_change+1].beat " << current_song.bpm_changes[frame_end_bpm_change+1].beat \
            << " i=" << i << ", frame_end_bpm_change=" << frame_end_bpm_change << endl;
          //#log.close();
        }
        frame_end_bpm_change++;
      }
      
      // process bpm change
      if (frame_end_bpm_change != current_bpm_change)
      {
        // update play data with the partial scroll between current
        // position and the position of the bpm change
        long partial_frame_time_end = current_song.bpm_changes[frame_end_bpm_change].timestamp;
        partial_frame(partial_frame_time_begin, partial_frame_time_end);
        
        // update variables needed to calculate next partial
        partial_frame_time_begin = partial_frame_time_end;

        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          //#log.open("debug", std::ios_base::app);
          log << "partial frame updated, new vars partial_frame_time_begin: " << partial_frame_time_begin << " current_bpm_change: " << current_bpm_change << endl;
          //#log.close();
        }

        // optimise pixels per second scroll calculation
        if (current_bpm_change != frame_end_bpm_change)
        {
          ms_per_pixel_at_current_bpm = 
              ms_per_pixel_at_1_bpm 
            / current_song.bpm_changes[frame_end_bpm_change].bpm;
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            //#log.open("debug", std::ios_base::app);
            log << "A new ms_per_pixel_at_current_bpm:" << ms_per_pixel_at_current_bpm << endl;
            //#log.close();
          }
        }
        current_bpm_change = frame_end_bpm_change;
      }
    }
  }
  
  // process final partial frame (entire frame in case where no bpm changes
  // occurred during frame)
  if (partial_frame_time_begin < frame_time_end)
  {
    partial_frame(partial_frame_time_begin, frame_time_end);
  }

  // optimise pixels per second scroll calculation
  if (current_bpm_change != frame_end_bpm_change)
  {
    ms_per_pixel_at_current_bpm = 
        ms_per_pixel_at_1_bpm 
      / current_song.bpm_changes[frame_end_bpm_change].bpm;
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      //#log.open("debug", std::ios_base::app);
      log << "B new ms_per_pixel_at_current_bpm:" << ms_per_pixel_at_current_bpm << endl;
      //#log.close();
    }
  }
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "frame end, updating current_bpm_change=" << frame_end_bpm_change << " current_beat_tick=" << frame_end_beat_tick << endl;
    //#log.close();
  }
  current_bpm_change = frame_end_bpm_change;
  current_beat_tick = frame_end_beat_tick;
  
  //TEMP:
  frame_time = song_time - old_song_time;
  
  // detect player input
  read_player_controls(0);
  
  rate_arrows(0);
}

void play_data::partial_frame(long begin, long end)
{
  // process viewport scroll
  float pixels_to_scroll = (end-begin) / ms_per_pixel_at_current_bpm;
  
  // add partial pixels left from last scroll's truncation
  pixels_to_scroll += pixels_left_to_scroll;  
  
  long whole_pixels_to_scroll = (long)pixels_to_scroll;
  pixels_left_to_scroll = pixels_to_scroll - whole_pixels_to_scroll;

  viewport_offset += whole_pixels_to_scroll;
}

void play_data::read_player_controls(int p)
{
  #ifdef WIN
  pd.up_control = keystate[SDLK_UP];
  pd.down_control = keystate[SDLK_DOWN];
  pd.left_control = keystate[SDLK_LEFT];
  pd.right_control = keystate[SDLK_RIGHT];
  #endif

  // update player controls
  #ifdef WII
  //u32 WiiButtons1Held = WiiButtonsHeld[0];
  //u32 WiiButtons1Down = WiiButtonsDown[0];
  u16 GCButtons1Held = GCButtonsHeld[0];
  u16 GCButtons1Down = GCButtonsDown[0];
  //expansion_t WPAD_Expansion1 = expans[0];

  //TODO: multiplayer
  player_data& pd = current_player_data[p];

  pd.up_control_down =    GCButtons1Down & PAD_BUTTON_UP;
  pd.down_control_down =  GCButtons1Down & PAD_BUTTON_DOWN;
  pd.left_control_down =  GCButtons1Down & PAD_BUTTON_LEFT;
  pd.right_control_down = GCButtons1Down & PAD_BUTTON_RIGHT;
  pd.up_control_held =    GCButtons1Held & PAD_BUTTON_UP;
  pd.down_control_held =  GCButtons1Held & PAD_BUTTON_DOWN;
  pd.left_control_held =  GCButtons1Held & PAD_BUTTON_LEFT;
  pd.right_control_held = GCButtons1Held & PAD_BUTTON_RIGHT;
/*
  if(WPAD_Expansion1.type == WPAD_EXP_CLASSIC)
  {
    pd.up_control = (WiiButtons1Held & WPAD_CLASSIC_BUTTON_UP) || (WiiButtons1Down & WPAD_CLASSIC_BUTTON_UP);
    pd.down_control = (WiiButtons1Held & WPAD_CLASSIC_BUTTON_DOWN) || (WiiButtons1Down & WPAD_CLASSIC_BUTTON_DOWN);
    pd.left_control = (WiiButtons1Held & WPAD_CLASSIC_BUTTON_LEFT) || (WiiButtons1Down & WPAD_CLASSIC_BUTTON_LEFT);
    pd.right_control = (WiiButtons1Held & WPAD_CLASSIC_BUTTON_RIGHT) || (WiiButtons1Down & WPAD_CLASSIC_BUTTON_RIGHT);
  }
  */
  
  #endif
}

void play_data::rate_arrows(int p)
{
  player_data& pd = current_player_data[p];
  for(int a = pd.first_visible_arrow; a <= pd.last_visible_arrow; a++)
  {
    arrow& ar = pd.arrows[a];
    if (ar.rating == RATING_NONE && song_time - ar.time > BOO_MS)
    {
log << "MISS ar:" << a << endl; //TEMPLOG: outputting vars anims depend on 
      ar.rating = RATING_MISS;
      if (ar.type == NOTE_TYPE_HOLD)
      {
        ar.freeze_rating = FREEZE_RATING_FAILED;
      }
      pd.combo=0;
    }
  }

  for(int b=0; b<4; b++)
  {
    if((b==0 && pd.left_control_down)||(b==1 && pd.down_control_down)||(b==2 && pd.up_control_down)||(b==3 && pd.right_control_down))
    {
      for(int a = pd.first_visible_arrow; a <= pd.last_visible_arrow; a++)
      {
        arrow& ar = pd.arrows[a];
        if (ar.direction == b && ar.rating == RATING_NONE)
        {
          int new_rating = RATING_NONE;
          if(abs(ar.time - song_time) <= MARVELLOUS_MS)
          {
            new_rating = RATING_MARVELLOUS;
          }
          if(abs(ar.time - song_time) <= PERFECT_MS)
          {
            new_rating = RATING_PERFECT;
          }
          else if(abs(ar.time-song_time) <= GREAT_MS)
          {
            new_rating = RATING_GREAT;
          }
          else if(abs(ar.time-song_time) <= GOOD_MS)
          {
            new_rating = RATING_GOOD;
          }
          
          if (new_rating != RATING_NONE)
          {
log << "RATE ar:" << a << " rating: " << new_rating << " because ar.t-st=" << abs(ar.time-song_time) << endl; //TEMPLOG: outputting vars anims depend on 
            ar.anim_start_time = song_time;
            ar.rating = new_rating;
            ++pd.combo;
            if (ar.length == 0)
            {
              ar.hidden = true;
            }
            break;
          }
        }
      }
    }
  }

  for(int b=0;b<4;b++)
  {
    if((b==0 && pd.left_control_held)||(b==1 && pd.down_control_held)||(b==2 && pd.up_control_held)||(b==3 && pd.right_control_held))
    {
      for(unsigned int a=pd.base_arrow;a<pd.arrows.size();a++)
      {
        arrow& ar = pd.arrows[a];
        if (ar.direction == b && ar.type == NOTE_TYPE_HOLD && ar.length != 0 && ar.rating != RATING_NONE && ar.rating != RATING_MISS && ar.freeze_rating == FREEZE_RATING_NONE)
        {
          if (abs(viewport_offset - ar.ypos) >= ar.length)
          {
            // don't give additional credit for zero-length freeze arrows.
            // the note type hold is used on these to enforce the rule that
            // all notes on the same row as a freeze arrow have freeze graphics
            if (ar.length != 0)
            {
    log << "RATE ar:" << a << " freeze rating ok." << endl; //TEMPLOG: outputting vars anims depend on 
              ar.freeze_rating = FREEZE_RATING_OK;
            }
            ar.hidden = true;
            ar.anim_start_time = song_time;
          }
        }
      }
    }
    else if((b==0 && !pd.left_control_held)||(b==1 && !pd.down_control_held)||(b==2 && !pd.up_control_held)||(b==3 && !pd.right_control_held))
    {
      for(unsigned int a=pd.base_arrow;a<pd.arrows.size();a++)
      {
        arrow& ar = pd.arrows[a];
        if (ar.direction == b && ar.type == NOTE_TYPE_HOLD && ar.length != 0 && ar.rating != RATING_NONE && ar.rating != RATING_MISS && ar.freeze_rating == FREEZE_RATING_NONE)
        {
          // don't fail freeze arrow too close to end, it looks funny and
          // this is supposed to be a fun game anyway ;)
          if (ar.length - abs(viewport_offset - ar.ypos) > FREEZE_LENGTH_ALLOW)
          {
  log << "RATE ar:" << a << " freeze rating FAILED." << endl; //TEMPLOG: outputting vars anims depend on 
            ar.freeze_rating = FREEZE_RATING_FAILED;
            // when a freeze arrow is failed, it grows a new head at the 
            // point where it was failed for the rest of the darkened
            // fail state drawing
            long old_ypos = ar.ypos;
            ar.ypos += viewport_offset - ar.ypos;
            ar.length -= ar.ypos - old_ypos;
          }
          else
          {
            //TODO: above loop that detects success is filtering on the pad
            // direction being pressed... investigate eliminating this duplication
            if (abs(viewport_offset - ar.ypos) >= ar.length)
            {
              // don't give additional credit for zero-length freeze arrows.
              // the note type hold is used on these to enforce the rule that
              // all notes on the same row as a freeze arrow have freeze graphics
              if (ar.length != 0)
              {
      log << "RATE ar:" << a << " freeze rating ok." << endl; //TEMPLOG: outputting vars anims depend on 
                ar.freeze_rating = FREEZE_RATING_OK;
              }
              ar.hidden = true;
              ar.anim_start_time = song_time;
            }
          }
        }
      }
    }
  }  
  
  //TODO: all rating of arrows should be looping by arrow not by direction
  // might be nice to be able to do if (pl.control_active[ar.direction])
}
