//TODO: add second player

const int maxarrows             = 1000;
class arrow{
public:
  int direction;
  int time;
  int length;
  arrow(int d,int t,int l);
};
arrow::arrow(int d,int t,int l)
{
	direction = d;
	time = t;
	length = l;
}
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
	deletearrow(arrownum);
}
