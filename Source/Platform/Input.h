//TODO: correct cross-platform control input needs some thought.  for the wii,
// gamecube pads should be present.  however if for example 2 player mode is
// supported in a mac os version then the base input class should be able to
// send out player inputs that were gathered by a subclass-specific method.
// if the base class stores multiple player inputs in a generic format and
// the subclasses update these base class data per their specific gathering 
// method, then it should all work.

// i.e. make a WiiInput subclass to encapsulate current code 
// and devise the base class generic data type


//      Input.h
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

#ifndef INPUT_H
#define INPUT_H


#include <SDL/SDL.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "LOG.H"

namespace Platform
{

class Input
{
private:

  //TODO: subclassing
  int screenWidth;
  int screenHeight;

  
public:

  void Init();
  void Update();
  void Cleanup();

  int cursorx[4];
  int cursory[4];

  static const int keycount = 500;
  int keystate[keycount];
  static const int mousecount = 10;
  int mousestate[mousecount];

//TODO: subclassing
  void WiiSetScreenExtents(int w, int h);
//#ifdef WII
  bool wiimoteactive[4];
  u32 WiiButtonsHeld[4];
  u32 WiiButtonsDown[4];
  u32 WiiButtonsUp[4];
  u16 GCButtonsHeld[4];
  u16 GCButtonsDown[4];
  u16 GCButtonsUp[4];
  ir_t ir[4];
  expansion_t expans[4];
  #ifdef USEACCELEROMETER
  gforce_t gforce[4];
  #endif
//#endif


};

}
#endif
