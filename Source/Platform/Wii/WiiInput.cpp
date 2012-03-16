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
  scan &= chanbit;
  return scan != 0;
}

void WiiInput::Update()
{
  //bool wiimoteactive[maxChannels];
  u32 WiiButtonsHeld[maxChannels];
  u32 WiiButtonsDown[maxChannels];
  //u32 WiiButtonsUp[maxChannels];
  u16 GCButtonsHeld[maxChannels];
  u16 GCButtonsDown[maxChannels];
  //u16 GCButtonsUp[maxChannels];   // never received??
  ir_t ir[maxChannels];
  
  //Input::Update();
  LOG(DEBUG_GUTS, "WiiInput::Update()" << endl)
/*
  WPAD_ScanPads();
  for (int i = 0; i < maxChannels; i++)
  {
    InputChannel& chan = inputChannels[i];
    u32 ext;
    chan.active = (WPAD_Probe(i, &ext)!=WPAD_ERR_NO_CONTROLLER);
    WiiButtonsHeld[i] = WPAD_ButtonsHeld(i);
    WiiButtonsDown[i] = WPAD_ButtonsDown(i);
    //WiiButtonsUp[i] = WPAD_ButtonsUp(i);
    
    //#chan.buttonDown[InputChannel::Button1] = WiiButtonsDown[i] & WPAD_BUTTON_2;
    //#chan.buttonHeld[InputChannel::Button1] = WiiButtonsHeld[i] & WPAD_BUTTON_2;
    //#chan.buttonDown[InputChannel::Button2] = WiiButtonsDown[i] & WPAD_BUTTON_1;
    //#chan.buttonHeld[InputChannel::Button2] = WiiButtonsHeld[i] & WPAD_BUTTON_1;
    //#chan.buttonDown[InputChannel::Button3] = WiiButtonsDown[i] & WPAD_BUTTON_B;
    //#chan.buttonHeld[InputChannel::Button3] = WiiButtonsHeld[i] & WPAD_BUTTON_B;
    //#
    chan.buttonDown[InputChannel::Button4] = WiiButtonsDown[i] & WPAD_BUTTON_A;
    chan.buttonHeld[InputChannel::Button4] = WiiButtonsHeld[i] & WPAD_BUTTON_A;
    
    //#chan.buttonDown[InputChannel::Button5] = WiiButtonsDown[i] & WPAD_BUTTON_MINUS;
    //#chan.buttonHeld[InputChannel::Button5] = WiiButtonsHeld[i] & WPAD_BUTTON_MINUS;
    //#chan.buttonDown[InputChannel::Button6] = WiiButtonsDown[i] & WPAD_BUTTON_HOME;
    //#chan.buttonHeld[InputChannel::Button6] = WiiButtonsHeld[i] & WPAD_BUTTON_HOME;
    //#chan.buttonDown[InputChannel::Button7] = WiiButtonsDown[i] & WPAD_BUTTON_PLUS;
    //#chan.buttonHeld[InputChannel::Button7] = WiiButtonsHeld[i] & WPAD_BUTTON_PLUS;

    //#chan.directionDown[InputChannel::UP] =    WiiButtonsDown[i] & WPAD_BUTTON_UP;
    //#chan.directionDown[InputChannel::DOWN] =  WiiButtonsDown[i] & WPAD_BUTTON_DOWN;
    //#chan.directionDown[InputChannel::LEFT] =  WiiButtonsDown[i] & WPAD_BUTTON_LEFT;
    //#chan.directionDown[InputChannel::RIGHT] = WiiButtonsDown[i] & WPAD_BUTTON_RIGHT;
    //#chan.directionHeld[InputChannel::UP] =    WiiButtonsHeld[i] & WPAD_BUTTON_UP;
    //#chan.directionHeld[InputChannel::DOWN] =  WiiButtonsHeld[i] & WPAD_BUTTON_DOWN;
    //#chan.directionHeld[InputChannel::LEFT] =  WiiButtonsHeld[i] & WPAD_BUTTON_LEFT;
    //#chan.directionHeld[InputChannel::RIGHT] = WiiButtonsHeld[i] & WPAD_BUTTON_RIGHT;
    //#

    WPAD_IR(i, &ir[i]);
    if (ir[i].valid)
    {
      //NOTE: GUI-graphic-specific adjustments.  1.25 could be calculated relative to screen width / height vs cursor width / height
      //chan.cursorX = ir[i].x*1.25-80;
      //chan.cursorY = ir[i].y*1.25-60;
      chan.cursorX = ir[i].x;
      chan.cursorY = ir[i].y;
    }
  }
  LOG(DEBUG_GUTS, "wiimoteactive[0]:" << inputChannels[0].active << " [1]:" << inputChannels[1].active << endl)
*/


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
  
  //NOTE not considering dance mat buttons and wiimote buttons as separate
  // this would be unexpected for many other applications!!! but no time to
  // spare.. must get done quickly for DanceNight
  WPAD_ScanPads();
 u32 scanResult;
  scanResult = PAD_ScanPads();
  for (int i = 0; i < maxChannels; i++)
  {
    InputChannel& chan = inputChannels[i];
    u32 ext;
    chan.active = (WPAD_Probe(i, &ext)!=WPAD_ERR_NO_CONTROLLER);
    WiiButtonsHeld[i] = WPAD_ButtonsHeld(i);
    WiiButtonsDown[i] = WPAD_ButtonsDown(i);
    //WiiButtonsUp[i] = WPAD_ButtonsUp(i);
    
    WPAD_IR(i, &ir[i]);
    if (ir[i].valid)
    {
      //NOTE: GUI-graphic-specific adjustments.  1.25 could be calculated relative to screen width / height vs cursor width / height
      //chan.cursorX = ir[i].x*1.25-80;
      //chan.cursorY = ir[i].y*1.25-60;
      chan.cursorX = ir[i].x;
      chan.cursorY = ir[i].y;
    }
        
    GCButtonsHeld[i] = PAD_ButtonsHeld(i);
    GCButtonsDown[i] = PAD_ButtonsDown(i);
    //GCButtonsUp[i] = PAD_ButtonsUp(i);    // never received?????
    
    chan.danceMatActive = GCControllerPresent(scanResult, i);
    
    
    chan.buttonDown[InputChannel::Button4] = (GCButtonsDown[i] & PAD_BUTTON_A || WiiButtonsDown[i] & WPAD_BUTTON_A);
    chan.buttonDown[InputChannel::Button3] = GCButtonsDown[i] & PAD_BUTTON_B;
    chan.buttonDown[InputChannel::Button5] = GCButtonsDown[i] & PAD_TRIGGER_Z;
    chan.buttonDown[InputChannel::Button5] = GCButtonsDown[i] & PAD_BUTTON_MENU;
    chan.buttonDown[InputChannel::Button7] = GCButtonsDown[i] & PAD_BUTTON_START;

    chan.buttonHeld[InputChannel::Button4] = (GCButtonsHeld[i] & PAD_BUTTON_A || WiiButtonsHeld[i] & WPAD_BUTTON_A);
    chan.buttonHeld[InputChannel::Button3] = GCButtonsHeld[i] & PAD_BUTTON_B;
    chan.buttonHeld[InputChannel::Button5] = (GCButtonsHeld[i] & PAD_TRIGGER_Z || GCButtonsHeld[i] & PAD_BUTTON_MENU);
    chan.buttonHeld[InputChannel::Button7] = GCButtonsHeld[i] & PAD_BUTTON_START;
    //#chan.buttonHeld[InputChannel::Button3] = !chan.buttonHeld[InputChannel::Button4] && GCButtonsDown[i] & PAD_BUTTON_B;
    //#chan.buttonHeld[InputChannel::Button5] = !chan.buttonHeld[InputChannel::Button4] && (GCButtonsDown[i] & PAD_TRIGGER_Z || GCButtonsDown[i] & PAD_BUTTON_MENU);
    //#chan.buttonHeld[InputChannel::Button7] = !chan.buttonHeld[InputChannel::Button4] && GCButtonsDown[i] & PAD_BUTTON_START;
    //#chan.buttonHeld[InputChannel::Button3] = GCButtonsHeld[i] & PAD_BUTTON_B;
    //#chan.buttonHeld[InputChannel::Button5] = GCButtonsHeld[i] & PAD_TRIGGER_Z;
    //#chan.buttonHeld[InputChannel::Button5] = GCButtonsHeld[i] & PAD_BUTTON_MENU;
    //#chan.buttonHeld[InputChannel::Button7] = GCButtonsHeld[i] & PAD_BUTTON_START;

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
  
//LOG(DEBUG_BASIC, "inputChannels[0].mat active: " << inputChannels[0].danceMatActive << " inputChannels[0].directionHeld[InputChannel::LEFT]:" << inputChannels[0].directionHeld[InputChannel::LEFT] << endl)

}

void WiiInput::Cleanup()
{
  
}

}
