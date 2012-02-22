//NOTE: too idealistic / purist
//
// ideally, a generic data format exists between song and sm_file and the
// other classes defining various data types.  at the sm_file level, 
// parsing creates the generic data, e.g. a vector of int, float pairs
// for bpm data.  then the song class receives generic data and internally
// constructs its bpm_change objects accordingly.
//
// for now, just kiasap


/*

//NOTE: stepmania file format at http://www.stepmania.com/wiki/The_.SM_file_format
//
//TODO: implement #STOPS:
//
#include <string>
using std::string;

class sm_file
{
private:
  sm_file();
  string path;
  vector<
public:
  sm_file(string p) {path = p;};
  parse();
};

sm_file::parse()
{
  if (DEBUG_LEVEL >= DEBUG_BASIC)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "beginning Game_menu_stepimport_sm(" << filename << ")" << endl;
    debug_log.close();
  }


  vector<string> file_lines;
  int current_line = 0;

  float currenttime = 0;
  float notetimevalue = 0;
  long beat0offset = 0;
  bool placeingnotes = 0;
  bool in_dance_single_block = 0;


  // read file
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "indata.open(" << temptext << ")" << endl;
    debug_log.close();
  }
  ifstream indata;
  indata.open(path());
  string file_line;
  while (getline(indata, file_line)) file_lines.push_back(file_line);
  
  // analyse contents
  string bpm_tag = "#BPMS:";
  string offset_tag = "#OFFSET:";
  string notes_tag = "#NOTES:";
  while (current_line < file_lines.size())
  {
    if (file_lines[current_line].substr(0, bpm_tag.size()) == bpm_tag)
    {
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found #BPMS:" << endl;
        debug_log.close();
      }
      // found bpm tag
      parse_bpms(file_lines[current_line];
    }
    else if (file_lines[current_line].substr(0, offset_tag.size()) == offset_tag)
    {
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "found #OFFSET:" << endl;
        debug_log.close();
      }
      
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
    
    current_line++;
  }
  
  
  
    if(charmatchstart(lastlines[0],(char*)"#BPMS:"))
    {
      //TODO: would be nice to start considering possibility of errors.
      // the purist in me defined a bool return value here...
      parse_bpms(lastlines[0]);
      
      //copy all bpm data to output file
      outdata << lastlines[0] << endl;
    }
   
  
}


*/
/*
void writesteps(ofstream &outdata)
{
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "writesteps() difficulty:" << difficulty << " number of arrows:" << song_arrows[difficulty].size() << endl;
    debug_log.close();
  }
  #endif

  
  if(song_arrows[difficulty].size())
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
      debug_log << "processing " << song_arrows[difficulty].size() << " arrows." << endl;
      debug_log.close();
    }
    #endif
    
    for(unsigned int a = 0; a < song_arrows[difficulty].size(); a++)
    {
      outdata << song_arrows[difficulty][a].direction << endl;
      outdata << song_arrows[difficulty][a].ypos << endl;
      outdata << song_arrows[difficulty][a].length << endl;
      outdata << song_arrows[difficulty][a].type << endl;
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
    init_arrow_data_structures();
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


  float currenttime = 0;
  long beat0offset = 0;
  float notetimevalue = 0;
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


  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_GUTS)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "song data structures..." << endl;
    debug_log.close();
  }
  #endif
  init_song_data_structures();

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


    if(charmatchstart(lastlines[0],(char*)"#BPMS:"))
    {
      //TODO: would be nice to start considering possibility of errors.
      // the purist in me defined a bool return value here...
      parse_bpms(lastlines[0]);
      
      //copy all bpm data to output file
      outdata << lastlines[0] << endl;
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
        init_arrow_data_structures();
        placeingnotes=1;difficulty=0;
        currenttime=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Easy:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Easy" << endl;
          debug_log.close();
        }
        #endif
        init_arrow_data_structures();
        placeingnotes=1;difficulty=1;
        currenttime=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Medium:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Medium" << endl;
          debug_log.close();
        }
        #endif
        init_arrow_data_structures();
        placeingnotes=1;difficulty=2;
        currenttime=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Hard:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Hard" << endl;
          debug_log.close();
        }
        #endif
        init_arrow_data_structures();
        placeingnotes=1;difficulty=3;
        currenttime=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
      if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Challenge:")){
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_MINOR)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "charmatchend on lastlines[0].  found Challenge" << endl;
          debug_log.close();
        }
        #endif
        init_arrow_data_structures();
        placeingnotes=1;difficulty=4;
        currenttime=beat0offset;}  //TODO: oops.  these y values are virtual, not what will be used at anim time
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
        if (bpm_changes[current_bpm_index].beat == current_beat)
        {
          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "beat change detected on beat " << current_beat << " new bpm is at index " << current_bpm_index << endl;
            debug_log.close();
          }
          #endif
          
          // there is a BPM change on this beat.
          current_bpm = bpm_changes[current_bpm_index].bpm;
          
          // now that the songtime of the current beat is calculated,
          // save it for output of bpm data
          bpm_changes[current_bpm_index].timestamp = currenttime;
          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "using currenttime " << currenttime << " for position of bpm change on beat " << current_beat << endl;
            debug_log.close();
          }
          #endif

          // move index to next index (if there is one!)
          current_bpm_index++;
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
          
          if(lastlines[a][0]=='1')song_arrows[difficulty].push_back(arrow(0,(long)currenttime,0,0,note_type));
          if(lastlines[a][1]=='1')song_arrows[difficulty].push_back(arrow(1,(long)currenttime,0,0,note_type));
          if(lastlines[a][2]=='1')song_arrows[difficulty].push_back(arrow(2,(long)currenttime,0,0,note_type));
          if(lastlines[a][3]=='1')song_arrows[difficulty].push_back(arrow(3,(long)currenttime,0,0,note_type));
          if(lastlines[a][0]=='2')song_arrows[difficulty].push_back(arrow(0,(long)currenttime,0,0,NOTE_TYPE_HOLD));
          if(lastlines[a][1]=='2')song_arrows[difficulty].push_back(arrow(1,(long)currenttime,0,0,NOTE_TYPE_HOLD));
          if(lastlines[a][2]=='2')song_arrows[difficulty].push_back(arrow(2,(long)currenttime,0,0,NOTE_TYPE_HOLD));
          if(lastlines[a][3]=='2')song_arrows[difficulty].push_back(arrow(3,(long)currenttime,0,0,NOTE_TYPE_HOLD));
          if(lastlines[a][0]=='3')
            for(int b=song_arrows[difficulty].size()-1; b>0; b--)if(song_arrows[difficulty][b].direction==0){
              song_arrows[difficulty][b].length=(int)currenttime-song_arrows[difficulty][b].ypos;b=0;}
          if(lastlines[a][1]=='3')
            for(int b=song_arrows[difficulty].size()-1; b>0; b--)if(song_arrows[difficulty][b].direction==1){
              song_arrows[difficulty][b].length=(int)currenttime-song_arrows[difficulty][b].ypos;b=0;}
          if(lastlines[a][2]=='3')
            for(int b=song_arrows[difficulty].size()-1; b>0; b--)if(song_arrows[difficulty][b].direction==2){
              song_arrows[difficulty][b].length=(int)currenttime-song_arrows[difficulty][b].ypos;b=0;}
          if(lastlines[a][3]=='3')
            for(int b=song_arrows[difficulty].size()-1; b>0; b--)if(song_arrows[difficulty][b].direction==3){
              song_arrows[difficulty][b].length=(int)currenttime-song_arrows[difficulty][b].ypos;b=0;}

          
          #ifdef LOG_IMPORT
          if (DEBUG_LEVEL >= DEBUG_DETAIL)
          {
            debug_log.open("debug", std::ios_base::app);
            debug_log << "updating current pos " << currenttime << " to " << currenttime+notetimevalue*1000*60/current_bpm << " on beat " << current_beat << endl;
            debug_log.close();
          }
          #endif
          
          currenttime += notetimevalue * 1000 * 60 / current_bpm;
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
  
 
  #ifdef LOG_IMPORT
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "leaving step import" << endl;
    debug_log.close();
  }
  #endif
}
*/
