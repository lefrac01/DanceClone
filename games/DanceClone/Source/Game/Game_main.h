//TODO: worth it to optimize bg by not always redrawing?
//TODO: replace static bg by video

#define NOTE_TYPE_QUARTER 1
#define NOTE_TYPE_EIGHTH 2
#define NOTE_TYPE_HOLD 3

int gamestate = 0;
bool gamestatechange = 0;
int difficulty = 0;
#include "Game_common.h"
#include "Game_setup.h"
#include "Game_menu.h"
#include "Game_playprep.h"
#include "Game_play.h"

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
  else if(gamestate==6)Game_menu_stepimport();
  else if(gamestate==7)Game_playprep();
  else if(gamestate==8)Game_play();
  else if(gamestate==9)Game_menu_score();

  if(oldgamestate==gamestate){
    gamestatechange=0;
  }else{
    gamestatechange=1;
    if (DEBUG_LEVEL >= DEBUG_BASIC)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "new gamestate: " << gamestate << endl;
      debug_log.close();
    }
  }

}

void game_cleanup()
{
  //#free_song_data_structures();
}
