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
    debug_log.open("debug", std::ios_base::app);
    debug_log << "play_data init() " << endl;
    debug_log.close();
  }
  current_beat_tick = -1;
  current_bpm_change = -1;
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
  pd.arrows = current_song.song_arrows[difficulty];
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "copied " << current_player_data[0].arrows.size() << " arrows" << " for difficulty " << difficulty << endl;
    debug_log.close();
  }
  pd.num_arrows = pd.arrows.size();
  if (pd.num_arrows)
  {
    pd.next_offscreen_arrow = 0;
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

void play_data::read_player_controls(int p)
{
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_play_controls() begins" << endl;
    debug_log.close();
  }

  // update player controls
  
  u32 WiiButtons1Held = WiiButtonsHeld[0];
  u32 WiiButtons1Down = WiiButtonsDown[0];
  u16 GCButtons1Held = GCButtonsHeld[0];
  u16 GCButtons1Down = GCButtonsDown[0];
  expansion_t WPAD_Expansion1 = expans[0];

  //TODO: multiplayer
  //TODO: leaving in reference to frame_time but 
  // this is to move to a function called from frame() where
  // it will receive frame time there
  player_data& pd = current_player_data[p];
  pd.up_control = pd.up_control - frame_time;
  pd.down_control = pd.down_control - frame_time;
  pd.left_control = pd.left_control - frame_time;
  pd.right_control = pd.right_control - frame_time;


  if(pd.up_control<0)pd.up_control=0;
  if(pd.down_control<0)pd.down_control=0;
  if(pd.left_control<0)pd.left_control=0;
  if(pd.right_control<0)pd.right_control=0;

  #ifdef WIN
  if(keystate[SDLK_UP] && pd.up_control==0){pd.up_control=1;}
  if(keystate[SDLK_DOWN] && pd.down_control==0){pd.down_control=1;}
  if(keystate[SDLK_LEFT] && pd.left_control==0){pd.left_control=1;}
  if(keystate[SDLK_RIGHT] && pd.right_control==0){pd.right_control=1;}
  if(keystate[SDLK_UP]==2)pd.up_control=125;
  if(keystate[SDLK_DOWN]==2)pd.down_control=125;
  if(keystate[SDLK_LEFT]==2)pd.left_control=125;
  if(keystate[SDLK_RIGHT]==2)pd.right_control=125;
  #endif
  
  #ifdef WII

  if(pd.up_control==0 && WiiButtons1Held & WPAD_BUTTON_UP)pd.up_control=1;
  if(pd.down_control==0 && WiiButtons1Held & WPAD_BUTTON_DOWN)pd.down_control=1;
  if(pd.left_control==0 && WiiButtons1Held & WPAD_BUTTON_LEFT)pd.left_control=1;
  if(pd.right_control==0 && WiiButtons1Held & WPAD_BUTTON_RIGHT)pd.right_control=1;
  if(WiiButtons1Down & WPAD_BUTTON_UP)pd.up_control=125;
  if(WiiButtons1Down & WPAD_BUTTON_DOWN)pd.down_control=125;
  if(WiiButtons1Down & WPAD_BUTTON_LEFT)pd.left_control=125;
  if(WiiButtons1Down & WPAD_BUTTON_RIGHT)pd.right_control=125;



  if(pd.up_control==0 && GCButtons1Held & PAD_BUTTON_UP)pd.up_control=1;
  if(pd.down_control==0 && GCButtons1Held & PAD_BUTTON_DOWN)pd.down_control=1;
  if(pd.left_control==0 && GCButtons1Held & PAD_BUTTON_LEFT)pd.left_control=1;
  if(pd.right_control==0 && GCButtons1Held & PAD_BUTTON_RIGHT)pd.right_control=1;
  if(GCButtons1Down & PAD_BUTTON_UP)pd.up_control=125;
  if(GCButtons1Down & PAD_BUTTON_DOWN)pd.down_control=125;
  if(GCButtons1Down & PAD_BUTTON_LEFT)pd.left_control=125;
  if(GCButtons1Down & PAD_BUTTON_RIGHT)pd.right_control=125;



  if(WPAD_Expansion1.type == WPAD_EXP_CLASSIC){
    if(pd.up_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_UP)pd.up_control=1;
    if(pd.down_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_DOWN)pd.down_control=1;
    if(pd.left_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_LEFT)pd.left_control=1;
    if(pd.right_control==0 && WiiButtons1Held & WPAD_CLASSIC_BUTTON_RIGHT)pd.right_control=1;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_UP)pd.up_control=125;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_DOWN)pd.down_control=125;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_LEFT)pd.left_control=125;
    if(WiiButtons1Down & WPAD_CLASSIC_BUTTON_RIGHT)pd.right_control=125;    
  }

  #endif
}

void play_data::rate_arrows(int p)
{
  player_data& pd = current_player_data[p];

  // rate arrows

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "checking for boo" << endl;
    debug_log.close();
  }
  //TODO: if arrow has flowed past, consider it still the next ratable
  // arrow until either the max time(boo rating) or until the next
  // arrow is sooner.  this is too allow excluding old arrows before the
  // normal timeout if a stream of closely-placed arrows arrives.
  //detect_missed_arrows(0);
  for(int a=pd.first_visible_arrow;a<pd.last_visible_arrow;a++)if(pd.arrows[a].time-song_time<-BOO_MS)
  {
    if (pd.arrows[a].rated == false)
    {
      pd.arrows[a].hit = true;
      pd.arrows[a].rated = true;
      //ratearrow(a,0);
      pd.combo=0;++pd.boo;
    }
  }
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "checking for perfect / good" << endl;
    debug_log.close();
  }
  
  //TODO: record rating.  each direction has a "current ratable arrow" 
  // whose y position is analysed relative to a scale calculated based on 
  // current BPM.  separate each control's hit detection statement to 
  // use the current ratable arrow for each column.
  
  
  //TODO: fix this.
//  detect_perfect_arrows(0);

  for(int b=0;b<4;b++)
  if((b==0 && pd.left_control==125)||(b==1 && pd.down_control==125)||(b==2 && pd.up_control==125)||(b==3 && pd.right_control==125))
  {
    for(int a=pd.first_visible_arrow;a<pd.last_visible_arrow;a++)if(pd.arrows[a].length==0)if(pd.arrows[a].direction==b)
    {
      if(pd.arrows[a].time-song_time>-PERFECT_MS && pd.arrows[a].time-song_time<PERFECT_MS)
      {
        if (pd.arrows[a].rated == false)
        {
          pd.arrows[a].hit = true;
          pd.arrows[a].rated = true;
  //        ratearrow(a,2);
          ++pd.combo;++pd.perfect;break;
        }
      }
      else if(pd.arrows[a].time-song_time>-GOOD_MS && pd.arrows[a].time-song_time<GOOD_MS)
      {
        if (pd.arrows[a].rated == false)
        {
          pd.arrows[a].hit = true;
          pd.arrows[a].rated = true;
  //        ratearrow(a,1);
          ++pd.combo;++pd.good;break;
        }
      }
    }
  }
  
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "checking for perfect on held arrow" << endl;
    debug_log.close();
  }
  for(int b=0;b<4;b++)
  if((b==0 && pd.left_control)||(b==1 && pd.down_control)||(b==2 && pd.up_control)||(b==3 && pd.right_control))
  {
    for(unsigned int a=pd.base_arrow;a<pd.arrows.size();a++)if(pd.arrows[a].length!=0)if(pd.arrows[a].direction==b)
    {
      if(pd.arrows[a].time-song_time>-1000/5 && pd.arrows[a].time-song_time<1000/5)
      {
//        pd.arrows[a].ypos+=timehaspast;
//        pd.arrows[a].length-=timehaspast;
//        if(pd.arrows[a].length<=0)
//        {
//          ratearrow(a,2);
//        }

        //TODO: fix length check
        //combo=combo+1;perfect=perfect+1;
      }
    }
  }
}
