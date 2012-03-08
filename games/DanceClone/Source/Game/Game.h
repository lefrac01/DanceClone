//      Game.h
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


#include <string>
using std::string;
#include <fstream>
using std::ofstream;
using std::ifstream;
using std::endl;

#include "../Platform/Platform.h"
#include "Constants.h"
#include "SoundAgent.h"
#include "GraphicsAgent.h"

namespace DanceClone
{

class Game
{
private:

  enum GameState
  {
    NONE,
    TITLE,
    CREDITS,
    SCORE,
    SONGSELECTPT1,
    SONGSELECTPT2,
    STEPCREATE,
    PLAYPREPPT1,
    PLAYPREPPT2,
    PLAY,
    DEBUG
  };
  
  GameState state;
  Platform& platform;
  Constants constants;
  SoundAgent soundAgent;
  GraphicsAgent graphicsAgent;
  Game(); //disallow emtpy public ctor 
  
public:

  Game(Platform& p);
  bool Init(string configFilePath = "");
  void Cleanup();
  void Pump();
  GameState State();
};

}
