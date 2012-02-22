//TODO: disallow home button (WiiDash in this game's context)
// during song play

//TODO: load player 2 arrows
//
//TODO: decide wether it is worth it to continue adding stepmania file
// data in the DC format and then maintaining this, or just code 
// stepmania support directly

void Game_playprep(){
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_playprep() begins" << endl;
    debug_log.close();
  }


  // Initialise music
  
  //char temptext[100];     //oohh.. buh-bye
  string file_line = "";
  
  //sprintf(temptext,"%s%s","Music/",songfilename); 
  string file_path = "Music/";
  file_path += songfilename;

  #ifdef WIN
    music = Mix_LoadMUS(temptext);
  #endif
  #ifdef WII
    ASND_Init();
    MP3Player_Init();
    FILE *BGM = 0;
    long lSize;
    char * buffer;
    //size_t result;
    //BGM = fopen(temptext, "rb");
    BGM = fopen(file_path.c_str(), "rb");
    fseek(BGM, 0, SEEK_END);
    lSize = ftell (BGM);
    rewind(BGM);
    buffer = (char*)malloc(sizeof(char)*lSize);
    //result = fread (buffer,1,lSize,BGM);
    fread(buffer, 1, lSize, BGM);
    fclose(BGM);
  #endif
  
  
  // initialise song data
  // arrows and BPM changes

  // calculate the ration between the y position in virtual coordinates
  // as loaded from the file and actual y positions to be used on-screen
  // based on virtual coordinates of 1ms per pixel
  //float virtual_to_screen_ratio = 1.0;


  
  init_song_data_structures();
  while(n_ratings)deleterating(0);
  
  //sprintf(temptext,"%s%s%s","Music/",songfilename,".dc");
  file_path += ".dc";
  //ifstream indata;indata.open(temptext);
  ifstream indata;indata.open(file_path.c_str());
  int currentdifficulty=-1;
  
  // SM file declares BPM must be defined for beat 0
  bool need_bpm_data = true;  // unfortunately, in DC format BPM logically
      // appears in each difficulty section even though BPM changes must
      // by the same for all difficulties.  therefore only read BPM changes
      // once
  string bpm_tag = "#BPMS:";
  
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "reading arrow information from indata(" << file_path << ")..." << endl;
    debug_log.close();
  }
//  for(int a=0; indata.good() && strcmp(temptext, "<end>") != 0; a++)
  while (indata && file_line.compare("<end>") != 0)
  {
    //indata >> temptext; // must peek
    getline(indata, file_line); // peek

    if (DEBUG_LEVEL >= DEBUG_GUTS)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "getline:" << endl << file_line << endl;
      debug_log.close();
    }

    if(need_bpm_data && file_line.substr(0, bpm_tag.size()) == bpm_tag)
    {
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found #BPMS tag, parsing" << endl;
        debug_log.close();
      }
      
      if (parse_bpms(file_line))
      {
        need_bpm_data = 0;
        
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "beat 0 bpm: " << bpm_changes[0].bpm << endl;
          debug_log.close();
        }
      }
    }
    
    if(currentdifficulty == -1)
    {
      if(difficulty==0 && file_line.compare("<startbeginner>")==0)currentdifficulty=0;
      if(difficulty==1 && file_line.compare("<starteasy>")==0)currentdifficulty=1;
      if(difficulty==2 && file_line.compare("<startmedium>")==0)currentdifficulty=2;
      if(difficulty==3 && file_line.compare("<starthard>")==0)currentdifficulty=3;
      if(difficulty==4 && file_line.compare("<startchallenge>")==0)currentdifficulty=4;

      if (DEBUG_LEVEL >= DEBUG_DETAIL && currentdifficulty != -1)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found start of difficulty " << currentdifficulty << " data." << endl;
        debug_log.close();
      }
    }
    else if(file_line[0] == '<')
    {
      if(file_line.compare("<stop>") == 0)
      {
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "found end of difficulty " << currentdifficulty << " data." << endl;
          debug_log.close();
        }
        currentdifficulty = -1;
      }
    }
    else
    {
      if (need_bpm_data && bpm_changes.size() == 0)
      {
        #ifdef LOG_ERRORS
        error_log.open("errors", std::ios_base::app);
        error_log << "ERROR: missing BPM for beat 0" << endl;
        error_log.close();
        #endif
      }
      else
      {
        int direction = atoi(file_line.c_str());
        long time;
        indata >> time;
        int length;
        indata >> length;
        int type;
        indata >> type;
        
        //TODO: calculate ypos from time here
        long ypos = time; //TEMP
        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "accessing song_arrows[3]..." << endl;
          debug_log.close();
        }
        song_arrows[currentdifficulty].push_back(arrow(direction, time, ypos, length, type));
        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "looks good simona" << endl;
          debug_log.close();
        }
        
        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "reading arrow # " << song_arrows[currentdifficulty].size() << " d=" \
            << song_arrows[currentdifficulty].back().direction << " t="\
            << song_arrows[currentdifficulty].back().time << " y="\
            << song_arrows[currentdifficulty].back().ypos << " l="\
            << song_arrows[currentdifficulty].back().length << " ty="\
            << song_arrows[currentdifficulty].back().type \
            << endl;
          debug_log.close();
        }
        
        // discard closing ---- line
//        indata >> temptext; // will be overwritten by read at loop start
        getline(indata, file_line); // will be overwritten by read at loop start
        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "discarded line:" << endl << file_line << endl;
          debug_log.close();
        }
        // consider discarding the discard ;)
        getline(indata, file_line); // peek

        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "really discarded:" << endl << file_line << endl;
          debug_log.close();
        }
        
      }
    }
  }
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done" << endl;
    debug_log.close();
  }
  indata.close();
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "closed input file" << endl;
    debug_log.close();
  }


  // initialise player's copy of the song arrows
  // according to player difficulty
  
  setup_player_arrows(player_arrows, difficulty);


  // start playing music
  
  #ifdef WIN
    Mix_PlayMusic(music,0);
  #endif
  #ifdef WII
    MP3Player_PlayBuffer(buffer, lSize, NULL);
    free(buffer);
  #endif


  // initialise per-song play data 
  
  longestcombo=0;
  combo=0;
  boo=0;
  good=0;
  perfect=0;

  songstarttime=WDgametime+(SDL_GetTicks()-WDruntime);
  songtime=0;
  current_bpm = 0;
  current_bpm_index = 0;
  if (bpm_changes.size() > 0)
  {
    current_bpm = bpm_changes.front().bpm;
  }
  else
  {
    // game state should not be moving forward.  time to go back to the menu!
  }
  
  upcontrol=0;
  downcontrol=0;
  leftcontrol=0;
  rightcontrol=0;
  
  
  
  // initialise animation and rating variables
  viewport_offset = 0;
  last_visible_arrow = 0;
  first_visible_arrow = 0;
  current_ratable_arrow[0]=current_ratable_arrow[1]=current_ratable_arrow[2]=current_ratable_arrow[3]=0;
  next_ratable_arrow[0]=next_ratable_arrow[1]=next_ratable_arrow[2]=next_ratable_arrow[3]=0;
  

  
  // switch to play state
  gamestate=8;
}
