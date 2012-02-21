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
  
  char temptext[100]; 
  sprintf(temptext,"%s%s","Music/",songfilename); 

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
    BGM = fopen(temptext, "rb");
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
  float virtual_to_screen_ratio = 1.0;


  
  initsongdatastructures();
  while(n_ratings)deleterating(0);
  
  sprintf(temptext,"%s%s%s","Music/",songfilename,".dc");
  ifstream indata;indata.open(temptext);
  int currentdifficulty=-1;
  
  // SM file declares BPM must be defined for beat 0
  bool need_bpm_data = true;  // unfortunately, in DC format BPM logically
      // appears in each difficulty section even though BPM changes must
      // by the same for all difficulties.  therefore only read BPM changes
      // once
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "reading arrow information from indata(" << temptext << ")..." << endl;
    debug_log.close();
  }
  for(int a=0; indata.good() && strcmp(temptext, "<end>") != 0; a++)
  {
    indata >> temptext; // must peek
    if(currentdifficulty == -1)
    {
      if(difficulty==0 && charmatchstart((char*)temptext,(char*)"<startbeginner>"))currentdifficulty=0;
      if(difficulty==1 && charmatchstart((char*)temptext,(char*)"<starteasy>"))currentdifficulty=1;
      if(difficulty==2 && charmatchstart((char*)temptext,(char*)"<startmedium>"))currentdifficulty=2;
      if(difficulty==3 && charmatchstart((char*)temptext,(char*)"<starthard>"))currentdifficulty=3;
      if(difficulty==4 && charmatchstart((char*)temptext,(char*)"<startchallenge>"))currentdifficulty=4;
      
      if (DEBUG_LEVEL >= DEBUG_DETAIL && currentdifficulty != -1)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found start of difficulty " << currentdifficulty << " data." << endl;
        debug_log.close();
      }
    }
    else if(temptext[0]=='<')
    {
      if(need_bpm_data && charmatchstart((char*)temptext,(char*)"<bpm>"))
      {
        // WHAT IS NECESSARY HERE?
        // this is incoming from the step data source file
        // there will be many of these
        // must load them all ahead of time
        // to know when to use each, the songtime at which
        // each is applicable must also exist
        // format:
        // <bpm:ms_offset=float_bpm>
        //
        // err.. can better be done?  think simple you fool
        // <bpm>EOL
        // ms_offsetEOL
        // float_bpmEOL
        // ----
        //
        // this is a way to cease failing.  think first!!! concept alert!
        indata >> bpm_changes[number_bpm_changes].ypos;
        bpm_changes[number_bpm_changes].ypos *= virtual_to_screen_ratio;
        indata >> bpm_changes[number_bpm_changes].bpm;
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "found bpm change indication number " << number_bpm_changes << " to " << bpm_changes[number_bpm_changes].bpm << " BPM at position " << bpm_changes[number_bpm_changes].ypos << endl;
          debug_log.close();
        }
        number_bpm_changes++;
        
        // discard closing ---- line
        indata >> temptext; // will be overwritten by read at loop start
      }
      else if(charmatchstart((char*)temptext,(char*)"<stop>"))
      {
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "found end of difficulty " << currentdifficulty << " data." << endl;
          debug_log.close();
        }
        currentdifficulty = -1;
        if (need_bpm_data)
        {
          if (number_bpm_changes == 0)
          {
            #ifdef LOG_ERRORS
            error_log.open("errors", std::ios_base::app);
            error_log << "ERROR: missing BPM data" << endl;
            error_log.close();
            #endif
          }
          else
          {
            // found our required BPM data.  don't look for other BPM
            // data.  it's there but should be identical
            need_bpm_data = false;
          }
        }
        number_bpm_changes = 0;
      }
    }
    else
    {
      if (need_bpm_data && number_bpm_changes == 0)
      {
        #ifdef LOG_ERRORS
        error_log.open("errors", std::ios_base::app);
        error_log << "ERROR: missing BPM marker for beat 0" << endl;
        error_log.close();
        #endif
      }
      else if (songarrowcount[currentdifficulty]<maxarrows)
      {
        //peeking now
        //indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].direction);
        songarrows[currentdifficulty][songarrowcount[currentdifficulty]].direction = atoi(temptext);
        
        indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].ypos);
        songarrows[currentdifficulty][songarrowcount[currentdifficulty]].ypos = 
          songarrows[currentdifficulty][songarrowcount[currentdifficulty]].ypos * virtual_to_screen_ratio;
          
        indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].length);
        songarrows[currentdifficulty][songarrowcount[currentdifficulty]].length *= virtual_to_screen_ratio;
        
        indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].type);
        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "reading arrow # " << songarrowcount[currentdifficulty] << " d=" \
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].direction << " y="\
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].ypos << " l="\
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].length \
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].type \
            << endl;
          debug_log.close();
        }
        songarrowcount[currentdifficulty]++;
        
        // discard closing ---- line
        indata >> temptext; // will be overwritten by read at loop start
        // consider discarding the discard ;)
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
  
  setupplayerarrows(playerarrows, difficulty);


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
  song_ms_per_pixel = base_ms_per_pixel / bpm_changes[0].bpm;
  
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
  
  
  
  // switch to play state
  gamestate=8;
}
