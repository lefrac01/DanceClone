//TODO: worth it to optimize bg by not always redrawing?
//TODO: replace static bg by video

#define ARROWS_HIT_ANIM_MS 200.0

#define NUM_DIFFICULTIES 5

#define SONG_START_OFFSET 100 // an offset to handle the delay between mp3 
    // start request and the time the sound is actually heard
//TODO: fix player start prep
#define PLAYER_PREP_OFFSET 0 // time to prepare before the song starts
    
float pixels_per_ms_at_1_bpm = 0.0; // calculate once screen height is known
// based on the observed rate of 4000ms per screen height per quarter note
// at 120 BPM

int goal_offset = 0;
int gamestate = 0;
int arrow_height = 64;
bool gamestatechange = 0;
int difficulty = 0;
#include "setup/play_logic.h"
#include "setup/play_data.h"
#include "Game_playprep.h"
#include "Game_common.h"
#include "Game_setup.h"
#include "Game_menu.h"
#include "Game_play.h"

play_data current_play_data;

void Game_run()
{
  apply_surface(0,0,backgroundimage,screen,NULL);

  int oldgamestate=gamestate;

  if(gamestate==0)Game_menu_titlescreen();
  else if(gamestate==1)Game_menu_credits();
  else if(gamestate==2)Game_menu_score();
  else if(gamestate==3)Game_menu_songselectp1();
  else if(gamestate==4)Game_menu_songselectp2();
  else if(gamestate==5)Game_menu_stepcreate();
  else if(gamestate==7)Game_playprep();
  else if(gamestate==8)Game_play();
  else if(gamestate==9)Game_menu_score();
  else if(gamestate==10)Game_menu_debug();

  if(oldgamestate==gamestate){
    gamestatechange=0;
  }else{
    gamestatechange=1;
    if (DEBUG_LEVEL >= DEBUG_BASIC)
    {
      log << "new gamestate: " << gamestate << endl;
      // flush log file
      log.close();
      log.open("debug", std::ios_base::app);
    }
  }
}

void game_cleanup()
{
  log.close();
}
