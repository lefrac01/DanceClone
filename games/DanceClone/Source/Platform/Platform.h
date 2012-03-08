//      Platform.h
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

#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <math.h>


#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <cstdlib>


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include "GUIDash.h"
#include "InputAgent.h"
#include "PlatformGraphicsAgent.h"

class Platform
{
private:

  GUIDash dash;
  InputAgent inputAgent;
  
public:

  PlatformGraphicsAgent platformGraphicsAgent;
  virtual void Init();
  virtual void Cleanup();
  virtual void Pump();
  bool DashVisible();
  void PumpDash();
  
  //TODO: not here
  void DrawButton(int x, int y, int w, int h, bool glow);
  bool DoButton(int x, int y, int w, int h, bool center, bool clickable, char* text);
};

#endif
