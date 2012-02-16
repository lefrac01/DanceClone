//TODO: add second player

const int maxarrows             = 1000;
class arrow{
public:
  int direction;
  int time;
  int length;
  bool is_hold;
  //#arrow(int d,int t,int l);
};
/*
arrow::arrow(int d,int t,int l)
{
  direction = d;
  time = t;
  length = l;
}
*/

int songarrowcount[5];
int playerarrowcount = 0;
int playerbasearrow = 0;  // original logic deleted arrows keeping arrow
//0 as the first arrow in iteration operations.  instead, use an offset
//to the first index in the array to start at. crossing fingers

arrow* playerarrows = 0;
arrow** songarrows = 0;

//void freearrowstructures(); // maybe later if too much time on hands
void initarrowstructures()
{
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "initialising arrow strucutres" << endl;
    debug_log.close();
  }  
  if (songarrows)
  {
    free(songarrows[0]);
    free(songarrows[1]);
    free(songarrows[2]);
    free(songarrows[3]);
    free(songarrows[4]);
    free(songarrows);
  }
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

  if (playerarrows)
  {
    free(playerarrows);
  }
  playerarrows = (arrow*)malloc( (maxarrows+1)*sizeof(arrow*));
  playerarrowcount = 0;
  playerbasearrow = 0;
}

void assignsongarrow(int difficulty_in, int direction_in, int time_in, int length_in, bool is_hold_in)
{
  if (songarrowcount[difficulty_in] < maxarrows)
  {
    // assign new arrow
    songarrows[difficulty_in][songarrowcount[difficulty_in]].direction = direction_in;
    songarrows[difficulty_in][songarrowcount[difficulty_in]].time = time_in;
    songarrows[difficulty_in][songarrowcount[difficulty_in]].length = length_in;
    songarrows[difficulty_in][songarrowcount[difficulty_in]].is_hold = is_hold_in;
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
    playerarrows_in[i].time = songarrows[difficulty_in][i].time;
    playerarrows_in[i].length = songarrows[difficulty_in][i].length;
    playerarrows_in[i].is_hold = songarrows[difficulty_in][i].is_hold;
  }
  if (DEBUG_LEVEL >= DEBUG_MINOR)
  {
    debug_log.open("debug", std::ios_base::app);
    debug_log << "done" << endl;
    debug_log.close();
  }  
}
/*
arrow* arrows[maxarrows+1];

int n_arrows = 0;


//QUESTION: is there a need to delete a random arrow out of the array?
void deletearrow(int num)
{
  delete arrows[num];
  arrows[n_arrows] = NULL;  // err what was arrows[n_arrows] before?
  if(num < n_arrows-1)
  {
    for(int b = num; b < n_arrows; b++)
    {
      arrows[b] = arrows[b+1];
    }
  }
  arrows[n_arrows-1] = NULL;
  n_arrows -= 1;
}
*/
const int maxratings             = 500;
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
  ratings[n_ratings] = new rating(320-125/2, goalline, rate);
  n_ratings++;
//  deletearrow(arrownum);
  playerbasearrow++;
}
