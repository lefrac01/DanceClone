//      Player.cpp
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

#include "Player.h"

namespace DanceClone
{

Player::Player(InputChannel& ic) :
inputs(ic)
{
  directionDownTime.resize(InputChannel::NUM_DIRECTIONS);
  directionJumpActive.resize(InputChannel::NUM_DIRECTIONS);
}

Player& Player::operator= (const Player& b)
{
  playerNumber = b.playerNumber;
  active = b.active;
  ready = b.ready;
  inputs = b.inputs;
  difficulty = b.difficulty;
  recordFileNumber = b.recordFileNumber;
  arrows = b.arrows;
  numArrows = b.numArrows;
  longestCombo = b.longestCombo;
  combo = b.combo;
  firstVisibleArrow = b.firstVisibleArrow;
  lastVisibleArrow = b.lastVisibleArrow;
  arrowFieldXOffset = b.arrowFieldXOffset;
  energyMeter = b.energyMeter;
  energyDifficultyFactor = b.energyDifficultyFactor;
  energyFailTime = b.energyFailTime;
  return *this;
}
  
void Player::Init(int n)
{
  LOG(DEBUG_MINOR, "DanceClone::Player::Init()" << endl)
  playerNumber = n;
  active = true;
  ready = false;
  difficulty = -1;
  recordFileNumber = -1;
  arrowFieldXOffset = -1;
  energyDifficultyFactor = 1.0;
}

void Player::Prepare()
{
  arrows.clear();
  numArrows = 0;
  baseArrow = 0;
  longestCombo = 0;
  combo = 0;
  nextOffscreenArrow = -1;
  firstVisibleArrow = -1;
  lastVisibleArrow = -1;
  nextOffscreenArrow = -1;
  energyMeter = 60.0;
  energyFailTime = -1;
  
  std::fill(directionDownTime.begin(), directionDownTime.end(), -1);
  std::fill(directionJumpActive.begin(), directionJumpActive.end(), false);
}

void Player::DoJumpProcessing(long songTime)
{
  // check clearing control down start time
  for (int d = 0; d < InputChannel::NUM_DIRECTIONS; d++)
  {
    if (directionDownTime[d] != -1 && !inputs.directionHeld[d])
    {
      directionDownTime[d] = -1;
      directionJumpActive[d] = false;
    }
  }

  
  // check setting control down start time
  for (int d = 0; d < InputChannel::NUM_DIRECTIONS; d++)
  {
    if (inputs.directionDown[d])
    {
      directionDownTime[d] = songTime;
    }
  }
  
  // check setting jump flag for each direction
  int numControlsDown = 0;
  for (int d = 0; d < InputChannel::NUM_DIRECTIONS; d++)
  {
    if (directionDownTime[d] != -1)
    {
      ++numControlsDown;
    }
  }
  if (numControlsDown > 1)
  {
    // all controls held down since a time smaller than the allowed jump delay
    // are considered part of a jump
    for (int d = 0; d < InputChannel::NUM_DIRECTIONS; d++)
    {
      directionJumpActive[d] = songTime - directionDownTime[d] <= constants.jumpAllowDelay;
    }
  }
}

int Player::Index()
{
  return playerNumber - 1;
}

}
