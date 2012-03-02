//TODO: add second player arrows with player-specific difficulty
//
//TODO: play video in background
//
//TODO: support an assist tick like in stepmania.  not necessary but
// could be really useful for people doing non-synth songs
//
//TODO: draw freeze arrow tails.  start with freezetail graphic
// drawn at ypos of arrow end and clipped up to arrow start.  while
// start not reached, add blits of freezebody clipped up to arrow start.

extern int arrow_height;

#include "play/controls.h"

void Game_play()
{
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    //#log.open("debug", std::ios_base::app);
    log << "Game_play() begins" << endl;
    //#log.close();
  }


  // update 
  current_play_data.frame();
/*
//TODO: fix player start prep
  // if player prep delay has gone by, start music
  if (current_play_data.song_time > 0-SONG_START_OFFSET)
  {
    // start playing music
    
    #ifdef WIN
      Mix_PlayMusic(music,0);
    #endif
    #ifdef WII
      MP3Player_PlayBuffer(mp3_buffer, mp3_lSize, NULL);
      free(mp3_buffer);
    #endif
  }
  
  */
  
  
  // read player controls
  // this function also rates arrows
  //current_play_data.read_player_controls();
//  Game_play_controls();


  // display ratings
  /*
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "applying ratings surfaces" << endl;
    //#log.close();
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
  float beat_fraction = fabs(current_play_data.current_song.beat_ticks[current_play_data.current_beat_tick].beat) - (long)fabs(current_play_data.current_song.beat_ticks[current_play_data.current_beat_tick].beat);
  // if we are before beat 0, correct tick whole calculation 
  if (current_play_data.current_song.beat_ticks[current_play_data.current_beat_tick].beat < 0)
  {
    beat_fraction = 0.99-beat_fraction;
  }
  int tick_whole = 0;
  while (beat_fraction >= 0.25)
  {
    ++tick_whole;
    beat_fraction -= 0.25;
  }
  
log << "cbt:" << current_play_data.current_beat_tick << "tw:" << tick_whole << "  beat_fraction:" <<  beat_fraction << endl; //TEMPLOG: outputting vars anims depend on 
    
  if (tick_whole == 0)
  {
    // first 16th of a quarter note, draw flashing home arrows
    apply_surface(185, goal_offset, home_arrows_image, screen, &arrows_frames[4]);
    apply_surface(250, goal_offset, home_arrows_image, screen, &arrows_frames[0]);
    apply_surface(320, goal_offset, home_arrows_image, screen, &arrows_frames[8]);
    apply_surface(385, goal_offset, home_arrows_image, screen, &arrows_frames[12]);
  }
  else
  {
    // other 15 16ths of quarter, draw normal home arrows
    apply_surface(185, goal_offset, home_arrows_image, screen, &arrows_frames[6]);
    apply_surface(250, goal_offset, home_arrows_image, screen, &arrows_frames[2]);
    apply_surface(320, goal_offset, home_arrows_image, screen, &arrows_frames[10]);
    apply_surface(385, goal_offset, home_arrows_image, screen, &arrows_frames[14]);
  }
  
  //TODO: fix goalline vs goaloffset
  
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    //#log.open("debug", std::ios_base::app);
    log << "lighting home arrows according to pad presses" << endl;
    //#log.close();
  }
  
  
  //TODO: multiplayer
  player_data& pd = current_play_data.current_player_data[0];  
  
  /*
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
    apply_surface(320,goal_offset,arrowsimage,screen,&arrowsframes[6]);
  else if(pd.up_control==1)
    apply_surface(320,goal_offset,arrowsimage,screen,&arrowsframes[10]);
  else
    apply_surface(320,goal_offset,arrowsimage,screen,&arrowsframes[2]);
  if(pd.right_control==0)
    apply_surface(385,goal_offset,arrowsimage,screen,&arrowsframes[7]);
  else if(pd.right_control==1)
    apply_surface(385,goal_offset,arrowsimage,screen,&arrowsframes[11]);
  else
    apply_surface(385,goal_offset,arrowsimage,screen,&arrowsframes[3]);
*/


  // animate arrows
  long offscreen_high = current_play_data.viewport_offset + rmode->viHeight - goal_offset;
  long offscreen_low = current_play_data.viewport_offset - arrow_height - goal_offset;
  for (int p = 0; p < current_play_data.num_players; p++)
  {
    player_data& pd = current_play_data.current_player_data[p];
      
    if (DEBUG_LEVEL >= DEBUG_DETAIL)
    {
      //#log.open("debug", std::ios_base::app);
      log << "begin animation player " << p << " arrows. base_arrow = " << pd.base_arrow << endl;
      //#log.close();
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
      //#log.open("debug", std::ios_base::app);
      log << "first_visible_arrow: " << pd.first_visible_arrow << " last_visible_arrow: = " << pd.last_visible_arrow << endl;
      //#log.close();
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
              //#log.open("debug", std::ios_base::app);
              log << "clearing last, first visible " << endl;
              //#log.close();
            }
            pd.first_visible_arrow = -1;
            pd.last_visible_arrow = -1;
            break;
          }
          else
          {
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              //#log.open("debug", std::ios_base::app);
              log << "incrementing first visible: " << pd.first_visible_arrow << endl;
              //#log.close();
            }
            ++pd.first_visible_arrow;
          }
        }
        else
        {
          //arrows become hidden if the player clears them
          if (!ar.hidden)
          {
            int screen_ypos = ar.ypos - current_play_data.viewport_offset + goal_offset;
            
            int arrows_frames_index = -1;
            int xpos = -1;
            switch (ar.direction)
            {
              case 0: xpos = 185; arrows_frames_index =  4+tick_whole; break;
              case 1: xpos = 250; arrows_frames_index =  0+tick_whole; break;
              case 2: xpos = 320; arrows_frames_index =  8+tick_whole; break;
              case 3: xpos = 385; arrows_frames_index = 12+tick_whole; break;
              default: break;
            }
            
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              //#log.open("debug", std::ios_base::app);
              log << "draw arrow index:" << a << " screen_ypos:" << screen_ypos << " direction:" << ar.direction  << " arrows_frames_index:" << arrows_frames_index  << endl; 
              //#log.close();
            }
            
            //TODO: add index for type of arrow (quarter, eighth, hold, etc)
            SDL_Surface* arrow_bitmap_src = NULL;
            switch(ar.type)
            {
            case NOTE_TYPE_QUARTER:
              arrow_bitmap_src = quarter_arrows_image;
              break;
            case NOTE_TYPE_EIGHTH:
              arrow_bitmap_src = eighth_arrows_image;
              break;
            case NOTE_TYPE_QUARTER_TRIPLET:
              arrow_bitmap_src = quarter_triplet_arrows_image;
              break;
            case NOTE_TYPE_SIXTEENTH:
              arrow_bitmap_src = sixteenth_arrows_image;
              break;
            case NOTE_TYPE_EIGHTH_TRIPLET:
              arrow_bitmap_src = eighth_triplet_arrows_image;
              break;
            case NOTE_TYPE_THIRTYSECOND:
              arrow_bitmap_src = thirtysecond_arrows_image;
              break;
            case NOTE_TYPE_SIXTEENTH_TRIPLET:
              arrow_bitmap_src = sixteenth_triplet_arrows_image;
              break;
            case NOTE_TYPE_SIXTYFOURTH:
              arrow_bitmap_src = sixtyfourth_arrows_image;
              break;
            //NOTE: same graphics used for 96th and 192nd
            case NOTE_TYPE_THIRTYSECOND_TRIPLET:
            case NOTE_TYPE_SIXTYFOURTH_TRIPLET:
              arrow_bitmap_src = sixtyfourth_triplet_arrows_image;
              break;

            default:
              log << "fell to default arrow type.  type=" << ar.type << endl;
              arrow_bitmap_src = quarter_arrows_image;
              break;
            }
            apply_surface(xpos, screen_ypos, arrow_bitmap_src, screen, &arrows_frames[arrows_frames_index]);
            
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
            //#log.open("debug", std::ios_base::app);
            log << "done" << endl;
            //#log.close();
          }
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
  
  
//TODO: fix player start prep
/*  
  #ifdef WII
  if((current_play_data.song_time > 0-SONG_START_OFFSET) && !MP3Player_IsPlaying()){
    MP3Player_Stop();
    gamestate=9;
  }
  #endif
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
  if(
      WiiButtonsDown[0] & WPAD_BUTTON_HOME
  ||  WiiButtonsDown[0] & WPAD_BUTTON_B
  ||  WiiDash_button(rmode->viWidth-100-40,rmode->viHeight-10-40,100,10,0,1,(char*)"Back")
  ){
    MP3Player_Stop();
    gamestate=4;
  }
  #endif

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "Game_play() done " << endl;
    //#log.close();
  }    
}
