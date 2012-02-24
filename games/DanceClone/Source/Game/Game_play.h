//TODO: change scroll speed based on current BPM
//


//TODO: home arrow animations so they pulse on the quarter notes of each beat
//
//TODO: add second player arrows with player-specific difficulty
//
//TODO: animate player arrows in time with beat
//
//TODO: when missed arrows scroll past, they scroll off screen
//
//TODO: move home arrows down toward center of screen
//
//TODO: see about regular hitching that arrows do.  must slide smoothly
//
//TODO: play video in background
//
//TODO: support an assist tick like in stepmania.  not necessary but
// could be really useful for people doing non-synth songs

extern int arrow_height;

#include "play/controls.h"

void Game_play()
{
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_play() begins" << endl;
    debug_log.close();
  }


  // update 
  current_play_data.frame();

  
    
  
  
  
//NOTE: the following scheme is used to display arrows.  all arrows are
// placed in y positions according to their position in the song so that
// they fill a virtual area as wide as the screen and high enough
// to contain all arrows.  as the song plays the screen is scrolled 
// down this area at a rate determined by the current BPM.  the 
// viewport_offset variable retains the current offset from the start of
// the area.  to determine each arrow's screen y position, subtract this
// offset from its virtual y position.  to do this we track the last
// and first arrows visible on screen, updating these each cycle as
// necessary.

  
  




  // read player controls
  // this function also rates arrows
  Game_play_controls();


  // display ratings
  /*
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "applying ratings surfaces" << endl;
    debug_log.close();
  }
  for(int a=0;a<n_ratings;a++){
    apply_surface(ratings[a]->posx,ratings[a]->posy,ratingsimage,screen,&ratingsframes[ratings[a]->type]);
    ratings[a]->posy=ratings[a]->posy-(int)((double)timehaspast/10);
    if(ratings[a]->posy<0-50){
      delete ratings[a];
      for(int b=a; b<n_ratings; b++)ratings[b]=ratings[b+1];
      n_ratings=n_ratings-1;
      a=a-1;
    }
  }
  */
  
  // animate home arrows per user input
  
  
  
  //TODO: fix goalline vs goaloffset
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "lighting home arrows according to pad presses" << endl;
    debug_log.close();
  }
  
  
  //TODO: multiplayer
  player_data& pd = current_play_data.current_player_data[0];  
  
  if(pd.left_control==0)
    apply_surface(185,goal_offset,arrowsimage,screen,&arrowsframes[4]);
  else if(pd.left_control==1)
    apply_surface(185,goal_offset,arrowsimage,screen,&arrowsframes[8]);
  else
    apply_surface(185,goal_offset,arrowsimage,screen,&arrowsframes[0]);
  if(pd.down_control==0)
    apply_surface(250,goal_offset,arrowsimage,screen,&arrowsframes[5]);
  else if(pd.down_control==1)
    apply_surface(250,goal_offset,arrowsimage,screen,&arrowsframes[9]);
  else
    apply_surface(250,goal_offset,arrowsimage,screen,&arrowsframes[1]);
  if(pd.up_control==0)
    apply_surface(rmode->viWidth/2,goal_offset,arrowsimage,screen,&arrowsframes[6]);
  else if(pd.up_control==1)
    apply_surface(rmode->viWidth/2,goal_offset,arrowsimage,screen,&arrowsframes[10]);
  else
    apply_surface(rmode->viWidth/2,goal_offset,arrowsimage,screen,&arrowsframes[2]);
  if(pd.right_control==0)
    apply_surface(385,goal_offset,arrowsimage,screen,&arrowsframes[7]);
  else if(pd.right_control==1)
    apply_surface(385,goal_offset,arrowsimage,screen,&arrowsframes[11]);
  else
    apply_surface(385,goal_offset,arrowsimage,screen,&arrowsframes[3]);



  // animate arrows

  // HMMMMMMMM...
  for (int p = 0; p < current_play_data.num_players; p++)
  {
    player_data& pd = current_play_data.current_player_data[p];
      
    //SDL_Rect temprect={50,0,50,35};   // X,Y,W,H
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "beginning main arrow animation loop with playerbasearrow=" << pd.player_base_arrow << " and playerarrowcount=" << pd.player_arrows.size() << endl;
      debug_log.close();
    }    
    
    if (pd.first_visible_arrow != -1)
    {
      for(unsigned int a = pd.first_visible_arrow; a < pd.player_arrows.size(); a++)
      {
        arrow& ar = pd.player_arrows[a];
        int screen_ypos = ar.ypos - current_play_data.viewport_offset + goal_offset;
        if ((int)a==pd.first_visible_arrow && screen_ypos < (0-goal_offset-arrow_height))
        {
          if (pd.first_visible_arrow+1 < (int)pd.player_arrows.size())
          {
            pd.first_visible_arrow++;
          }
          else
          {
            // no more arrows
            pd.first_visible_arrow = -1;
            break;
          }
        }

        int arrows_frames_index = -1;
        int xpos = -1;
        switch (ar.direction)
        {
          case 0: xpos = 185; arrows_frames_index = 12; break;
          case 1: xpos = 250; arrows_frames_index = 13; break;
          case 2: xpos = 320; arrows_frames_index = 14; break;
          case 3: xpos = 385; arrows_frames_index = 15; break;
          default: break;
        }
        
        //TODO: add index for type of arrow (quarter, eighth, hold, etc)
        apply_surface(xpos, screen_ypos, arrowsimage, screen, &arrowsframes[arrows_frames_index]);
        
        /*
        if(posy>0-70 && posy<rmode->viHeight){
          if(pd.player_arrows[a].length){
            if(pd.player_arrows[a].direction==0){
              apply_surface(185+10,posy,holdimage,screen,&holdframes[0]);
              for(int b=0;b<floor((double)pd.player_arrows[a].length/5/35);b++)
                apply_surface(185+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
              temprect.h = (Uint16)(pd.player_arrows[a].length/5-floor((double)pd.player_arrows[a].length/5/35)*35);
              apply_surface(185+10,posy+(int)floor((double)pd.player_arrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
              apply_surface(185+10,posy+pd.player_arrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
            if(pd.player_arrows[a].direction==1){
              ...
        }
        */
      }
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "done" << endl;
        debug_log.close();
      }    
    }
  }
  
  
  //TODO: rating stuff here
  /*
  if(pd.combo>pd.longest_combo)pd.longest_combo=pd.combo;

  char temptext[100];
  sprintf(temptext,"%d%s",pd.boo," BOO");
  WiiDash_spritetext(rmode->viWidth/2,440-18*3,temptext,2);
  sprintf(temptext,"%d%s",pd.good," GOOD");
  WiiDash_spritetext(rmode->viWidth/2,440-18*2,temptext,2);
  sprintf(temptext,"%d%s",pd.perfect," PERFECT");
  WiiDash_spritetext(rmode->viWidth/2,440-18*1,temptext,2);
  if(pd.combo>2){
    sprintf(temptext,"%d%s",pd.combo," COMBO");
    WiiDash_spritetext(rmode->viWidth/2,440-18*0,temptext,2);
  }
  */
  
  #ifdef WII
  if(!MP3Player_IsPlaying()){
    MP3Player_Stop();
    gamestate=9;
  }
  #endif
  
  
  //TODO: implement song cancel via held button / button combo
  
  #ifdef WIN
  if(WiiDash_button(rmode->viWidth-100-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back")){
    Mix_FreeMusic(music);
    gamestate=4;
  }else if(!Mix_PlayingMusic()){
    Mix_FreeMusic(music);
    gamestate=9;
  }else if(keystate[SDLK_ESCAPE]==2){
    Mix_PauseMusic();
  }else if(Mix_PausedMusic()){Mix_ResumeMusic();}
  #endif

  #ifdef WII
  if(WiiDash_button(rmode->viWidth-100-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back")){
    MP3Player_Stop();
    gamestate=4;
  }
  #endif

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_play() done " << endl;
    debug_log.close();
  }    
}
