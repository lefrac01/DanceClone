//NOTE: stepmania file format at http://www.stepmania.com/wiki/The_.SM_file_format
//
//TODO: implement #STOPS:
//
//TODO: put #BPMS; at top of file because it doesn't make sense to
// repeat it for each of the difficulties.
// it can just be a copy of the one from the SM file.
// reading the SM file directly just seems easier and easier
//
//TODO: rip out concept of max line length.  



// maxlinelength is 10000 chars.  one BPM definition requires
// about 10 chars so max ~1000 definitions.  
const int maxBPMS = 1000;
int*  BPMSindices = 0;
float* BPMS = 0;
long*  BPMchangepositions = 0;
int   current_BPM_index = 0;
int   numBPMS = 0;
int   BPM_output_index = 0;


//NOTE:
// awwww crap.  the whole original algo is step-based
// and I am trying to immitate the SM beat-based format
// by adding bpm changes at specific timestamps.
// time might be saved here by seriously reconsidering the
// entire file format question of SM vs DC...
void writesteps(ofstream &outdata)
{
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "writesteps() difficulty:" << difficulty << " number of arrows:" << songarrowcount[difficulty] << endl;
    debug_log.close();
  }
  #endif

  
  if(songarrowcount[difficulty])
  {
    #ifdef LOG_IMPORT
    if (DEBUG_LEVEL >= DEBUG_MINOR)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "writing difficulty" << endl;
      debug_log.close();
    }
    #endif
    
    if(difficulty == 0) outdata << "<startbeginner>" << endl;
    if(difficulty == 1) outdata << "<starteasy>" << endl;
    if(difficulty == 2) outdata << "<startmedium>" << endl;
    if(difficulty == 3) outdata << "<starthard>" << endl;
    if(difficulty == 4) outdata << "<startchallenge>" << endl;

    #ifdef LOG_IMPORT
    if (DEBUG_LEVEL >= DEBUG_MINOR)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "processing " << songarrowcount[difficulty] << " arrows." << endl;
      debug_log.close();
    }
    #endif
    
    for(int a = 0; a < songarrowcount[difficulty]; a++)
    {
      // check if there is a bpm block to insert before the current arrow
      if (  BPM_output_index < numBPMS 
        &&  BPMchangepositions[BPM_output_index] <=  songarrows[difficulty][a].ypos)
      {
        outdata << "<bpm>" << endl;
        outdata << BPMchangepositions[BPM_output_index] << endl;
        outdata << BPMS[BPM_output_index] << endl;
        outdata << "----" << endl;
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "wrote bpm block for timestamp, beat: " \
            << BPMchangepositions[BPM_output_index] << "," \
            << BPMS[BPM_output_index] << endl;
          debug_log.close();
        }
        #endif
        BPM_output_index++;
      }
      
      outdata << songarrows[difficulty][a].direction << endl;
      outdata << songarrows[difficulty][a].ypos << endl;
      outdata << songarrows[difficulty][a].length << endl;
      outdata << songarrows[difficulty][a].type << endl;
      outdata << "----" << endl;
    }
    outdata << "<stop>" << endl;
    
    #ifdef LOG_IMPORT
    if (DEBUG_LEVEL >= DEBUG_MINOR)
    {
      debug_log.open("debug", std::ios_base::app);
      debug_log << "deleting arrows" << endl;
      debug_log.close();
    }
    #endif
    initsongdatastructures();
  }
}

void Game_menu_stepimport_sm(char* filename)
{
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "beginning Game_menu_stepimport_sm(" << filename << ")" << endl;
    debug_log.close();
  }
  #endif

  ofstream outdata;
#ifdef WII
  ifstream indata;
#endif
#ifdef WIN
  FILE * indata;
#endif

  const int maxlinelength = 10000;
  char* temptext = 0;
  char* newtemp = 0;
  const int lastlineslength = 70;
  char** lastlines;

  int   current_beat = 0;
  double tempBPMS = 0;
  
//  double tempoffset = 0;
//  double currenttime = 0;   // this concept is replaced with current pos
  long beat0offset = 0;
  long currentpos = 0;
  double notetimevalue = 0;
  int tempint1 = 0;
  int tempint2 = 0;
  bool placeingnotes = 0;
  bool in_dance_single_block = 0;

  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "wiping working mem..." << endl;
    debug_log << "temptext..." << endl;
    debug_log.close();
  }
  #endif
  temptext = (char*)malloc(maxlinelength + 10);
  memset(temptext, 0, sizeof(temptext));

  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "newtemp..." << endl;
    debug_log.close();
  }
  #endif
  newtemp = (char*)malloc(maxlinelength + 10);
  memset(newtemp, 0, maxlinelength + 10);

  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "lastlines array..." << endl;
    debug_log.close();
  }
  #endif
  lastlines = (char **) malloc(lastlineslength * sizeof(char *));
  for (int i = 0; i < lastlineslength; i++)
  {
    lastlines[i] = (char*)malloc(maxlinelength + 10);
    memset(lastlines[i], 0, sizeof(lastlines[i]));
  }

  BPMSindices = (int*)malloc(maxBPMS * sizeof(int));
  BPMchangepositions = (long*)malloc(maxBPMS * sizeof(long));
  BPMS = (float*)malloc(maxBPMS * sizeof(float));
  
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "BPMS arrays..." << endl;
    debug_log.close();
  }
  #endif

  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done" << endl;
    debug_log.close();
  }
  #endif

  sprintf(temptext, "%s%s%s", "Music/", songfilename, ".dc");
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "outdata.open(" << temptext << ")" << endl;
    debug_log.close();
  }
  #endif
  outdata.open(temptext);
  
  sprintf(temptext, "%s%s", "Music/", filename);
#ifdef WIN
  indata = fopen(temptext, "r");
#endif
#ifdef WII
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "indata.open(" << temptext << ")" << endl;
    debug_log.close();
  }
  #endif
  indata.open(temptext);
#endif
  
#ifdef WIN
  while(fgets(newtemp, maxlinelength, indata)) if(strlen(newtemp) > 1)
  {
#endif
#ifdef WII
  while(indata.good())
  {
#endif
    // instead of copying contents, now that the lastlines buffer
    // is on the heap, just shift the pointers in the first dimension
    // of the array
    char* temp = lastlines[lastlineslength-1]; // shifting onto this one
    for (int a = lastlineslength-1; a > 0; a--)
    {
      lastlines[a] = lastlines[a-1];
    }
    lastlines[0] = temp; // put saved pointer back at other end
    
#ifdef WIN
    newtemp[strlen(newtemp) - 1] = '\0';
#endif
#ifdef WII
    indata >> newtemp;
#endif
    sprintf(lastlines[0],"%s",newtemp);


    //NOTE: sample and definition of SM BPMS tag
    //#BPMS:0=140.215,1=140.208,14=140.215,16=140.208,43=140.215;
    //#BPMS:<int beat>=<float BPM>[,<int beat>=<float BPM>];
    //beat 0 must be defined
    if(charmatchstart(lastlines[0],(char*)"#BPMS:"))
    {
      unsigned int linelength = strlen(lastlines[0]);
      for(unsigned int offset=0; offset<linelength; offset++)
      {
        if (lastlines[0][offset]==':' || lastlines[0][offset]==',')
        {
          tempint1 = offset+1;
          tempint2 = -1;
          for(unsigned int a=tempint1; a<linelength && tempint2==-1; a++)
          {
            if(lastlines[0][a]=='=')
            {
              tempint2 = a;
            }
          }
          if(tempint2!=-1) // found an int describing the beat to apply the BPM
          {
            for(int a=0; a<tempint2-tempint1; a++)
            {
              temptext[a]=lastlines[0][tempint1+a];
            }
            temptext[tempint2-tempint1]=0;
            int temp_beat = atoi(temptext);

            #ifdef LOG_IMPORT
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              debug_log.open("debug", std::ios_base::app);
              debug_log << "extracted int beat: " << temp_beat << " from text (" << temptext << ")" << endl;
              debug_log.close();
            }
            #endif
            
            // continue past '=' looking for BPM data
            tempint1 = tempint2+1;
            tempint2 = -1;
            for(unsigned int a=tempint1; a<linelength && tempint2==-1; a++)
            {
              if(lastlines[0][a]==',' || lastlines[0][a]==';')
              {
                tempint2 = a;
              }
            }
            if(tempint2!=-1) // found a float describing the BPM
            {
              for(int a=0; a<tempint2-tempint1; a++)
              {
                temptext[a]=lastlines[0][tempint1+a];
              }
              temptext[tempint2-tempint1]=0;
              float temp_BPM = atof(temptext);
              
              #ifdef LOG_IMPORT
              if (DEBUG_LEVEL >= DEBUG_DETAIL)
              {
                debug_log.open("debug", std::ios_base::app);
                debug_log << "extracted float BPM: " << temp_BPM << " from text (" << temptext << ")" << endl;
                debug_log.close();
              }
              #endif
              
              if (numBPMS < maxBPMS)
              {
                BPMSindices[numBPMS] = temp_beat;
                BPMS[numBPMS] = temp_BPM;
                numBPMS++;
              }
              else
              {
                #ifdef LOG_ERRORS
                error_log.open("errors", std::ios_base::app);
                error_log << "ERROR: too many BPMS changes! (more than " << maxBPMS << ")" << endl;
                error_log.close();
                #endif
              }
            }
            else
            {
              #ifdef LOG_ERRORS
              error_log.open("errors", std::ios_base::app);
              error_log << "ERROR: invalid BPMS data!" << endl;
              error_log.close();
              #endif
            }
          }
          else
          {
            #ifdef LOG_ERRORS
            error_log.open("errors", std::ios_base::app);
            error_log << "ERROR: invalid BPMS data!" << endl;
            error_log.close();
            #endif
          }
        }
      }
      #ifdef LOG_ERRORS
      if (numBPMS == 0)
      {
        error_log.open("errors", std::ios_base::app);
        error_log << "ERROR: failed to extract BPMS!" << endl;
        error_log.close();
      }
      #endif
    }
    if(charmatchstart(lastlines[0],(char*)"#OFFSET:")){
      #ifdef LOG_IMPORT
      if (DEBUG_LEVEL >= DEBUG_MINOR)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "charmatchstart on lastlines[0].  found #OFFSET:" << endl;
        debug_log << "lastlines[0]=(" << lastlines[0] << ")" << endl;
        debug_log.close();
      }
      #endif
      
      tempint1=-1;tempint2=-1;
      for(unsigned int a=0; a<strlen(lastlines[0]); a++){
        if(lastlines[0][a]==':')tempint1=a+1;}
      for(unsigned int a=0; a<strlen(lastlines[0]); a++){
        if(lastlines[0][a]==';')tempint2=a+0;}
      if(tempint1!=-1 && tempint2!=-1)
      {
        for(int a=0; a<tempint2-tempint1; a++){
          temptext[a]=lastlines[0][a+tempint1];
          temptext[a+1]='\n';}

        beat0offset=(long)(atof(temptext)*1000.0);
        if (beat0offset < 0) beat0offset = -beat0offset; // current stepmania format is negative float offset in seconds
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "extracted beat 0 offset:" << beat0offset << " from text " << temptext << endl;
          debug_log.close();
        }
        #endif
      }
      else
      {
        #ifdef LOG_ERRORS
        error_log.open("errors", std::ios_base::app);
        error_log << "ERROR: failed to extract offset!" << endl;
        error_log.close();
        #endif
      }
    }
    if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"dance-single:")){
      #ifdef LOG_IMPORT
      if (DEBUG_LEVEL >= DEBUG_MINOR)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "detected dance-single block" << endl;
        debug_log.close();
      }
      #endif
      in_dance_single_block=1;
    }
    if (in_dance_single_block)
    {
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Beginner:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Beginner" << endl;
          debug_log.close();
        }
        #endif
        initsongdatastructures();
        placeingnotes=1;difficulty=0;
        currentpos=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Easy:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Easy" << endl;
          debug_log.close();
        }
        #endif
        initsongdatastructures();
        placeingnotes=1;difficulty=1;
        currentpos=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Medium:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Medium" << endl;
          debug_log.close();
        }
        #endif
        initsongdatastructures();
        placeingnotes=1;difficulty=2;
        currentpos=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Hard:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Hard" << endl;
          debug_log.close();
        }
        #endif
        initsongdatastructures();
        placeingnotes=1;difficulty=3;
        currentpos=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Challenge:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Challenge" << endl;
          debug_log.close();
        }
        #endif
        initsongdatastructures();
        placeingnotes=1;difficulty=4;
        currentpos=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
    }
    if(placeingnotes){
      #ifdef LOG_IMPORT
      if (DEBUG_LEVEL >= DEBUG_GUTS)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "placing notes..." << endl;
        debug_log.close();
      }
      #endif
      if((lastlines[0][0]==','||lastlines[0][0]==';') && strlen(lastlines[1])==4){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "detected beat separator (" << lastlines[0][0] << ")" << endl;
          debug_log.close();
        }
        #endif
        
        // set BPM according to current beat and list of BPMs that apply to each beat
        if (BPMSindices[current_BPM_index] == current_beat)
        {
          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "beat change detected on beat " << current_beat << " new bpm is at index " << current_BPM_index << endl;
            debug_log.close();
          }
          #endif
          
          // there is a BPM change on this beat.
          tempBPMS = BPMS[current_BPM_index];
          
          // now that the songtime of the current beat is calculated,
          // save it for output of bpm data
          BPMchangepositions[current_BPM_index] = currentpos;
          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "using currentpos " << currentpos << " for position of bpm change on beat " << current_beat << endl;
            debug_log.close();
          }
          #endif

          // move index to next index (if there is one!)
          if (current_BPM_index < maxBPMS)
          {
            current_BPM_index++;
          }
        }
        
        notetimevalue=0;
        for(int a=1; a<lastlineslength; a++){
          if(strlen(lastlines[a])!=4){  // could start for at a=2 since strlen(lastlines[1])==4
            notetimevalue=1/(double)(a-1);  // x4 ?
            #ifdef LOG_IMPORT
            if (DEBUG_LEVEL >= DEBUG_DETAIL)
            {
              debug_log.open("debug", std::ios_base::app);
              debug_log << "calculated note time value: " << notetimevalue \
                << " by searching back " << a << " rows" << endl;
              debug_log.close();
            }
            #endif
            a=500;}
        }

        int current_ticks_per_beat = (int)(1/notetimevalue);
        for(int a=current_ticks_per_beat; a>0; a--){   
          // no, no.. do this after assigning values to the current notes!
          //#currenttime=currenttime+notetimevalue*1000*60/tempBPMS;

          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "reading note data in block of size: " << 1/notetimevalue << endl;
            debug_log.close();
          }
          #endif

          //NOTE: this logic moves away from a beat-based data block
          //format to an arrow-based data block format.  implications??
          int note_type = -1;
          if (current_ticks_per_beat == 4)
          {
            note_type = NOTE_TYPE_QUARTER;
          }
          //else if (current_ticks_per_beat == 8)
          //MEGAAAAHSIMPUULLLLL
          else 
          {
            note_type = NOTE_TYPE_EIGHTH;
          }
          
          if(lastlines[a][0]=='1')assignsongarrow(difficulty,0,currentpos,0,note_type);
          if(lastlines[a][1]=='1')assignsongarrow(difficulty,1,currentpos,0,note_type);
          if(lastlines[a][2]=='1')assignsongarrow(difficulty,2,currentpos,0,note_type);
          if(lastlines[a][3]=='1')assignsongarrow(difficulty,3,currentpos,0,note_type);
          if(lastlines[a][0]=='2')assignsongarrow(difficulty,0,currentpos,0,NOTE_TYPE_HOLD);
          if(lastlines[a][1]=='2')assignsongarrow(difficulty,1,currentpos,0,NOTE_TYPE_HOLD);
          if(lastlines[a][2]=='2')assignsongarrow(difficulty,2,currentpos,0,NOTE_TYPE_HOLD);
          if(lastlines[a][3]=='2')assignsongarrow(difficulty,3,currentpos,0,NOTE_TYPE_HOLD);
          
          if(lastlines[a][0]=='3' && songarrowcount[difficulty]<maxarrows)
            for(int b=songarrowcount[difficulty]-1; b>0; b--)if(songarrows[difficulty][b].direction==0){
              songarrows[difficulty][b].length=(int)currentpos-songarrows[difficulty][b].ypos;b=0;}
          if(lastlines[a][1]=='3' && songarrowcount[difficulty]<maxarrows)
            for(int b=songarrowcount[difficulty]-1; b>0; b--)if(songarrows[difficulty][b].direction==1){
              songarrows[difficulty][b].length=(int)currentpos-songarrows[difficulty][b].ypos;b=0;}
          if(lastlines[a][2]=='3' && songarrowcount[difficulty]<maxarrows)
            for(int b=songarrowcount[difficulty]-1; b>0; b--)if(songarrows[difficulty][b].direction==2){
              songarrows[difficulty][b].length=(int)currentpos-songarrows[difficulty][b].ypos;b=0;}
          if(lastlines[a][3]=='3' && songarrowcount[difficulty]<maxarrows)
            for(int b=songarrowcount[difficulty]-1; b>0; b--)if(songarrows[difficulty][b].direction==3){
              songarrows[difficulty][b].length=(int)currentpos-songarrows[difficulty][b].ypos;b=0;}

          
          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "updating current pos " << currentpos << " to " << currentpos+notetimevalue*1000*60/tempBPMS << " on beat " << current_beat << endl;
            debug_log.close();
          }
          #endif
          
          //TODO: redo position based on a static rate of seconds / virtual unit
          // the current calc is the same as 1 ms / virtual unit.
          currentpos+=notetimevalue*1000*60/tempBPMS;
        }
        
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "done processing beat " << current_beat << endl;
          debug_log.close();
        }
        #endif
        // the beat has been processed.
        current_beat++;
      }
      if(lastlines[0][0]==';')
      {
        placeingnotes = 0;
        in_dance_single_block = 0;
        writesteps(outdata);
      }
    }
  }
  writesteps(outdata);
  #ifdef WII
  indata.close();
  #endif
  outdata << "<end>";
  outdata.close();

  free(temptext);
  free(newtemp);
  for (int i=0; i<lastlineslength; i++)
  {
    free(lastlines[i]);
  }
  free(lastlines);
  
  free(BPMSindices);
  free(BPMchangepositions);
  free(BPMS);
  
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "leaving step import" << endl;
    debug_log.close();
  }
  #endif
}
