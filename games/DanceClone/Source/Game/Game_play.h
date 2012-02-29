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


  // read player controls
  // this function also rates arrows
  //current_play_data.read_player_controls();
//  Game_play_controls();


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
  
  if (DEBUG_LEVEL >= DEBUG_GUTS)
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
  long offscreen_high = current_play_data.viewport_offset + rmode->viHeight - goal_offset;
  long offscreen_low = current_play_data.viewport_offset - arrow_height - goal_offset;
  for (int p = 0; p < current_play_data.num_players; p++)
  {
    player_data& pd = current_play_data.current_player_data[p];
      
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "begin animation player " << p << " arrows. base_arrow = " << pd.base_arrow << endl;
      debug_log.close();
    }
    
    // update first and last visible arrows
    while (pd.next_offscreen_arrow != -1 && pd.arrows[pd.next_offscreen_arrow].ypos < offscreen_high)
    {
      pd.last_visible_arrow = pd.next_offscreen_arrow;
      if (pd.first_visible_arrow == -1)
      {
        pd.first_visible_arrow = pd.last_visible_arrow;
      }
      ++pd.next_offscreen_arrow;
      if (pd.next_offscreen_arrow == pd.num_arrows)
      {
        pd.next_offscreen_arrow = -1;
      }
    }

    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "first_visible_arrow: " << pd.first_visible_arrow << " last_visible_arrow: = " << pd.last_visible_arrow << endl;
      debug_log.close();
    }

    
    // draw arrows from last to first, updating first if it is offscreen
    if (pd.first_visible_arrow != -1)
    {
      for (int a = pd.last_visible_arrow; a >= pd.first_visible_arrow; a--)
      {
        arrow& ar = pd.arrows[a];
        if (ar.ypos <= offscreen_low)
        {
          if (pd.first_visible_arrow == pd.last_visible_arrow)
          {
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              debug_log.open("debug", std::ios_base::app);
              debug_log << "clearing last, first visible " << endl;
              debug_log.close();
            }
            pd.first_visible_arrow = -1;
            pd.last_visible_arrow = -1;
            break;
          }
          else
          {
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              debug_log.open("debug", std::ios_base::app);
              debug_log << "incrementing first visible: " << pd.first_visible_arrow << endl;
              debug_log.close();
            }
            ++pd.first_visible_arrow;
          }
        }
        else
        {
          int screen_ypos = ar.ypos - current_play_data.viewport_offset + goal_offset;
          
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
          
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "draw arrow index:" << a << " screen_ypos:" << screen_ypos << endl;
            debug_log.close();
          }
          
          //TODO: add index for type of arrow (quarter, eighth, hold, etc)
          apply_surface(xpos, screen_ypos, arrowsimage, screen, &arrowsframes[arrows_frames_index]);
          
          /*
          if(posy>0-70 && posy<rmode->viHeight){
            if(pd.arrows[a].length){
              if(pd.arrows[a].direction==0){
                apply_surface(185+10,posy,holdimage,screen,&holdframes[0]);
                for(int b=0;b<floor((double)pd.arrows[a].length/5/35);b++)
                  apply_surface(185+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
                temprect.h = (Uint16)(pd.arrows[a].length/5-floor((double)pd.arrows[a].length/5/35)*35);
                apply_surface(185+10,posy+(int)floor((double)pd.arrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
                apply_surface(185+10,posy+pd.arrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
              if(pd.arrows[a].direction==1){
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
  }
  
  
  //TODO: rating stuff here
  
  
  
  
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
  if(
      WiiButtonsDown[0] & WPAD_BUTTON_HOME
  ||  WiiButtonsDown[0] & WPAD_BUTTON_B
  ||  WiiDash_button(rmode->viWidth-100-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back")
  ){
    MP3Player_Stop();
    gamestate=4;
    
debug_log.close();    
  }
  #endif

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_play() done " << endl;
    debug_log.close();
  }    
}
