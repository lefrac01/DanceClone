//      Game.cpp
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

#include "Game.h"

namespace DanceClone
{

Game::Game(OS& os, GUI& g) :
sys(os),
gui(g),
gfx(os)
{
}

bool Game::Init(string ConfigFilePath)
{
  try
  {
    constants.Init();
    gfx.Init();
  }
  catch (exception& e)
  {
    // log
    LOG(DEBUG_BASIC, "Game::Init error (" << e.what() << ")" << endl)
    ofstream error_log;
    error_log.open("errors", std::ios_base::app);
    error_log << " Game::Init() failed due to exception: " << endl;
    error_log << e.what();
    error_log.close();
    return false;
  }

  sound.Init();
  
  gameStateChanged = false;
  state = TITLE;
  
  return true;
}

void Game::Cleanup()
{
  gfx.Cleanup();
}

void Game::Run()
{
  //TODO: gfx are a resource.  have a class responsible for all the 
  // game drawing logic
  gfx.DrawBackground();

  GameState oldState = state;

  switch (state)
  {
  case TITLE:
    RunTitleScreen();
    break;
  case CREDITS:
    //#Game_menu_credits();
    break;
  case SCORE:
    //#Game_menu_score();
    break;
  case SONG_SELECT1:
    //#Game_menu_songselectp1();
    break;
  case SONG_SELECT2:
    //#Game_menu_songselectp2();
    break;
  case STEP_CREATE:
    //#Game_menu_stepcreate();
    break;
  case PLAY_PREP1:
    //#Game_playprep();
    break;
  case PLAY_PREP2:
    //#Game_playprep();
    break;
  case PLAY:
    //#Game_play();
    break;
  case DEBUG:
    //#Game_menu_debug();
    break;
  case NONE:
  default:
    state = TITLE;
    break;
  }

  gameStateChanged = oldState == state;
}

Game::GameState Game::State()
{
  return state;
}

//NOTE: no relation to OO methodology here!  no time!!
void Game::RunTitleScreen()
{
  sys.vid.ApplySurface(sys.vid.ScreenWidth()/2-300/2, 30, gfx.title_image, sys.vid.screen, NULL);
  
  if(gui.DoButton(sys.vid.ScreenWidth()/2,55+4*40,sys.vid.ScreenWidth()-40,10,1,1,(char*)"Play")){state=PLAY;}
  if(gui.DoButton(sys.vid.ScreenWidth()/2,55+5*40,sys.vid.ScreenWidth()-40,10,1,1,(char*)"Credits")){state=CREDITS;}
  if(gui.DoButton(sys.vid.ScreenWidth()/2,55+6*40,sys.vid.ScreenWidth()-40,10,1,1,(char*)"Debug")){state=DEBUG;}
}

}
