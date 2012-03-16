//      Player.h
//      
//      Copyright 2012 Carl Lefrançois <carl.lefrancois@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <vector>
using std::vector;

#include "../Platform/LOG.H"
#include "../Platform/Input.h"
using Platform::InputChannel;

#include "Constants.h"
#include "Arrow.h"

namespace DanceClone
{

class Player
{
private:
  Player();

public:

  Player& operator= (const Player& b);
  Player(InputChannel& ic);
  int playerNumber;
  bool active;
  bool ready;
  InputChannel& inputs;
  vector<long> directionDownTime;
  vector<bool> directionJumpActive;

  //Constants::Difficulty difficulty;
  int difficulty;
  //recordFileId      //note operator=
  vector<Arrow> arrows;
  int baseArrow;
  int numArrows;
  int nextOffscreenArrow; 
  int longestCombo;
  int combo;
  //dance mode e.g. dance-single vs dance-double      //note operator=
  int firstVisibleArrow;
  int lastVisibleArrow;
  int arrowFieldXOffset;
  
  void Init(int n);
  void Prepare();
  void DoJumpProcessing(long songTime);
};

}


