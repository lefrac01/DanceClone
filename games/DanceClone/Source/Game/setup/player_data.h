// player_data
//


class player_data
{
private:

public:
  int player_base_arrow;
  vector<arrow> player_arrows;
  int longest_combo;
  int combo;
  int boo;
  int good;
  int perfect;
  vector<unsigned int> current_ratable_arrow;
  vector<unsigned int> next_ratable_arrow;
  int last_visible_arrow;
  int first_visible_arrow;
  int up_control;
  int down_control;
  int left_control;
  int right_control;
    
  player_data();
  void init();
};

player_data::player_data() :
current_ratable_arrow(4), // one for each direction. change to support dance-double
next_ratable_arrow(4) 
{
}

void player_data::init()
{
  player_arrows.clear();
  player_base_arrow = 0;
  longest_combo = 0;
  combo = 0;
  boo = 0;
  good = 0;
  perfect = 0;
  current_ratable_arrow[0]=current_ratable_arrow[1]=current_ratable_arrow[2]=current_ratable_arrow[3] = 0;
  next_ratable_arrow[0]=next_ratable_arrow[1]=next_ratable_arrow[2]=next_ratable_arrow[3] = 0;
  last_visible_arrow = 0;
  first_visible_arrow = 0;
  
  //TODO: control data in vector?
  up_control = 0;
  down_control = 0;
  left_control = 0;
  right_control = 0;
}
