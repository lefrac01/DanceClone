// player_data
//

//TODO: make global since same index as sm files to be used?
#define LEFT 0
#define DOWN 1
#define UP 2
#define RIGHT 3
#define NUM_CONTROL_DIRECTIONS 4

class control_direction_data
{
private:

public:
  bool down;
  long down_time;
  bool held;
  bool jump_active;
};

class player_data
{
private:

public:
  int base_arrow;
  vector<arrow> arrows;
  int longest_combo;
  int combo;
  int boo;
  int good;
  int great;
  int perfect;
  int marvellous;
  int first_visible_arrow;
  int last_visible_arrow;
  int next_offscreen_arrow;
  int num_arrows;

  vector<control_direction_data> control_data;
  player_data();
  void init();
};

player_data::player_data() :
control_data(NUM_CONTROL_DIRECTIONS)
{
}

void player_data::init()
{
  arrows.clear();
  base_arrow = 0;
  longest_combo = 0;
  combo = 0;
  boo = 0;
  good = 0;
  perfect = 0;
  last_visible_arrow = -1;
  first_visible_arrow = -1;
  next_offscreen_arrow = -1;
  
  for (int i = 0; i > NUM_CONTROL_DIRECTIONS; i++)
  {
    control_data[i].down = false;
    control_data[i].down_time = -1;
    control_data[i].held = false;
    control_data[i].jump_active = false;
  }
}
