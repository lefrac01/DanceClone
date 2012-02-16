//TODO: load player 2 arrows
//
//TODO: decide wether it is worth it to continue adding stepmania file
// data in the DC format and then maintaining this, or just code 
// stepmania support directly
//
//TODO: decide wether to change arrow loading so that all difficulties
// are loaded in a global arrow array and the play algo uses indices
// into this


void Game_playprep(){
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "Game_playprep() begins" << endl;
    debug_log.close();
  }

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
  
  initarrowstructures();
  while(n_ratings)deleterating(0);
  
  sprintf(temptext,"%s%s%s","Music/",songfilename,".dc");
  ifstream indata;indata.open(temptext);
  int currentdifficulty=-1;
  
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "reading arrow information from indata(" << temptext << ")..." << endl;
    debug_log.close();
  }
  for(int a=0; indata.good() && strcmp(temptext, "<end>") != 0; a++)
  {
    if(currentdifficulty == -1)
    {
      indata >> temptext;
      if(difficulty==0 && charmatchstart((char*)temptext,(char*)"<startbeginner>"))currentdifficulty=0;
      if(difficulty==1 && charmatchstart((char*)temptext,(char*)"<starteasy>"))currentdifficulty=1;
      if(difficulty==2 && charmatchstart((char*)temptext,(char*)"<startmedium>"))currentdifficulty=2;
      if(difficulty==3 && charmatchstart((char*)temptext,(char*)"<starthard>"))currentdifficulty=3;
      if(difficulty==4 && charmatchstart((char*)temptext,(char*)"<startchallenge>"))currentdifficulty=4;
      
      if (DEBUG_LEVEL >= DEBUG_GUTS && currentdifficulty != -1)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found start of difficulty " << currentdifficulty << " data." << endl;
        debug_log.close();
      }
    }
    else if(charmatchstart((char*)temptext,(char*)"<stop>"))
    {
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found end of difficulty " << currentdifficulty << " data." << endl;
        debug_log.close();
      }
      currentdifficulty = -1;
    }
    else
    {
      if (songarrowcount[currentdifficulty]<maxarrows)
      {
        indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].direction);
        indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].time);
        indata >> (songarrows[currentdifficulty][songarrowcount[currentdifficulty]].length);
        if (DEBUG_LEVEL >= DEBUG_GUTS)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "reading arrow # " << songarrowcount[currentdifficulty] << " d=" \
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].direction << " t="\
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].time << " l="\
            << songarrows[currentdifficulty][songarrowcount[currentdifficulty]].length \
            << endl;
          debug_log.close();
        }
        songarrowcount[currentdifficulty]++;
      }
      indata >> temptext;
    }
  }
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done" << endl;
    debug_log.close();
  }
  delete songarrows[songarrowcount[currentdifficulty]];songarrowcount[currentdifficulty]--;
  indata.close();
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "closed input file and sucked back songarrow count to " << songarrowcount[currentdifficulty] << endl;
    debug_log.close();
  }
  
  setupplayerarrows(playerarrows, difficulty);
  
  #ifdef WIN
    Mix_PlayMusic(music,0);
  #endif
  #ifdef WII
    MP3Player_PlayBuffer(buffer, lSize, NULL);
    free(buffer);
  #endif
  
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
  gamestate=8;
}
