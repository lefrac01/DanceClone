// player_data
//


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
  bool up_control_down;
  bool down_control_down;
  bool left_control_down;
  bool right_control_down;
  bool up_control_held;
  bool down_control_held;
  bool left_control_held;
  bool right_control_held;
    
  void init();
  int find_next_ratable_arrow(unsigned int direction);
};

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
  
  //TODO: control data in vector?
  up_control_down = false;
  down_control_down = false;
  left_control_down = false;
  right_control_down = false;
  up_control_held = false;
  down_control_held = false;
  left_control_held = false;
  right_control_held = false;
}

int player_data::find_next_ratable_arrow(unsigned int direction)
{
  return -1;
}
