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
  int current_bpm_index;
  int current_beat;
  int player_base_arrow;
  vector<arrow> player_arrows;
  long song_start_time;
  long song_time;
  long frame_time;
  long viewport_offset;
  float current_bpm;
  float scroll_rate;
  vector<player_data> current_player_data;
  int num_players;
  
  play_data();
  bool init();
  float get_current_bpm();
  int get_num_players();
  void update();
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
  current_bpm_index = 0;
  current_beat = 0;
  player_arrows.clear();
  player_base_arrow = 0;
  song_start_time = WDgametime+(SDL_GetTicks()-WDruntime);
  song_time = 0;
  viewport_offset = 0;
  current_bpm_index = 0;
  
  if (current_song.initialised && current_song.bpm_changes.size() > 0)
  {
    current_bpm = current_song.bpm_changes.front().bpm;
  }
  num_players = MAX_PLAYERS;
  
  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    current_player_data[i].init();
  }
  scroll_rate = 23.5;//something based on BPM
  
  //TODO: receive players vector and use to augment this:
  current_player_data[0].player_arrows = current_song.song_arrows[difficulty];
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "copied " << current_player_data[0].player_arrows.size() << " arrows" << " for difficulty " << difficulty << endl;
    debug_log.close();
  }
 
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "play_data init() exiting with current_bpm " << current_bpm << endl;
    debug_log.close();
  }
  return current_bpm != 0;
}

float play_data::get_current_bpm()
{
  if (!current_song.initialised || current_song.bpm_changes.size() == 0) return -1;
  return current_song.bpm_changes.front().bpm;
}

void play_data::update()
{
  long temp = song_time;
  song_time = WDgametime - song_start_time;
  frame_time = song_time - temp;
  //timehaspast=songtime;
  //songtime=WDgametime-songstarttime;
  //timehaspast=songtime-timehaspast;



  //THINK:
  //
  // to avoid problems with long-term sync, detect the next beat ms
  // according to current time and current bpm and last beat.
  // then split this frame into two sections.
  // go up to the beat, then process things like bpm changes and stops.
  // continue processing the remainder of the frame time.
  // think ahead to save time with the implementation...


  
  // which beat are we on?
  
  
  // update current bpm
  
  
  // scroll speed per current bpm









  


  // sample song @ 126 BPM
  // scrolls about 120 pixels per second
  // 1000 / 120 ~= 8.333
  // 
  
  // 4 screen heights per second for a quarter note at 120 BPM
  












  // how many pixels does the view window move down over the arrow 
  // field in one millisecond?  this depends on a base rate adjusted by
  // the beat 0 BPM.
  // also, the #OFFSET; value must be put back at the tope ofe thee filee
  // you loosser because you fooked the one you calced for the first arrow
  // by *= some funky rate. hmm
  
// this is currently used to create DC file:
//   currentpos+=notetimevalue*1000*60/tempBPMS;

// soooo, if BPM = 60, 1 beat = 1000 ms.  with a virtual to screen factor
// of 1, that is also 1000 pixels, so a bit more than two screen heights
// per second at 60 BPM.  this means the factor must be modified.  at
// 140 BPM the arrows are on-screen for nearly a second before covering 
// two thirds of the screen.  ~360 px

// if BPM = 120, 1 beat = 500 ms, 500 px.
 
  // set song ms per pixel based on beat 0 BPM
  //song_ms_per_pixel = base_ms_per_pixel / current_play_data.get_current_bpm();
  
  // rating distances are pixel distances based on current BPM and screen
  // dimensions
  // boo = number of pixels scrolled after 200 ms 

  
  //     ~ 400px / second @ 140 BPM
  //
  //   = ~ 171px / second @ 60 BPM
  //
  //   = ~ 171px / 1000ms @ 60 BPM
  //   = ~ 0.171px/ms @ 60 BPM
  //   = ~ 6 ms/px @ 60 BPM
  //   = ~ 350 ms / px @ 1 BPM
  
  // another way: beats per screen height, screen heights per second at given BPM
  // given a screen height y this gives how many y per second at given BPM
  
  
  
  
  
  
  
  
  
  
  // scroll
  viewport_offset += frame_time / scroll_rate;
     // right now?

  
}
