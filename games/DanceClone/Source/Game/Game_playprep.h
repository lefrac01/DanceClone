//TODO: disallow home button (WiiDash in this game's context)
// during song play

//TODO: load player 2 arrows
//

extern play_data current_play_data;

void Game_playprep(){
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_playprep() begins" << endl;
    debug_log.close();
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
    long lSize;
    char * buffer;
    //size_t result;
    //BGM = fopen(temptext, "rb");
    BGM = fopen(current_play_data.current_song.path().c_str(), "rb");
    fseek(BGM, 0, SEEK_END);
    lSize = ftell (BGM);
    rewind(BGM);
    buffer = (char*)malloc(sizeof(char)*lSize);
    //result = fread (buffer,1,lSize,BGM);
    fread(buffer, 1, lSize, BGM);
    fclose(BGM);
  #endif
  

  // initialise play data
  
  //TODO: move to play data
  while(n_ratings)deleterating(0);
  

  if (!current_play_data.init())
  {
    // game state should not be moving forward.  time to go back to the menu!
    gamestate = 3;
  }
  else
  {

    // start playing music
    
    #ifdef WIN
      Mix_PlayMusic(music,0);
    #endif
    #ifdef WII
      MP3Player_PlayBuffer(buffer, lSize, NULL);
      free(buffer);
    #endif

    
    // switch to play state
    gamestate = 8;
  }
}
