// play_data
//
#include "song.h"
#include "player_data.h"

#define MAX_PLAYERS 1
song blank_song;

class play_data
{
private:

public:
  song& current_song;
  int current_bpm_index;
  int current_beat;
  int player_base_arrow;
  vector<arrow> player_arrows;
  long song_start_time;
  long song_time;
  float current_bpm;
  vector<player_data> current_player_data;
  int num_players;
  
  play_data();
  bool init(song& s);
  float get_current_bpm();
  int get_num_players();
};

play_data::play_data() :
current_song(blank_song),
current_player_data(MAX_PLAYERS)
{
}

bool play_data::init(song& s)
{
  current_bpm_index = 0;
  current_beat = 0;
  player_arrows.clear();
  player_base_arrow = 0;
  song_start_time = WDgametime+(SDL_GetTicks()-WDruntime);
  song_time = 0;
  current_bpm_index = 0;
  if (s.bpm_changes.size() > 0)
  {
    current_bpm = s.bpm_changes.front().bpm;
  }
  num_players = MAX_PLAYERS;
  return current_bpm != 0;
}

float play_data::get_current_bpm()
{
  if (!current_song.initialised || current_song.bpm_changes.size() == 0) return -1;
  return current_song.bpm_changes.front().bpm;
}




/*
// setup player arrow
  player_arrows_in = song_arrows[difficulty_in];
  player_base_arrow = 0;
*/
