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

int timehaspast;

#include "play/controls.h"

void Game_play(){
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_play() begins" << endl;
    debug_log.close();
  }


  // update synchronisation variables

  timehaspast=songtime;
  songtime=WDgametime-songstarttime;
  timehaspast=songtime-timehaspast;
  
  
  // which beat are we on?
  
  
  // update current bpm
  
  
  // scroll speed per current bpm


  // sample song @ 126 BPM
  // scrolls about 120 pixels per second
  // 1000 / 120 ~= 8.333
  // 
  












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
  song_ms_per_pixel = base_ms_per_pixel / bpm_changes[current_bpm_index].bpm;
  
  // rating distances are pixel distances based on current BPM and screen
  // dimensions
  // boo = number of pixels scrolled after 200 ms 
  boo_rating_distance = 180/song_ms_per_pixel;
  good_rating_distance = 135/song_ms_per_pixel;
  great_rating_distance = 90/song_ms_per_pixel;
  perfect_rating_distance = 45/song_ms_per_pixel;
  marvellous_rating_distance = 23/song_ms_per_pixel;
  
  
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
  
  frame_offset = timehaspast / song_ms_per_pixel;
  viewport_offset += frame_offset;  // right now?
    
  
  
  
  
  
  
  




  // read player controls
  // this function also rates arrows
  Game_play_controls();


  // display ratings
  
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "applying surface ratings" << endl;
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
  
  
  // animate home arrows per user input
  
  //TODO: fix goalline vs goaloffset
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "lighting home arrows according to pad presses" << endl;
    debug_log.close();
  }
  if(leftcontrol==0)
    apply_surface(185,goaloffset,arrowsimage,screen,&arrowsframes[4]);
  else if(leftcontrol==1)
    apply_surface(185,goaloffset,arrowsimage,screen,&arrowsframes[8]);
  else
    apply_surface(185,goaloffset,arrowsimage,screen,&arrowsframes[0]);
  if(downcontrol==0)
    apply_surface(250,goaloffset,arrowsimage,screen,&arrowsframes[5]);
  else if(downcontrol==1)
    apply_surface(250,goaloffset,arrowsimage,screen,&arrowsframes[9]);
  else
    apply_surface(250,goaloffset,arrowsimage,screen,&arrowsframes[1]);
  if(upcontrol==0)
    apply_surface(rmode->viWidth/2,goaloffset,arrowsimage,screen,&arrowsframes[6]);
  else if(upcontrol==1)
    apply_surface(rmode->viWidth/2,goaloffset,arrowsimage,screen,&arrowsframes[10]);
  else
    apply_surface(rmode->viWidth/2,goaloffset,arrowsimage,screen,&arrowsframes[2]);
  if(rightcontrol==0)
    apply_surface(385,goaloffset,arrowsimage,screen,&arrowsframes[7]);
  else if(rightcontrol==1)
    apply_surface(385,goaloffset,arrowsimage,screen,&arrowsframes[11]);
  else
    apply_surface(385,goaloffset,arrowsimage,screen,&arrowsframes[3]);



  // animate arrows

  SDL_Rect temprect={50,0,50,35};   // X,Y,W,H
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "beginning main arrow animation loop with playerbasearrow=" << player_base_arrow << " and playerarrowcount=" << player_arrows.size() << endl;
    debug_log.close();
  }    
  
  // starting from the first unconsumed arrow,
  //TODO: fix goalline vs goaloffset
  for(unsigned int a = player_base_arrow; a < player_arrows.size(); a++)
  {
    // calculate its y position based on songtime
    // this calculation must not take into account the BPM because when
    // BPM changes, the distance between the arrows stays the same
    int posy = (int)(goaloffset + ((player_arrows[a].time - songtime)) /5.0);
    
    // posy is based on goaloffset, there at posy = 0, the arrow is not yet
    // at the top of the screen.  
    
    // the arrows must scroll completely off the screen, so the
    // minimum y position will be 0 - goalline - arrow sprite height
    if(posy>0-70 && posy<rmode->viHeight){
      if(player_arrows[a].length){
        if(player_arrows[a].direction==0){
          apply_surface(185+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)player_arrows[a].length/5/35);b++)
            apply_surface(185+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(player_arrows[a].length/5-floor((double)player_arrows[a].length/5/35)*35);
          apply_surface(185+10,posy+(int)floor((double)player_arrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(185+10,posy+player_arrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
        if(player_arrows[a].direction==1){
          apply_surface(250+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)player_arrows[a].length/5/35);b++)
            apply_surface(250+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(player_arrows[a].length/5-floor((double)player_arrows[a].length/5/35)*35);
          apply_surface(250+10,posy+(int)floor((double)player_arrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(250+10,posy+player_arrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
        if(player_arrows[a].direction==2){
          apply_surface(rmode->viWidth/2+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)player_arrows[a].length/5/35);b++)
            apply_surface(rmode->viWidth/2+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(player_arrows[a].length/5-floor((double)player_arrows[a].length/5/35)*35);
          apply_surface(rmode->viWidth/2+10,posy+(int)floor((double)player_arrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(rmode->viWidth/2+10,posy+player_arrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
        if(player_arrows[a].direction==3){
          apply_surface(385+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)player_arrows[a].length/5/35);b++)
            apply_surface(385+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(player_arrows[a].length/5-floor((double)player_arrows[a].length/5/35)*35);
          apply_surface(385+10,posy+(int)floor((double)player_arrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(385+10,posy+player_arrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
      }
      if(player_arrows[a].direction==0)
        apply_surface(185,posy,arrowsimage,screen,&arrowsframes[12]);
      if(player_arrows[a].direction==1)
        apply_surface(250,posy,arrowsimage,screen,&arrowsframes[13]);
      if(player_arrows[a].direction==2)
        apply_surface(rmode->viWidth/2,posy,arrowsimage,screen,&arrowsframes[14]);
      if(player_arrows[a].direction==3)
        apply_surface(385,posy,arrowsimage,screen,&arrowsframes[15]);
    }
  }
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done" << endl;
    debug_log.close();
  }    

  if(combo>longestcombo)longestcombo=combo;

  char temptext[100];
  sprintf(temptext,"%d%s",boo," BOO");
  WiiDash_spritetext(rmode->viWidth/2,440-18*3,temptext,2);
  sprintf(temptext,"%d%s",good," GOOD");
  WiiDash_spritetext(rmode->viWidth/2,440-18*2,temptext,2);
  sprintf(temptext,"%d%s",perfect," PERFECT");
  WiiDash_spritetext(rmode->viWidth/2,440-18*1,temptext,2);
  if(combo>2){
    sprintf(temptext,"%d%s",combo," COMBO");
    WiiDash_spritetext(rmode->viWidth/2,440-18*0,temptext,2);
  }
  
  #ifdef WII
  if(!MP3Player_IsPlaying()){
    MP3Player_Stop();
    gamestate=9;
  }
  #endif
  
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
