//TODO: add second player
//
//TODO: ratable arrows per player
//
// Game_common

#include "setup/classes.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

// functions and variables that apply at game scope

int goaloffset = 0;

char songfilename[MAXPATHLEN];


// maxlinelength is 10000 chars.  one BPM definition requires
// about 10 chars so max ~1000 definitions.  
int   current_bpm_index = 0;
//int   num_bpm_changes = 0;
int   current_beat = 0;
float current_bpm = 0;

//const int maxarrows = 1000;
//const int max_bpm_changes = 1000;

//int songarrowcount[5];
//int playerarrowcount = 0;
int player_base_arrow = 0;
//arrow* playerarrows = NULL;
//arrow** songarrows = NULL;
//bpm_change* bpm_changes = NULL;
vector<arrow> player_arrows;
vector< vector<arrow> > song_arrows (5);  // a vector for each difficulty
vector<bpm_change> bpm_changes;

void init_arrow_data_structures()
{
//  songarrowcount[0]=0;
//  songarrowcount[1]=0;
//  songarrowcount[2]=0;
//  songarrowcount[3]=0;
//  songarrowcount[4]=0;

  // using hard-coded number of difficulties 5.  why bother with more fanciness
  //songarrows = (arrow**)malloc(5*sizeof(arrow*));
  //songarrows[0] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  //songarrows[1] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  //songarrows[2] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  //songarrows[3] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  //songarrows[4] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  song_arrows.clear();

  player_arrows.clear();
  //playerarrows = (arrow*)malloc( (maxarrows+1)*sizeof(arrow*));
  //playerarrowcount = 0;
  //playerbasearrow = 0;
}

/*
void free_arrow_data_structures()
{
  if (songarrows)
  {
    free(songarrows[0]);
    free(songarrows[1]);
    free(songarrows[2]);
    free(songarrows[3]);
    free(songarrows[4]);
    free(songarrows);
    songarrows = NULL;
  }
  if (playerarrows)
  {
    free(playerarrows);
    playerarrows = NULL;
  }
}
*/
void init_bpm_data_structures()
{
  //bpm_changes = (bpm_change*)malloc(max_bpm_changes * sizeof(bpm_change));
  bpm_changes.clear();
  //#num_bpm_changes = 0;
  current_bpm_index = 0;
}
/*
void free_bpm_data_structures()
{
  if(bpm_changes)
  {
    free(bpm_changes);
    bpm_changes = NULL;
  }
}


void free_song_data_structures()
{
  free_arrow_data_structures();
  free_bpm_data_structures();
}
*/

void init_song_data_structures()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "initialising song data structures" << endl;
    debug_log.close();
  }  
//  free_song_data_structures();
  
  init_arrow_data_structures();
  init_bpm_data_structures();
}


bool parse_bpms(const string& str)
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "parse_bpms()" << endl;
    debug_log.close();
  }
  //free_bpm_data_structures();
  init_bpm_data_structures();

    
  //NOTE: sample and definition of SM BPMS tag
  //#BPMS:0=140.215,1=140.208,14=140.215,16=140.208,43=140.215;
  //
  //#BPMS:<int beat>=<float BPM>[,<int beat>=<float BPM>];
  //beat 0 must be defined
  
  size_t start_pos = 0;
  size_t end_pos = 0;
 
  while ( (start_pos = str.find_first_of(":,", start_pos)) != string::npos)
  {
    // search for end of first part of bpm token
    start_pos++;
    end_pos = str.find_first_of("=", start_pos);
    if (end_pos != string::npos)
    {
      string bpm_beat = str.substr(start_pos, end_pos - start_pos);

      #ifdef LOG_IMPORT
      if (DEBUG_LEVEL >= DEBUG_DETAIL)
      {
        debug_log.open("debug", std::ios_base::app);
        debug_log << "extracted int beat: " << atoi(bpm_beat.c_str()) << " from text (" << bpm_beat << ")" << endl;
        debug_log.close();
      }
      #endif

      // search for end of second part of bpm token
      start_pos = end_pos+1;
      end_pos = str.find_first_of(",;", start_pos);
      if (end_pos != string::npos)
      {
        // found complete bpm token
        string bpm_float = str.substr(start_pos, end_pos-start_pos);
        
        #ifdef LOG_IMPORT
        if (DEBUG_LEVEL >= DEBUG_DETAIL)
        {
          debug_log.open("debug", std::ios_base::app);
          debug_log << "extracted float BPM: " << atof(bpm_float.c_str()) << " from text (" << bpm_float << ")" << endl;
          debug_log.close();
        }
        #endif        
        
        // add to array of bpm changes
        bpm_changes.push_back(bpm_change(atoi(bpm_beat.c_str()), 0, atof(bpm_float.c_str())));
        //#bpm_changes.beat = atoi(bpm_beat.c_str()) ;
        //#bpm_changes[num_bpm_changes].bpm = atof(bpm_float.c_str());
        //#num_bpm_changes++;
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
    /*
    if (str[offset]==':' || str[offset]==',')
    {
      tempint1 = offset+1;
      tempint2 = -1;
      for(unsigned int a=tempint1; a<linelength && tempint2==-1; a++)
      {
        if(str[a]=='=')
        {
          tempint2 = a;
        }
      }
      if(tempint2!=-1) // found an int describing the beat to apply the BPM
      {
        string bpm_beat = "";
        for(int a=0; a<tempint2-tempint1; a++)
        {
          bpm_beat += str[tempint1+a];
          temptext[a]=str[tempint1+a];
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
          if(p[a]==',' || p[a]==';')
          {
            tempint2 = a;
          }
        }
        if(tempint2!=-1) // found a float describing the BPM
        {
          for(int a=0; a<tempint2-tempint1; a++)
          {
            temptext[a]=p[tempint1+a];
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
/////////
/////////
          
          if (num_bpm_changes < max_bpm_changes)
          {
            bpm_indices[num_bpm_changes] = temp_beat;
            bpm_changes[num_bpm_changes].bpm = temp_BPM;
            num_bpm_changes++;
          }
          else
          {
            #ifdef LOG_ERRORS
            error_log.open("errors", std::ios_base::app);
            error_log << "ERROR: too many BPMS changes! (more than " << max_bpm_changes << ")" << endl;
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
    */
  
  #ifdef LOG_ERRORS
  if (bpm_changes.size() == 0)
  {
    error_log.open("errors", std::ios_base::app);
    error_log << "ERROR: failed to extract BPMS!" << endl;
    error_log.close();
    return false;
  }
  #endif  

  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "parse_bpms() finished.  extracted " << bpm_changes.size() << " bpm changes" << endl;
    debug_log.close();
  }  
  return true;
}

/*
void assignsongarrow(int difficulty_in, int direction_in, long ypos_in, int length_in, int type_in)
{
  // assign new arrow
  songarrows[difficulty_in][songarrowcount[difficulty_in]].direction = direction_in;
  songarrows[difficulty_in][songarrowcount[difficulty_in]].ypos = ypos_in;
  songarrows[difficulty_in][songarrowcount[difficulty_in]].length = length_in;
  songarrows[difficulty_in][songarrowcount[difficulty_in]].type = type_in;
  songarrowcount[difficulty_in]++;
}
*/

void setup_player_arrows(vector<arrow>& player_arrows_in, int difficulty_in)
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "setting up player arrow copy" << endl;
    debug_log.close();
  }  
  player_arrows_in = song_arrows[difficulty_in];
  player_base_arrow = 0;
  /*
  playerarrowcount = songarrowcount[difficulty_in]; // shooting first
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "shot first getting " << playerarrowcount << " from song arrow count array at difficulty " << difficulty_in << endl;
    debug_log.close();
  }
  for (int i=0; i<playerarrowcount; i++)
  {
    playerarrows_in[i].direction = songarrows[difficulty_in][i].direction;
    playerarrows_in[i].ypos = songarrows[difficulty_in][i].ypos;
    playerarrows_in[i].length = songarrows[difficulty_in][i].length;
    playerarrows_in[i].type = songarrows[difficulty_in][i].type;
  }
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done" << endl;
    debug_log.close();
  }  
  */
}

const int maxratings             = 1000;    // hard to pick a good value.
// some fanatics make extreme songs and >500 is in normal DDR
class rating
{
public:
  int posx;
  int posy;
  int type;
  rating(int x,int y,int t);
};
rating::rating(int x,int y,int t)
{
  posx = x;
  posy = y;
  type = t;
}

rating* ratings[maxratings+1];
int n_ratings = 0;

void deleterating(int num)
{
  delete ratings[num];
  ratings[n_ratings] = NULL;
  if(num < n_ratings-1)
  {
    for(int b = num; b < n_ratings; b++)
    {
      ratings[b] = ratings[b+1];
    }
  }
  ratings[n_ratings-1] = NULL;
  n_ratings -= 1;
}

void ratearrow(int arrownum, int rate)
{
  ratings[n_ratings] = new rating(rmode->viWidth/2-125/2, goaloffset, rate);
  n_ratings++;
  player_base_arrow++;
}
