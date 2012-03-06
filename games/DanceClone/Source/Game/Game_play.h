//TODO: add second player arrows with player-specific difficulty
//
//TODO: play video in background
//
//TODO: support an assist tick like in stepmania.  not necessary but
// could be really useful for people doing non-synth songs
//
//TODO: remove hardcoding of arrow widths, heights

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
  
  
  //TODO: multiplayer
  player_data& pd = current_play_data.current_player_data[0];  
  
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
  
//log << "cbt:" << current_play_data.current_beat_tick << "tw:" << tick_whole << "  beat_fraction:" <<  beat_fraction << endl; //TEMPLOG: outputting vars anims depend on 
    
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
    // other 15 16ths of quarter, draw lit home arrow only if player presses it
    apply_surface(185, goal_offset, home_arrows_image, screen, &arrows_frames[pd.left_control_down?4:6]);
    apply_surface(250, goal_offset, home_arrows_image, screen, &arrows_frames[pd.down_control_down?0:2]);
    apply_surface(320, goal_offset, home_arrows_image, screen, &arrows_frames[pd.up_control_down?8:10]);
    apply_surface(385, goal_offset, home_arrows_image, screen, &arrows_frames[pd.right_control_down?12:14]);
  }
  
 
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
        //NOTE: hold arrow isn't offscreen until tail is!
        if (ar.ypos+ar.length <= offscreen_low)
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
            
            if (ar.type == NOTE_TYPE_HOLD)
            {
              // freeze arrow has three states, unhit, held and missed
              // the bitmaps are organised so that each state is in a separate
              // block.  the frame index pointer begins at a base value that 
              // points at the correct block for the correct state
              int freeze_tail_frame_index = -1;
              int freeze_body_frame_index = -1;
              int freeze_head_frame_index = -1;
              if (ar.freeze_rating == FREEZE_RATING_FAILED)
              {
                //dark index, failed state
                freeze_tail_frame_index = 4;
                freeze_body_frame_index = 4;
                freeze_head_frame_index = 4;
              }
              else if (ar.rating == RATING_NONE)
              {
                // normal unhit state
                freeze_tail_frame_index = 0;
                freeze_body_frame_index = 0;
                freeze_head_frame_index = 0;
              }
              else
              {
                // being held
                freeze_tail_frame_index = 8;
                freeze_body_frame_index = 8;
                // animate the head based on time since start
                if ((current_play_data.song_time - ar.anim_start_time) % 2)
                {
                  freeze_head_frame_index = 8;
                }
                else
                {
                  freeze_head_frame_index = 12;
                }
              }
              
              // draw tail from bottom up to head
              int current_freeze_top = screen_ypos + 32;  // freeze arrows start from the middle of the head
              int current_freeze_bottom = screen_ypos + 64 + ar.length; // bottom as in bottom of blit rect
              int current_freeze_length = ar.length + 32; // part of the freeze tail is overlapped by the head
              int blit_height = -1; // SDL_BlitSurface uses dest rect as output... just to be safe

              // all freeze bitmaps are indexed according to direction
              freeze_tail_frame_index += ar.direction;
              freeze_body_frame_index += ar.direction;
              freeze_head_frame_index += ar.direction;
              
              
              // if being held, arrows only draw up to the home arrows
              if ((ar.rating != RATING_NONE) && (ar.freeze_rating != FREEZE_RATING_FAILED))
              {
                if (current_freeze_top < goal_offset + 32)
                {
                  current_freeze_length -= goal_offset + 32 - current_freeze_top;
                  current_freeze_top = goal_offset + 32;
                }
              }

              
              // clip to screen - body and tail may be hidden or partial
              if (current_freeze_top < rmode->viHeight)
              {
                SDL_Rect freeze_src_rect = freeze_tail_frames[freeze_tail_frame_index];
                SDL_Rect freeze_dest_rect;
                freeze_dest_rect.x = xpos;
                freeze_dest_rect.w = 64;

                // tail
                if (current_freeze_length >= 64)
                {
                  blit_height = 64;
                  freeze_dest_rect.h = 64;
                  freeze_dest_rect.y = current_freeze_bottom - 64;
                }
                else
                {
                  blit_height = current_freeze_length;
                  freeze_src_rect.h = current_freeze_length;
                  freeze_src_rect.y = 64 - current_freeze_length;
                  freeze_dest_rect.h = current_freeze_length;
                  freeze_dest_rect.y = current_freeze_bottom - current_freeze_length;
                }
                SDL_BlitSurface(freeze_arrows_tail_image, &freeze_src_rect, screen, &freeze_dest_rect);
                current_freeze_bottom -= blit_height;
                current_freeze_length -= blit_height;
                
                // body
                freeze_src_rect = freeze_body_frames[freeze_body_frame_index];
                while (current_freeze_length > 0)
                {
                  // just in case freeze dest rect gets clipped by the blit func (shouldn't happen)
                  freeze_dest_rect.x = xpos;
                  freeze_dest_rect.w = 64;
                  
                  // align blit source so that colours in freeze body line up
                  if (current_freeze_length >= 128)
                  {
                    blit_height = 128;
                    freeze_dest_rect.h = 128;
                    freeze_dest_rect.y = current_freeze_bottom - 128;
                  }
                  else
                  {
                    // partial body.  source rect is bottom-up
                    blit_height = current_freeze_length;
                    freeze_src_rect.h = current_freeze_length;
                    freeze_src_rect.y += 128 - current_freeze_length;
                    freeze_dest_rect.h = current_freeze_length;
                    freeze_dest_rect.y = current_freeze_bottom - current_freeze_length;
                  }
                  SDL_BlitSurface(freeze_arrows_body_image, &freeze_src_rect, screen, &freeze_dest_rect);
                  current_freeze_bottom -= blit_height;
                  current_freeze_length -= blit_height;
                }
              }
              // head
              // if being held, arrow head is drawn over home arrow
              if ((ar.rating != RATING_NONE) && (ar.freeze_rating != FREEZE_RATING_FAILED))
              {
                screen_ypos = goal_offset;
              }
              
              apply_surface(xpos, screen_ypos, freeze_arrows_head_image, screen, &freeze_head_frames[freeze_head_frame_index]);
            }
            else
            {
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
            }            
          }
          else
          {
            // arrow is hidden.  does it have an animation?
            if (ar.anim_start_time != -1)
            {
              if (current_play_data.song_time - ar.anim_start_time > ARROWS_HIT_ANIM_MS)
              {
                ar.anim_start_time = -1;
              }
              else
              {
                int screen_ypos = goal_offset;
                
                int arrows_hit_index = -1;
                int xpos = -1;
                switch (ar.direction)
                {
                  // set up xpos and arrow hit index row
                  case 0: xpos = 185; arrows_hit_index = 8; break;
                  case 1: xpos = 250; arrows_hit_index = 0; break;
                  case 2: xpos = 320; arrows_hit_index = 16; break;
                  case 3: xpos = 385; arrows_hit_index = 24; break;
                  default: break;
                }
                SDL_Surface* anim_bitmap_src = NULL;
                if (ar.type == NOTE_TYPE_HOLD && ar.length != 0)
                {
                  anim_bitmap_src = freeze_hit_image;
                  arrows_hit_index = 0; // only one row of freeze hit anims
                }
                else
                {
                  switch (ar.rating)
                  {
                    case RATING_MARVELLOUS: anim_bitmap_src = marvellous_hit_image; break;
                    case RATING_PERFECT: anim_bitmap_src = perfect_hit_image; break;
                    case RATING_GREAT: anim_bitmap_src = great_hit_image; break;
                    case RATING_GOOD: anim_bitmap_src = good_hit_image; break;
                    default: break;
                  }
                }
                
                // move arrow hit index along row to the right frame of animation
                arrows_hit_index += (int)(7 * ((current_play_data.song_time - ar.anim_start_time) / ARROWS_HIT_ANIM_MS) );
                
                if (anim_bitmap_src)
                {
                  apply_surface(xpos, screen_ypos, anim_bitmap_src, screen, &arrows_hit_frames[arrows_hit_index]);
                }
              }
            }
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
  
  // update base arrow
  for (int i = pd.base_arrow; i < pd.num_arrows; i++)
  {
    if (pd.arrows[i].hidden && pd.arrows[i].anim_start_time == -1)
    {
      pd.base_arrow = i;
    }
    else
    {
      break;
    }
  }

  if(pd.combo>pd.longest_combo)pd.longest_combo=pd.combo;
  char temptext[100];
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
