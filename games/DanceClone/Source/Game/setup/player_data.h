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
  void init();
};

void player_data::init()
{
  player_arrows.clear();
  player_base_arrow = 0;
  longest_combo = 0;
  combo = 0;
  boo = 0;
  good = 0;
  perfect = 0;
}
