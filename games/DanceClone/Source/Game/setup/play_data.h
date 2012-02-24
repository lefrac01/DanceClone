//TODO: fix current_song assignment.  not tight enough

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
  int player_base_arrow;
  vector<arrow> player_arrows;
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
};

play_data::play_data() :
current_player_data(MAX_PLAYERS)
{
}

bool play_data::init()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "play_data init() " << endl;
    debug_log.close();
  }
  current_beat_tick = -1;
  current_bpm_change = -1;
  player_arrows.clear();
  player_base_arrow = -1;
  song_start_time = WDgametime+(SDL_GetTicks()-WDruntime);
  song_time = 0;
  ms_per_pixel_at_1_bpm = 120.0*4000.0/rmode->viHeight;
  viewport_offset = 0;
  num_players = MAX_PLAYERS;
  
  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    current_player_data[i].init();
  }
  
  //TODO: receive players vector and use to augment this:
  player_data& pd = current_player_data[0];
  pd.player_arrows = current_song.song_arrows[difficulty];
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "copied " << current_player_data[0].player_arrows.size() << " arrows" << " for difficulty " << difficulty << endl;
    debug_log.close();
  }
  if (pd.player_arrows.size() > 0)
  {
    pd.first_visible_arrow = 0;
    //pd.current_ratable_arrow[0] = 
    //pd.current_ratable_arrow[1] = 
    // ...
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
      debug_log.open("debug", std::ios_base::app);
      debug_log << "play_data init() exiting with initial bpm: " << current_song.bpm_changes.front().bpm << endl;
      debug_log.close();
    }
    return true;
  }
  
  debug_log.open("debug", std::ios_base::app);
  debug_log << "ERROR: missing BPM" << endl;
  debug_log.close();
  return false;
}

void play_data::initial_frame()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "initial frame" << endl;
    debug_log.close();
  }

  song_start_time = SDL_GetTicks();
  pixels_left_to_scroll = 0.0;
}

void play_data::frame()
{
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "frame" << endl;
    debug_log.close();
  }
  
  // process song time
  long old_song_time = song_time;
  song_time = SDL_GetTicks() - song_start_time;

  // process partial frames
  long partial_frame_time_begin = old_song_time;
  long frame_time_end = song_time;
  
  // process beat ticks
  int frame_end_beat_tick = current_beat_tick;
  while (
    frame_end_beat_tick+1 < num_beat_ticks 
    &&  frame_time_end 
        >= current_song.beat_ticks[frame_end_beat_tick+1].timestamp
  )
  {
    frame_end_beat_tick++;
  }

  // process bpm changes
  int frame_end_bpm_change = current_bpm_change;
  if (frame_end_beat_tick != current_beat_tick)
  {
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "new beat tick(s) before new time: " << song_time << " (curre: " << current_beat_tick << " new: " << frame_end_beat_tick << ")" << endl;
      debug_log.close();
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
          debug_log.open("debug", std::ios_base::app);
          debug_log << "inc frame_end_bpm_change because " \
            << endl << "current_song.beat_ticks[i].beat " << current_song.beat_ticks[i].beat \
            << " >= current_song.bpm_changes[frame_end_bpm_change+1].beat " << current_song.bpm_changes[frame_end_bpm_change+1].beat \
            << " i=" << i << ", frame_end_bpm_change=" << frame_end_bpm_change << endl;
          debug_log.close();
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
          debug_log.open("debug", std::ios_base::app);
          debug_log << "partial frame updated, new vars partial_frame_time_begin: " << partial_frame_time_begin << " current_bpm_change: " << current_bpm_change << endl;
          debug_log.close();
        }

        // optimise pixels per second scroll calculation
        if (current_bpm_change != frame_end_bpm_change)
        {
          ms_per_pixel_at_current_bpm = 
              ms_per_pixel_at_1_bpm 
            / current_song.bpm_changes[frame_end_bpm_change].bpm;
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "new ms_per_pixel_at_current_bpm:" << ms_per_pixel_at_current_bpm << endl;
            debug_log.close();
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
      debug_log.open("debug", std::ios_base::app);
      debug_log << "new ms_per_pixel_at_current_bpm:" << ms_per_pixel_at_current_bpm << endl;
      debug_log.close();
    }
  }
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "frame end, updating current_bpm_change=" << frame_end_bpm_change << " current_beat_tick=" << frame_end_beat_tick << endl;
    debug_log.close();
  }
  current_bpm_change = frame_end_bpm_change;
  current_beat_tick = frame_end_beat_tick;
  
  //TEMP:
  frame_time = song_time - old_song_time;
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

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "partial_frame(" << begin << ", " << end << ")" \
      << endl << "whole_pixels_to_scroll: " << whole_pixels_to_scroll \
      << endl << "pixels_left_to_scroll: " << pixels_left_to_scroll \
      << endl << "viewport_offset: " << viewport_offset \
      << endl << "---------------" << endl;
    debug_log.close();
  }
}
