//TODO: intro delay (with initial bpm-flashing home arrows) to give
// player a moment to get ready.  should to be at least one screen
// worth of time at that normal bpms or else arrows will start on screen

//TODO: load player 2 arrows
//

extern play_data current_play_data;

void Game_playprep(){
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    //#log.open("debug", std::ios_base::app);
    log << "Game_playprep() begins" << endl;
    //#log.close();
  }


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
  

  // initialise play data
  
  //TODO: move to play data
  while(n_ratings)deleterating(0);
  

  if (!current_play_data.init())
  {
    // game state should not be moving forward.  time to go back to the menu!
    free(mp3_buffer);
    gamestate = 3;
  }
  else
  {
/*
//TODO: fix player start prep
quick hack delay song start for player prep
*/
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
