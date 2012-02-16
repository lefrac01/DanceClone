//NOTE: stepmania file format at http://www.stepmania.com/wiki/The_.SM_file_format
//
//TODO: support full stepmania BPM spec.  stepmania stores BPM up to 
//three decimal places.  DC is currently doing atoi on the input...
//also BPM is currently only supported as single BPM.
//
//TODO: check for dance-single, dance-double etc.  only logical
//to support dance single first.  must ignore others
//
//TODO: tighten up last lines buffer logic.  original code uses 20
// char arrays on the stack using a hard-coded value.  one design consideration
// making such a small number reasonable is the decision to shift all
// contents of the array at each read of the input file.
// replace this with a heap-based array using a compile-time constant size
// and track the current, next and recent lines using a secondary index
// array instead of copying all the time.
// FACEPALM d'OH!!!!! ORRRRRRRRRR <n00b_awareness_index_++> shift pointers
// in first dimension of array</n00b_more_aware>
//
//TODO: implement #STOPS:
//
//TODO: tighten up displaying of hold notes. find out if any code needs
// to change in order to read the hold note data properly

//include "../../../../../../Generic/log_console_globbed.h"

//#define LOG_IMPORT
#define LOG_ERRORS

void writesteps(ofstream &outdata)
{
  #ifdef LOG_IMPORT
  ofstream debug_log;
  debug_log.open("debug", std::ios_base::app);
  debug_log << "writesteps() difficulty:" << difficulty << " n_arrows:" << n_arrows << endl;
  debug_log.close();
  #endif
	if(n_arrows)
  {
    #ifdef LOG_IMPORT
    debug_log.open("debug", std::ios_base::app);
    debug_log << "writing difficulty" << endl;
    debug_log.close();
    #endif
    
    if(difficulty == 0) outdata << "<startbeginner>" << endl;
    if(difficulty == 1) outdata << "<starteasy>" << endl;
    if(difficulty == 2) outdata << "<startmedium>" << endl;
    if(difficulty == 3) outdata << "<starthard>" << endl;
    if(difficulty == 4) outdata << "<startchallenge>" << endl;

    #ifdef LOG_IMPORT
    debug_log.open("debug", std::ios_base::app);
    debug_log << "processing " << n_arrows << " arrows." << endl;
    debug_log.close();
    #endif
    for(int a = 0; a < n_arrows; a++)
    {
      outdata << arrows[a]->direction << endl;
      outdata << arrows[a]->time << endl;
      outdata << arrows[a]->length << endl;
      outdata << "----" << endl;
    }
    outdata << "<stop>" << endl;
    
    #ifdef LOG_IMPORT
    debug_log.open("debug", std::ios_base::app);
    debug_log << "deleting arrows" << endl;
    debug_log.close();
    #endif
    while(n_arrows) deletearrow(0);
	}
}

void Game_menu_stepimport_sm(char* filename)
{
  #ifdef LOG_IMPORT
  ofstream debug_log;
  debug_log.open("debug", std::ios_base::trunc);
  debug_log << "beginning Game_menu_stepimport_sm(" << filename << ")" << endl;
  debug_log.close();
  #endif
  #ifdef LOG_ERRORS
  ofstream error_log;
  error_log.open("error", std::ios_base::trunc);
  error_log.close();
  #endif

  ofstream outdata;
#ifdef WII
	ifstream indata;
#endif
#ifdef WIN
	FILE * indata;
#endif

	const int maxlinelength = 10000;    // fix many BPM changes easily over original 1000
  char* temptext = 0;
  char* newtemp = 0;
  char** lastlines;
  
 	double tempBPMS = 0;
	double tempoffset = 0;
	double currenttime = 0;
	double notetimevalue = 0;
	int tempint1 = 0;
	int tempint2 = 0;
	bool placeingnotes = 0;

  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "wiping working mem..." << endl;
  debug_log << "temptext..." << endl;
  debug_log.close();
  #endif
  temptext = (char*)malloc(maxlinelength + 10);
  memset(temptext, 0, sizeof(temptext));

  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "newtemp..." << endl;
  debug_log.close();
  #endif
  newtemp = (char*)malloc(maxlinelength + 10);
  memset(newtemp, 0, maxlinelength + 10);

  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "lastlines array..." << endl;
  debug_log.close();
  #endif
  lastlines = (char **) malloc(20 * sizeof(char *));
  for (int i = 0; i < 20; i++)
  {
    lastlines[i] = (char*)malloc(maxlinelength + 10);
    memset(lastlines[i], 0, sizeof(lastlines[i]));
  }

  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "done" << endl;
  debug_log.close();
  #endif

	sprintf(temptext, "%s%s%s", "Music/", songfilename, ".dc");
  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "outdata.open(" << temptext << ")" << endl;
  debug_log.close();
  #endif
	outdata.open(temptext);
	
	sprintf(temptext, "%s%s", "Music/", filename);
#ifdef WIN
	indata = fopen(temptext, "r");
#endif
#ifdef WII
  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "indata.open(" << temptext << ")" << endl;
  debug_log.close();
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
    char* temp = lastlines[0]; // shifting onto this one
    for (int a = 0; a < 20; a++)
    {
      lastlines[a] = lastlines[a+1];
    }
    lastlines[19] = temp; // put saved pointer back at other end
    
#ifdef WIN
		newtemp[strlen(newtemp) - 1] = '\0';
#endif
#ifdef WII
		indata >> newtemp;
#endif
		sprintf(lastlines[0],"%s",newtemp);

		if(charmatchstart(lastlines[0],(char*)"#BPMS:")){
			tempint1=-1;tempint2=-1;
			for(unsigned int a=0; a<strlen(lastlines[0]); a++){
				if(lastlines[0][a]=='=')tempint1=a+1;}
			for(unsigned int a=0; a<strlen(lastlines[0]); a++){
				if(lastlines[0][a]==';')tempint2=a+0;}
			if(tempint1!=-1 && tempint2!=-1)
      {
				for(int a=0; a<tempint2-tempint1; a++){
					temptext[a]=lastlines[0][a+tempint1];
					temptext[a+1]=0;}
          
        tempBPMS=atof(temptext);
        #ifdef LOG_IMPORT
        debug_log.open("debug", std::ios_base::app);
        debug_log << "extracted tempBPMS:" << tempBPMS << " from text " << temptext << endl;
        debug_log.close();
        #endif
      }
      else
      {
        #ifdef LOG_ERRORS
        error_log.open("errors", std::ios_base::app);
        error_log << "ERROR: failed to extract BPM!" << endl;
        error_log.close();
        #endif
      }
		}
		if(charmatchstart(lastlines[0],(char*)"#OFFSET:")){
      #ifdef LOG_IMPORT
      debug_log.open("debug", std::ios_base::app);
      debug_log << "charmatchstart on lastlines[0].  found #OFFSET:" << endl;
      debug_log << "lastlines[0]=(" << lastlines[0] << ")" << endl;
      debug_log.close();
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

        tempoffset=(long)(atof(temptext)*1000.0);
        if (tempoffset < 0) tempoffset = -tempoffset; // current stepmania format is negative float offset in seconds
        #ifdef LOG_IMPORT
        debug_log.open("debug", std::ios_base::app);
        debug_log << "extracted offset:" << tempoffset << " from text " << temptext << endl;
        debug_log.close();
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
		if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Beginner:")){
      #ifdef LOG_IMPORT
      debug_log.open("debug", std::ios_base::app);
      debug_log << "charmatchend on lastlines[0].  found Beginner" << endl;
      debug_log.close();
      #endif
			while(n_arrows)deletearrow(0);
			placeingnotes=1;difficulty=0;
			currenttime=tempoffset;}
		if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Easy:")){
      #ifdef LOG_IMPORT
      debug_log.open("debug", std::ios_base::app);
      debug_log << "charmatchend on lastlines[0].  found Easy" << endl;
      debug_log.close();
      #endif
			while(n_arrows)deletearrow(0);
			placeingnotes=1;difficulty=1;
			currenttime=tempoffset;}
		if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Medium:")){
      #ifdef LOG_IMPORT
      debug_log.open("debug", std::ios_base::app);
      debug_log << "charmatchend on lastlines[0].  found Medium" << endl;
      debug_log.close();
      #endif
			while(n_arrows)deletearrow(0);
			placeingnotes=1;difficulty=2;
			currenttime=tempoffset;}
		if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Hard:")){
      #ifdef LOG_IMPORT
      debug_log.open("debug", std::ios_base::app);
      debug_log << "charmatchend on lastlines[0].  found Hard" << endl;
      debug_log.close();
      #endif
			while(n_arrows)deletearrow(0);
			placeingnotes=1;difficulty=3;
			currenttime=tempoffset;}
		if(placeingnotes==0 && charmatchend(lastlines[0],(char*)"Challenge:")){
      #ifdef LOG_IMPORT
      debug_log.open("debug", std::ios_base::app);
      debug_log << "charmatchend on lastlines[0].  found Challenge" << endl;
      debug_log.close();
      #endif
			while(n_arrows)deletearrow(0);
			placeingnotes=1;difficulty=4;
			currenttime=tempoffset;}
		if(placeingnotes){
			if((lastlines[0][0]==','||lastlines[0][0]==';') && strlen(lastlines[1])==4){
        #ifdef LOG_IMPORT
        debug_log.open("debug", std::ios_base::app);
        debug_log << "detected note end separator" << endl;
        debug_log.close();
        #endif
        
        //QUESTION: what does notetimevalue represent?
        //ANSWER: stepmania outputs steps in blocks of varying lengths depending
        // on the shortest step time value in the block.  example if all steps in a beat
        // are at least quarter note beats (ie four steps per beat) then there are four
        // lines in the file.  if there are eighth note beats (blue steps) then there are eight
        // lines.
        //PROBLEM: this import algo uses a buffer of latest lines read from input file
        // and searches backward in the buffer to find the length of the step block.
        // since the original buffer is 20 lines long, at most 16-line step blocks can be detected.
        // it is common for DDR songs to use shorter note beats (yellow and/or green steps?)
        //QUESTION2: what is the minimum supported note length beat.  ddreamstudio allows 192nd
        // notes but that is ridiculous, meaning a 192x4 block length.  in practice 64 should be a 
        // reasonable target.
        
				notetimevalue=0;
				for(int a=1; a<20; a++){
					if(strlen(lastlines[a])!=4){  // could start for at a=2 since strlen(lastlines[1])==4
						notetimevalue=1/(double)(a-1)*4;
						a=500;}
				}
        #ifdef LOG_IMPORT
        debug_log.open("debug", std::ios_base::app);
        debug_log << "calculated note time value: " << notetimevalue << endl;
        debug_log.close();
        #endif
				for(int a=(int)(1/notetimevalue*4); a>0; a--){
					currenttime=currenttime+notetimevalue*1000*60/tempBPMS;
          #ifdef LOG_IMPORT
          debug_log.open("debug", std::ios_base::app);
          debug_log << "currenttime is now " << currenttime << endl;
          debug_log << "looking in lastlines[a][0]: " << lastlines[a][0] << " for a 1 and using n_arrows and maxarrows " << n_arrows << " and " << maxarrows << endl;
          debug_log.close();
          #endif
					if(lastlines[a][0]=='1' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(0,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][1]=='1' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(1,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][2]=='1' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(2,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][3]=='1' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(3,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][0]=='2' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(0,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][1]=='2' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(1,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][2]=='2' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(2,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][3]=='2' && n_arrows<maxarrows){
						arrows[n_arrows] = new arrow(3,(int)(currenttime),0);
						n_arrows++;}
					if(lastlines[a][0]=='3' && n_arrows<maxarrows)
						for(int b=n_arrows-1; b>0; b--)if(arrows[b]->direction==0){
							arrows[b]->length=(int)currenttime-arrows[b]->time;b=0;}
					if(lastlines[a][1]=='3' && n_arrows<maxarrows)
						for(int b=n_arrows-1; b>0; b--)if(arrows[b]->direction==1){
							arrows[b]->length=(int)currenttime-arrows[b]->time;b=0;}
					if(lastlines[a][2]=='3' && n_arrows<maxarrows)
						for(int b=n_arrows-1; b>0; b--)if(arrows[b]->direction==2){
							arrows[b]->length=(int)currenttime-arrows[b]->time;b=0;}
					if(lastlines[a][3]=='3' && n_arrows<maxarrows)
						for(int b=n_arrows-1; b>0; b--)if(arrows[b]->direction==3){
							arrows[b]->length=(int)currenttime-arrows[b]->time;b=0;}
              
          #ifdef LOG_IMPORT
          debug_log.open("debug", std::ios_base::app);
          debug_log << "incremented currenttime ???? currenttime is now " << currenttime << endl;
          debug_log.close();
          #endif
				}
			}
			if(lastlines[0][0]==';')
      {
        placeingnotes = 0;
        #ifdef LOG_IMPORT
        debug_log.open("debug", std::ios_base::app);
        debug_log << "writesteps(outdata) in while" << endl;
        debug_log.close();
        #endif
        writesteps(outdata);
      }
		}
	}
  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "writesteps(outdata)" << endl;
  debug_log.close();
  #endif
	writesteps(outdata);
	#ifdef WII
	indata.close();
	#endif
	outdata.close();

  free(temptext);
  free(newtemp);
  for (int i=0; i<20; i++)
  {
    free(lastlines[i]);
  }
  free(lastlines);
  
  #ifdef LOG_IMPORT
  debug_log.open("debug", std::ios_base::app);
  debug_log << "leaving step import" << endl;
  debug_log.close();
  #endif
}
