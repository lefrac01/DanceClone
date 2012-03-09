//TODO: add second player
//
//TODO: ratable arrows per player
//
// Game_common

// functions and variables that apply at game scope

// based on observation:
// 4 seconds for a quarter note at 120 BPM to traverse a 480 high screen


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

