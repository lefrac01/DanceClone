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

Game::Game(Platform& p) :
platform(p),
graphicsAgent(p)
{
}

bool Game::Init(string ConfigFilePath)
{
  try
  {
    constants.Init();
    graphicsAgent.Init();
  }
  catch (exception& e)
  {
    // log
    ofstream error_log;
    error_log.open("errors", std::ios_base::app);
    error_log << " Game::Init() failed due to exception: " << endl;
    error_log << e.what();
    error_log.close();
    return false;
  }

  soundAgent.Init();
  
  gameStateChanged = false;
  
  return true;
}

void Game::Cleanup()
{
  graphicsAgent.Cleanup();
}

void Game::Pump()
{
  graphicsAgent.DrawBackground();

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

void Game::RunTitleScreen()
{
  graphicsAgent.ApplySurface(platform.platformGraphicsAgent.ScreenWidth()/2-300/2, 30, graphicsAgent.title_image, platform.platformGraphicsAgent.screen, NULL);
  
  if(platform.DoButton(platform.platformGraphicsAgent.ScreenWidth()/2,55+4*40,platform.platformGraphicsAgent.ScreenWidth()-40,10,1,1,(char*)"Play")){state=PLAY;}
  if(platform.DoButton(platform.platformGraphicsAgent.ScreenWidth()/2,55+5*40,platform.platformGraphicsAgent.ScreenWidth()-40,10,1,1,(char*)"Credits")){state=CREDITS;}
  if(platform.DoButton(platform.platformGraphicsAgent.ScreenWidth()/2,55+6*40,platform.platformGraphicsAgent.ScreenWidth()-40,10,1,1,(char*)"Debug")){state=DEBUG;}
}

}
