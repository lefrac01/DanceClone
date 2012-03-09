//TODO: replace static bg by video

int gamestate = 0;
bool gamestatechange = 0;
int difficulty = 0;
#include "setup/play_logic.h"
#include "setup/play_data.h"
#include "Game_common.h"
#include "Game_setup.h"
#include "Game_menu.h"
#include "Game_playprep.h"
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
  else if(gamestate==7 || gamestate==11)Game_playprep();
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
