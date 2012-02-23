// play_logic
//
#include "play_data.h"

//TODO: .ini for rating times
#define BOO_MS 180
#define GOOD_MS 135
#define GREAT_MS 90
#define PERFECT_MS 45
#define MARVELLOUS_MS 23
 

extern play_data current_play_data;

void rate_arrow(int p, int arrownum, int rate)
{
  //TODO: fix ratings
  //#ratings[n_ratings] = new rating(rmode->viWidth/2-125/2, goaloffset, rate);
  //#n_ratings++;
  current_play_data.current_player_data[p].player_base_arrow++;
}

void detect_missed_arrows(int p)
{
  for (unsigned int a = current_play_data.current_player_data[p].player_base_arrow; a < current_play_data.current_player_data[p].player_arrows.size(); a++)
  {
    if (current_play_data.current_player_data[p].player_arrows[a].time - current_play_data.song_time < (-1000/8))
    {
      rate_arrow(p, a, 0);
      current_play_data.current_player_data[p].combo = 0;
      current_play_data.current_player_data[p].boo++;
    }
  }
}
