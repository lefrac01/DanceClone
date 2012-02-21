//TODO: add second player

const int maxarrows             = 1000;
class arrow{
public:
  int direction;
  //int time; // zzz too easy or too hard.  nothing matters or makes sense anymore zzz
  long ypos;
  int length;
  int type; // for drawing based on note value
};
const int max_bpm_changes = 1000;
class bpm_change
{
public:
  long ypos;
  float bpm;
};

int songarrowcount[5];
int playerarrowcount = 0;
int playerbasearrow = 0;
int number_bpm_changes = 0;
arrow* playerarrows = 0;
arrow** songarrows = 0;
bpm_change* bpm_changes = 0;

void freesongdatastructures()
{
  if (songarrows)
  {
    free(songarrows[0]);
    free(songarrows[1]);
    free(songarrows[2]);
    free(songarrows[3]);
    free(songarrows[4]);
    free(songarrows);
    songarrows = 0;
  }
  if (playerarrows)
  {
    free(playerarrows);
    playerarrows = 0;
  }
  if(bpm_changes)
  {
    free(bpm_changes);
    bpm_changes = 0;
  }
}

void initsongdatastructures()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "initialising song data strucutres" << endl;
    debug_log.close();
  }  
  freesongdatastructures();
  
  songarrowcount[0]=0;
  songarrowcount[1]=0;
  songarrowcount[2]=0;
  songarrowcount[3]=0;
  songarrowcount[4]=0;
  
  // using hard-coded number of difficulties 5.  why bother with more fanciness
  songarrows = (arrow**)malloc(5*sizeof(arrow*));
  songarrows[0] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  songarrows[1] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  songarrows[2] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  songarrows[3] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));
  songarrows[4] = (arrow*)malloc( (maxarrows+1)*sizeof(arrow));

  playerarrows = (arrow*)malloc( (maxarrows+1)*sizeof(arrow*));
  playerarrowcount = 0;
  playerbasearrow = 0;

  bpm_changes = (bpm_change*)malloc(max_bpm_changes * sizeof(bpm_change));
  number_bpm_changes = 0;
}

void assignsongarrow(int difficulty_in, int direction_in, long ypos_in, int length_in, int type_in)
{
  if (songarrowcount[difficulty_in] < maxarrows)
  {
    // assign new arrow
    songarrows[difficulty_in][songarrowcount[difficulty_in]].direction = direction_in;
    songarrows[difficulty_in][songarrowcount[difficulty_in]].ypos = ypos_in;
    songarrows[difficulty_in][songarrowcount[difficulty_in]].length = length_in;
    songarrows[difficulty_in][songarrowcount[difficulty_in]].type = type_in;
    songarrowcount[difficulty_in]++;
  }            
}

void setupplayerarrows(arrow* playerarrows_in, int difficulty_in)
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "setting up player arrow copy" << endl;
    debug_log.close();
  }  
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
  //TODO: fix goalline vs goaloffset
  ratings[n_ratings] = new rating(rmode->viWidth/2-125/2, goaloffset, rate);
  n_ratings++;
  playerbasearrow++;
}
