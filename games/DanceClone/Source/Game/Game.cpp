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
platform(p)
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
/*
  soundAgent.Init();
  
  
  
#ifdef WIN
Mix_Music* music = NULL;
#endif

#include "setup/sprites.h"
extern float pixels_per_ms_at_1_bpm;

void Game_init()
{
  pixels_per_ms_at_1_bpm = rmode->viHeight / 4000.0 / 120.0;  
  
#ifdef WIN
  Mix_OpenAudio(0,0,0,0);
  //Mix_VolumeMusic(100);
  //Mix_PlayMusic(music, -1);
#endif

  Game_setup_sprites();
  
  goal_offset = rmode->viHeight / 4;
  */
  return true;
}

void Game::Cleanup()
{
  graphicsAgent.Cleanup();
}

void Game::Pump()
{
}

Game::GameState Game::State()
{
  return state;
}


}
