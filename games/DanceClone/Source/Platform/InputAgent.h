//      InputAgent.h
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

#ifndef INPUTAGENT_H
#define INPUTAGENT_H


#include <SDL/SDL.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

class InputAgent
{
private:

  int cursorx[4];
  int cursory[4];

  static const int keycount=500;
  int keystate[keycount];
  static const int mousecount = 10;
  int mousestate[mousecount];

#ifdef WII
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
  s8 HWButton;
#endif

public:
  void Init();
  void Update();
  void Cleanup();
};

#endif
