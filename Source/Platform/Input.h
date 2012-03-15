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

#include <vector>
using std::vector;

#include <SDL.h>
#include "LOG.H"

namespace Platform
{

class InputChannel
{
public:
  enum InputDirection
  {
    LEFT,
    DOWN,
    UP,
    RIGHT,
    NUM_DIRECTIONS
  };
  
  enum InputButton
  {
    Button1,
    Button2,
    Button3,
    Button4,
    Button5,
    Button6,
    Button7,
    NUM_BUTTONS
  };
                    
  bool active;
  
  int cursorX;
  int cursorY;

  bool directionDown[NUM_DIRECTIONS];
  bool directionHeld[NUM_DIRECTIONS];
  long directionDownTime[NUM_DIRECTIONS];
//  bool directionUp[NUM_DIRECTIONS];
  bool buttonDown[NUM_BUTTONS];
  bool buttonHeld[NUM_BUTTONS];
//  bool buttonUp[NUM_BUTTONS];
};

class Input
{
public:

  virtual void Init();
  virtual void Update() = 0;
  virtual void Cleanup();

  vector<InputChannel> inputChannels;

  static const int keycount = 500;
  int keystate[keycount];
  static const int mousecount = 10;
  int mousestate[mousecount];

  bool ButtonDown(int c, InputChannel::InputButton b);
  bool ButtonHeld(int channel, InputChannel::InputButton b);
  bool DirectionDown(int channel, InputChannel::InputDirection d);
  bool DirectionUp(int channel, InputChannel::InputDirection d);
  bool CursorX(int channel);
  bool CursorY(int channel);
};

}
#endif
