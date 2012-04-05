//      Input.cpp
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

#include "Input.h"

namespace Platform
{

void Input::Init()
{
  LOG(DEBUG_GUTS, "Input::Init(" << endl)
  //there is a way to reinit SDL adding joystick...
  //SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);
}

bool Input::ButtonDown(int c, InputChannel::InputButton b)
{
  if (c == -1)
  {
    for (unsigned int i = 0; i < inputChannels.size(); i++)
    {
      if (inputChannels[i].buttonDown[b])
      {
        LOG(DEBUG_GUTS, "Input::ButtonDown(" << c << ", b) returning true because of channel " << i << endl)
        return true;
      }
    }
  }
  else if (c >= 0 && c < (int)inputChannels.size())
  {
    LOG(DEBUG_GUTS, "Input::ButtonDown(" << c << ", b) returning channel[" << c << "].buttonDown[b] which is " << inputChannels[c].buttonDown[b] << endl)
    return inputChannels[c].buttonDown[b];
  }
  
  return false;
}

bool Input::DirectionDown(int c, InputChannel::InputDirection d)
{
  if (c == -1)
  {
    for (unsigned int i = 0; i < inputChannels.size(); i++)
    {
      if (inputChannels[i].directionDown[d])
      {
        LOG(DEBUG_GUTS, "Input::directionDown(" << c << ", d) returning true because of channel " << i << endl)
        return true;
      }
    }
  }
  else if (c >= 0 && c < (int)inputChannels.size())
  {
    LOG(DEBUG_GUTS, "Input::directionDown(" << c << ", d) returning channel[" << c << "].directionDown[d] which is " << inputChannels[c].directionDown[d] << endl)
    return inputChannels[c].directionDown[d];
  }
  
  return false;
}

void Input::Cleanup()
{
  
}

}
