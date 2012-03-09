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
  long pre_start_time;
  long song_abort_start_time;
  long frame_time; //TEMP
  long viewport_offset;
  float ms_per_pixel_at_1_bpm;
  float ms_per_pixel_at_current_bpm;
  float pixels_left_to_scroll;
  vector<player_data> current_player_data;
  int num_players;
  
  play_data();
  bool pre_start_delay_finished();
  void check_abort();
  bool init();
  void initial_frame();
  void frame();
  void partial_frame(long begin, long end);
  void read_player_controls(int p);
  void rate_arrows(int p);
};

play_data::play_data() :
pre_start_time(-1),
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
  song_abort_start_time = -1;
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

bool play_data::pre_start_delay_finished()
{
  if (pre_start_time == -1)
  {
    pre_start_time = SDL_GetTicks();
  }
  if (SDL_GetTicks() - pre_start_time > PRE_START_DELAY)
  {
    pre_start_time = -1;
    return true;
  }
  else
  {
    return false;
  }
}

void play_data::check_abort()
{
  bool abort_held = false;
  for (int i = 0; i < num_players; i++)
  {
    if(   (WiiButtonsHeld[i] & WPAD_BUTTON_A &&  WiiButtonsHeld[i] & WPAD_BUTTON_B)
       || (GCButtonsHeld[i]  & PAD_BUTTON_A  &&  GCButtonsHeld[i]  & PAD_BUTTON_B)
    ) {
      abort_held = true;
    }
  }
  if (abort_held)
  {
    if (song_abort_start_time == -1)
    {
      song_abort_start_time = SDL_GetTicks();
    }
    else
    {
      if (SDL_GetTicks() - song_abort_start_time > SONG_ABORT_DELAY)
      {
        song_abort_start_time = -1;
        MP3Player_Stop();
        gamestate=4;
      }
    }
  }
  else
  {
    song_abort_start_time = -1;
  }
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
  song_start_time = SDL_GetTicks() + SONG_START_OFFSET;
  
  // viewport offset starts increasingly negative as song offset goes up,
  // so that after offset time it is at 0.  this synchronises rating and drawing
  viewport_offset = - ((SONG_START_OFFSET) / ms_per_pixel_at_current_bpm);
  
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
  pd.up_control_down = keystate[SDLK_UP];
  pd.down_control_down = keystate[SDLK_DOWN];
  pd.left_control_down = keystate[SDLK_LEFT];
  pd.right_control_down = keystate[SDLK_RIGHT];
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

  pd.control_data[UP].down =      GCButtons1Down & PAD_BUTTON_UP;
  pd.control_data[DOWN].down =    GCButtons1Down & PAD_BUTTON_DOWN;
  pd.control_data[LEFT].down =    GCButtons1Down & PAD_BUTTON_LEFT;
  pd.control_data[RIGHT].down =   GCButtons1Down & PAD_BUTTON_RIGHT;
  pd.control_data[UP].held =      GCButtons1Held & PAD_BUTTON_UP;
  pd.control_data[DOWN].held =    GCButtons1Held & PAD_BUTTON_DOWN;
  pd.control_data[LEFT].held =    GCButtons1Held & PAD_BUTTON_LEFT;
  pd.control_data[RIGHT].held =   GCButtons1Held & PAD_BUTTON_RIGHT;

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
  
  // check clearing control down start time
  for (int i = 0; i < NUM_CONTROL_DIRECTIONS; i++)
  {
    if (pd.control_data[i].down_time != -1 && !pd.control_data[i].held)
    {
      pd.control_data[i].down_time = -1;
      pd.control_data[i].jump_active = false;
    }
  }

  
  // check setting control down start time
  for (int i = 0; i < NUM_CONTROL_DIRECTIONS; i++)
  {
    if (pd.control_data[i].down) 
    {
      pd.control_data[i].down_time = song_time;
    }
  }
  
  // check setting jump flag for each direction
  int num_controls_down = 0;
  for (int i = 0; i < NUM_CONTROL_DIRECTIONS; i++)
  {
    if (pd.control_data[i].down_time != -1)
    {
      ++num_controls_down;
    }
  }
  if (num_controls_down > 1)
  {
    // all controls held down since a time smaller than the allowed jump delay
    // are considered part of a jump
    for (int i = 0; i < NUM_CONTROL_DIRECTIONS; i++)
    {
      pd.control_data[i].jump_active = song_time - pd.control_data[i].down_time <= JUMP_ALLOW_MS;
    }
  }
}

void play_data::rate_arrows(int p)
{
  player_data& pd = current_player_data[p];
  for(int a = pd.first_visible_arrow; a <= pd.last_visible_arrow; a++)
  {
    arrow& ar = pd.arrows[a];
    if (ar.rating == RATING_NONE && song_time - ar.time > BOO_MS)
    {
log << "MISS ar:" << a << " because song_time - ar.time = " << song_time << " - " << ar.time << " == " << song_time-ar.time << endl; //TEMPLOG: outputting vars anims depend on 
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
    if (pd.control_data[b].down)
    {
      for(int a = pd.first_visible_arrow; a <= pd.last_visible_arrow; a++)
      {
        arrow& ar = pd.arrows[a];
        
        // when an arrow is reached that is too far ahead, don't bother looping any further
        if (ar.time - song_time > BOO_MS) break;
        
        if (ar.direction == b && ar.rating == RATING_NONE)
        {
log << "check ar:" << a << "  song_time - ar.time = " << song_time << " - " << ar.time << " == " << song_time-ar.time << endl; //TEMPLOG: outputting vars anims depend on 
          int new_rating = RATING_NONE;
          if(labs(ar.time - song_time) <= MARVELLOUS_MS)
          {
            new_rating = RATING_MARVELLOUS;
          }
          else if(labs(ar.time - song_time) <= PERFECT_MS)
          {
            new_rating = RATING_PERFECT;
          }
          else if(labs(ar.time-song_time) <= GREAT_MS)
          {
            new_rating = RATING_GREAT;
          }
          else if(labs(ar.time-song_time) <= GOOD_MS)
          {
            new_rating = RATING_GOOD;
          }
          else if(labs(ar.time-song_time) <= BOO_MS)
          {
            new_rating = RATING_MISS;
          }
          

          // jump rating: all arrows present in the same row are considered
          // part of a jump group.  all jump directions must be active for
          // these to be rated and they count as one arrow with one rating
          int min_jump_arrow_index = a;
          int max_jump_arrow_index = a;
          while (min_jump_arrow_index-1 >= pd.first_visible_arrow)
          {
            if (pd.arrows[min_jump_arrow_index-1].time != ar.time)
            {
              break;
            }
            --min_jump_arrow_index;
          }
          while (max_jump_arrow_index+1 <= pd.last_visible_arrow)
          {
            if (pd.arrows[max_jump_arrow_index+1].time != ar.time)
            {
              break;
            }
            ++max_jump_arrow_index;
          }
          if (min_jump_arrow_index != max_jump_arrow_index)
          {
            // more than one arrow on the same line.  are jump flags
            // for each necessary direction active?
            for (int i = min_jump_arrow_index; i <= max_jump_arrow_index; i++)
            {
              if (pd.control_data[pd.arrows[i].direction].jump_active == false)
              {
log << "NULLING RATE ar:" << a << " because control direction " << pd.arrows[i].direction << " not jump active " << " i: " << i << "  min idx: " << min_jump_arrow_index << " max: " << max_jump_arrow_index << endl; //TEMPLOG: outputting vars anims depend on 
                new_rating = RATING_NONE;
              }
            }
            // if the rating is still good, apply it to all arrows in the jump
            if (new_rating != RATING_NONE)
            {
              for (int i = min_jump_arrow_index; i <= max_jump_arrow_index; i++)
              {
                arrow& jmp_ar = pd.arrows[i];
                
    log << "JUMP RATE ar:" << i << " rating: " << new_rating << endl;
                jmp_ar.anim_start_time = song_time;
                jmp_ar.rating = new_rating;
                ++pd.combo;
                if (jmp_ar.length == 0)
                {
                  jmp_ar.hidden = true;
                }
              }
              break;
            }
          }
          else
          {
            // non-jump arrow case
            if (new_rating != RATING_NONE)
            {
  log << "RATE ar:" << a << " rating: " << new_rating << " because ar.t-st=" << labs(ar.time-song_time) << endl; //TEMPLOG: outputting vars anims depend on 
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
  }

  for(int b=0;b<4;b++)
  {
    if (pd.control_data[b].held)
    {
      for(unsigned int a=pd.base_arrow;a<pd.arrows.size();a++)
      {
        arrow& ar = pd.arrows[a];
        if (ar.direction == b && ar.type == NOTE_TYPE_HOLD && ar.length != 0 && ar.rating != RATING_NONE && ar.rating != RATING_MISS && ar.freeze_rating == FREEZE_RATING_NONE)
        {
          if (labs(viewport_offset - ar.ypos) >= ar.length)
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
    else
    {
      for(unsigned int a=pd.base_arrow;a<pd.arrows.size();a++)
      {
        arrow& ar = pd.arrows[a];
        if (ar.direction == b && ar.type == NOTE_TYPE_HOLD && ar.length != 0 && ar.rating != RATING_NONE && ar.rating != RATING_MISS && ar.freeze_rating == FREEZE_RATING_NONE)
        {
          // don't fail freeze arrow too close to end, it looks funny and
          // this is supposed to be a fun game anyway ;)
          if (ar.length - labs(viewport_offset - ar.ypos) > FREEZE_LENGTH_ALLOW)
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
            if (labs(viewport_offset - ar.ypos) >= ar.length)
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
