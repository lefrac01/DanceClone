//      WiiInput.cpp
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

#include "WiiInput.h"

namespace Platform
{

void WiiInput::Init()
{
  LOG(DEBUG_GUTS, "Input::Init(" << endl)
  //there is a way to reinit SDL adding joystick...
  //SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);
  
  inputChannels.resize(maxChannels);

  for (unsigned int i = 0; i < inputChannels.size(); ++i)
  {
    InputChannel& chan = inputChannels[i];
    chan.number = i;
    for (unsigned int d = 0; d < InputChannel::NUM_DIRECTIONS; ++d)
    {
      chan.directionHeld[d] = false;
    }
  }
}

void WiiInput::WiiInputInit(int w, int h)
{
  WPAD_Init();
  WPAD_SetVRes(WPAD_CHAN_ALL, w, h);  
  WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);  
}

bool WiiInput::GCControllerPresent(u32 scan, int chan)
{
  u8 chanbit = 1 << chan;
  return (scan & chanbit) != 0;
}

void WiiInput::Update()
{
  u32 WiiButtonsHeld[maxChannels];
  u32 WiiButtonsDown[maxChannels];
  u16 GCButtonsHeld[maxChannels];
  u16 GCButtonsDown[maxChannels];
  ir_t ir[maxChannels];
  
  LOG(DEBUG_GUTS, "WiiInput::Update()" << endl)

  //NOTE: Wii GC Dance Mat to GC controller mappings:
  //minus PAD_TRIGGER_Z     }- both?
  //minus PAD_BUTTON_MENU   }
  //plus  PAD_BUTTON_START
  //B     PAD_BUTTON_B
  //A     PAD_BUTTON_A
  //LEFT  PAD_BUTTON_LEFT
  //RIGHT PAD_BUTTON_RIGHT
  //UP    PAD_BUTTON_UP
  //DOWN  PAD_BUTTON_DOWN
  
  WPAD_ScanPads();
  u32 scanResult = PAD_ScanPads();
  for (int i = 0; i < maxChannels; i++)
  {
    InputChannel& chan = inputChannels[i];
    u32 ext;
    chan.active = (WPAD_Probe(i, &ext)!=WPAD_ERR_NO_CONTROLLER);
    WiiButtonsHeld[i] = WPAD_ButtonsHeld(i);
    WiiButtonsDown[i] = WPAD_ButtonsDown(i);
    
    WPAD_IR(i, &ir[i]);
    if (ir[i].valid)
    {
      chan.cursorX = ir[i].x;
      chan.cursorY = ir[i].y;
    }
        
    GCButtonsHeld[i] = PAD_ButtonsHeld(i);
    GCButtonsDown[i] = PAD_ButtonsDown(i);
    
    chan.danceMatActive = GCControllerPresent(scanResult, i);
    
    
    chan.buttonDown[InputChannel::Button4] = (GCButtonsDown[i] & PAD_BUTTON_A || WiiButtonsDown[i] & WPAD_BUTTON_A);
    chan.buttonDown[InputChannel::Button3] = GCButtonsDown[i] & PAD_BUTTON_B;
    chan.buttonDown[InputChannel::Button5] = GCButtonsDown[i] & PAD_TRIGGER_Z;
    chan.buttonDown[InputChannel::Button5] = GCButtonsDown[i] & PAD_BUTTON_MENU;
    chan.buttonDown[InputChannel::Button6] = WiiButtonsDown[i] & WPAD_BUTTON_HOME;
    chan.buttonHeld[InputChannel::Button6] = WiiButtonsHeld[i] & WPAD_BUTTON_HOME;
    chan.buttonDown[InputChannel::Button7] = GCButtonsDown[i] & PAD_BUTTON_START;

    chan.buttonHeld[InputChannel::Button4] = (GCButtonsHeld[i] & PAD_BUTTON_A || WiiButtonsHeld[i] & WPAD_BUTTON_A);
    chan.buttonHeld[InputChannel::Button3] = GCButtonsHeld[i] & PAD_BUTTON_B;
    chan.buttonHeld[InputChannel::Button5] = (GCButtonsHeld[i] & PAD_TRIGGER_Z || GCButtonsHeld[i] & PAD_BUTTON_MENU);
    chan.buttonHeld[InputChannel::Button7] = GCButtonsHeld[i] & PAD_BUTTON_START;

    chan.buttonHeld[InputChannel::Button4] = !chan.buttonHeld[InputChannel::Button4] && GCButtonsDown[i] & PAD_BUTTON_A;
    
    chan.directionDown[InputChannel::UP]   = GCButtonsDown[i] & PAD_BUTTON_UP;
    chan.directionDown[InputChannel::DOWN] = GCButtonsDown[i] & PAD_BUTTON_DOWN;
    chan.directionDown[InputChannel::LEFT] = GCButtonsDown[i] & PAD_BUTTON_LEFT;
    chan.directionDown[InputChannel::RIGHT] = GCButtonsDown[i] & PAD_BUTTON_RIGHT;

    chan.directionHeld[InputChannel::UP]   = GCButtonsHeld[i] & PAD_BUTTON_UP;
    chan.directionHeld[InputChannel::DOWN] = GCButtonsHeld[i] & PAD_BUTTON_DOWN;
    chan.directionHeld[InputChannel::LEFT] = GCButtonsHeld[i] & PAD_BUTTON_LEFT;
    chan.directionHeld[InputChannel::RIGHT] = GCButtonsHeld[i] & PAD_BUTTON_RIGHT;
  }
}

void WiiInput::Cleanup()
{
  
}

}
