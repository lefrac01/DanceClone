//TODO: jumps only count as 1 for combos and points...

//TODO: still scroll screen and arrows during intro delay so arrows
// don't pop up already on-screen

extern play_data current_play_data;

void Game_playprep(){
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    //#log.open("debug", std::ios_base::app);
    log << "Game_playprep() begins" << endl;
    //#log.close();
  }

  if (gamestate == 7)
  {
    // Initialise music
    
    string file_line = "";
    
    #ifdef WIN
      music = Mix_LoadMUS(current_song.path());
    #endif
    #ifdef WII
      ASND_Init();
      MP3Player_Init();
      FILE *BGM = 0;
      //long lSize;
      //size_t result;
      //BGM = fopen(temptext, "rb");
      BGM = fopen(current_play_data.current_song.path().c_str(), "rb");
      fseek(BGM, 0, SEEK_END);
      mp3_lSize = ftell (BGM);
      rewind(BGM);
      mp3_buffer = (char*)malloc(sizeof(char)*mp3_lSize);
      //result = fread (buffer,1,lSize,BGM);
      fread(mp3_buffer, 1, mp3_lSize, BGM);
      fclose(BGM);
    #endif
    
    gamestate = 11; // ugly but soooo effective :P
  }  
  
  //graphics.draw_bg();
  //graphics.draw_home_arrows();
  //TEMP: ripped code from drawing routine to at least show home arrows during pre-play pause
  // other 15 16ths of quarter, draw lit home arrow only if player presses it
  apply_surface(185, goal_offset, home_arrows_image, screen, &arrows_frames[6]);
  apply_surface(250, goal_offset, home_arrows_image, screen, &arrows_frames[2]);
  apply_surface(320, goal_offset, home_arrows_image, screen, &arrows_frames[10]);
  apply_surface(385, goal_offset, home_arrows_image, screen, &arrows_frames[14]);
  
  // getready.png 300 x 120
  apply_surface(rmode->viWidth/2 - 300/2, rmode->viHeight/2 - 120/2, getreadyimage, screen, NULL);
  
  if (current_play_data.pre_start_delay_finished())
  {
    if (!current_play_data.init())
    {
      // game state should not be moving forward.  time to go back to the menu!
      free(mp3_buffer);
      gamestate = 3;
    }
    else
    {
      // start playing music
      #ifdef WIN
        Mix_PlayMusic(music,0);
      #endif
      #ifdef WII
        MP3Player_PlayBuffer(mp3_buffer, mp3_lSize, NULL);
        free(mp3_buffer);
      #endif

      // synchronise play data just after mp3 code has had a chance
      // to finish beginning play
      current_play_data.initial_frame();
      
      // switch to play state
      gamestate = 8;
    }
  }
}
