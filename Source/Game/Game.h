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
#include <vector>
using std::vector;

#include <fstream>
using std::ofstream;
using std::ifstream;
using std::endl;

#include <boost/algorithm/string.hpp>

#include "../Platform/OS.h"
using Platform::OS;
using Platform::DirectoryEntry;
#include "../GUI/GUI.h"
using Gooey::GUI;
using Gooey::Container;
using Gooey::Element;
using Gooey::Button;
using Gooey::Image;
using Gooey::Label;
#include "../GUI/SimpleSongScroller.h"
using Gooey::SimpleSongScroller;
#include "Constants.h"
#include "Sound.h"
#include "Graphics.h"

namespace DanceClone
{

class Game
{
public:

  enum GameState
  {
    NONE,         //
    TITLE,        // 0
    CREDITS,      // 1
    SCORE,        // 2, 9
    SONG_SELECT1, // 3
    SONG_SELECT2, // 4
    STEP_CREATE,  // 5
    PLAY_PREP1,   // 7
    PLAY_PREP2,   // 11
    PLAY,         // 8
    DEBUG         // 10
  };


private:
  
  OS& sys;
  GUI& gui;
  GameState state;
  bool gameStateChanged;
  Constants constants;
  Sound sound;
  Graphics gfx;
  Game(); //disallow emtpy public ctor 

  
public:

  Game(OS& os, GUI& g);
  bool Init(string configFilePath = "");
  void Cleanup();
  void Run();
  GameState State();
  void RunTitleScreen();
  void RunCreditsScreen();
  void RunDebugScreen();
  void RunSongSelect1();
};

}
