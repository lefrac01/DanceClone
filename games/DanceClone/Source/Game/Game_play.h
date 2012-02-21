//TODO: change scroll speed based on current BPM
//


//TODO: home arrow animations so they pulse on the beat
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
    debug_log << "beginning main arrow animation loop with playerbasearrow=" << playerbasearrow << " and playerarrowcount=" << playerarrowcount << endl;
    debug_log.close();
  }    
  
  // starting from the first unconsumed arrow,
  //TODO: fix goalline vs goaloffset
  for(int a = playerbasearrow; a < playerarrowcount; a++)
  {
    // calculate its y position based on songtime
    // this calculation must not take into account the BPM because when
    // BPM changes, the distance between the arrows stays the same
    int posy = (int)(goaloffset + ((playerarrows[a].ypos - songtime)) /5.0);
    
    // posy is based on goaloffset, there at posy = 0, the arrow is not yet
    // at the top of the screen.  
    
    // the arrows must scroll completely off the screen, so the
    // minimum y position will be 0 - goalline - arrow sprite height
    if(posy>0-70 && posy<rmode->viHeight){
      if(playerarrows[a].length){
        if(playerarrows[a].direction==0){
          apply_surface(185+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)playerarrows[a].length/5/35);b++)
            apply_surface(185+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(playerarrows[a].length/5-floor((double)playerarrows[a].length/5/35)*35);
          apply_surface(185+10,posy+(int)floor((double)playerarrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(185+10,posy+playerarrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
        if(playerarrows[a].direction==1){
          apply_surface(250+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)playerarrows[a].length/5/35);b++)
            apply_surface(250+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(playerarrows[a].length/5-floor((double)playerarrows[a].length/5/35)*35);
          apply_surface(250+10,posy+(int)floor((double)playerarrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(250+10,posy+playerarrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
        if(playerarrows[a].direction==2){
          apply_surface(rmode->viWidth/2+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)playerarrows[a].length/5/35);b++)
            apply_surface(rmode->viWidth/2+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(playerarrows[a].length/5-floor((double)playerarrows[a].length/5/35)*35);
          apply_surface(rmode->viWidth/2+10,posy+(int)floor((double)playerarrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(rmode->viWidth/2+10,posy+playerarrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
        if(playerarrows[a].direction==3){
          apply_surface(385+10,posy,holdimage,screen,&holdframes[0]);
          for(int b=0;b<floor((double)playerarrows[a].length/5/35);b++)
            apply_surface(385+10,posy+35*b+35,holdimage,screen,&holdframes[1]);
          temprect.h = (Uint16)(playerarrows[a].length/5-floor((double)playerarrows[a].length/5/35)*35);
          apply_surface(385+10,posy+(int)floor((double)playerarrows[a].length/5/35)*35+35,holdimage,screen,&temprect);
          apply_surface(385+10,posy+playerarrows[a].length/5+35,holdimage,screen,&holdframes[2]);}
      }
      if(playerarrows[a].direction==0)
        apply_surface(185,posy,arrowsimage,screen,&arrowsframes[12]);
      if(playerarrows[a].direction==1)
        apply_surface(250,posy,arrowsimage,screen,&arrowsframes[13]);
      if(playerarrows[a].direction==2)
        apply_surface(rmode->viWidth/2,posy,arrowsimage,screen,&arrowsframes[14]);
      if(playerarrows[a].direction==3)
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
