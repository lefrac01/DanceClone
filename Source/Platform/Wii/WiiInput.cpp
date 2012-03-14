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
  
  //Wii has 4 input channels
  inputChannels.resize(maxChannels);

}

void WiiInput::WiiInputInit(int w, int h)
{
  WPAD_Init();
  WPAD_SetVRes(WPAD_CHAN_ALL, w, h);  
  WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);  
}

void WiiInput::Update()
{
  //bool wiimoteactive[maxChannels];
  u32 WiiButtonsHeld[maxChannels];
  u32 WiiButtonsDown[maxChannels];
  //u32 WiiButtonsUp[maxChannels];
  u16 GCButtonsHeld[maxChannels];
  u16 GCButtonsDown[maxChannels];
  //u16 GCButtonsUp[maxChannels];  
  ir_t ir[maxChannels];
  
  //Input::Update();
  LOG(DEBUG_GUTS, "WiiInput::Update()" << endl)

  WPAD_ScanPads();
  for (int i = 0; i < maxChannels; i++)
  {
    InputChannel& chan = inputChannels[i];
    u32 ext;
    chan.active = (WPAD_Probe(i, &ext)!=WPAD_ERR_NO_CONTROLLER);
    WiiButtonsHeld[i] = WPAD_ButtonsHeld(i);
    WiiButtonsDown[i] = WPAD_ButtonsDown(i);
    //WiiButtonsUp[i] = WPAD_ButtonsUp(i);
    chan.buttonDown[InputChannel::Button1] = WiiButtonsDown[i] & WPAD_BUTTON_2;
    chan.buttonHeld[InputChannel::Button1] = WiiButtonsHeld[i] & WPAD_BUTTON_2;
    chan.buttonDown[InputChannel::Button2] = WiiButtonsDown[i] & WPAD_BUTTON_1;
    chan.buttonHeld[InputChannel::Button2] = WiiButtonsHeld[i] & WPAD_BUTTON_1;
    chan.buttonDown[InputChannel::Button3] = WiiButtonsDown[i] & WPAD_BUTTON_B;
    chan.buttonHeld[InputChannel::Button3] = WiiButtonsHeld[i] & WPAD_BUTTON_B;
    chan.buttonDown[InputChannel::Button4] = WiiButtonsDown[i] & WPAD_BUTTON_A;
    chan.buttonHeld[InputChannel::Button4] = WiiButtonsHeld[i] & WPAD_BUTTON_A;
    chan.buttonDown[InputChannel::Button5] = WiiButtonsDown[i] & WPAD_BUTTON_MINUS;
    chan.buttonHeld[InputChannel::Button5] = WiiButtonsHeld[i] & WPAD_BUTTON_MINUS;
    chan.buttonDown[InputChannel::Button6] = WiiButtonsDown[i] & WPAD_BUTTON_HOME;
    chan.buttonHeld[InputChannel::Button6] = WiiButtonsHeld[i] & WPAD_BUTTON_HOME;
    chan.buttonDown[InputChannel::Button7] = WiiButtonsDown[i] & WPAD_BUTTON_PLUS;
    chan.buttonHeld[InputChannel::Button7] = WiiButtonsHeld[i] & WPAD_BUTTON_PLUS;



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


  //NOTE not considering dance mat buttons and wiimote buttons as separate
  // this would be unexpected for many other applications!!! but no time to
  // spare.. must get done quickly for DanceNight
  PAD_ScanPads();
  for (int i = 0; i < maxChannels; i++)
  {
    InputChannel& chan = inputChannels[i];
    GCButtonsHeld[i] = PAD_ButtonsHeld(i);
    GCButtonsDown[i] = PAD_ButtonsDown(i);
    //GCButtonsUp[i] = PAD_ButtonsUp(i);
    
    chan.buttonDown[InputChannel::Button4] |= GCButtonsDown[i] & PAD_BUTTON_A;
    chan.buttonHeld[InputChannel::Button4] |= GCButtonsHeld[i] & PAD_BUTTON_A;
    chan.buttonDown[InputChannel::Button3] |= GCButtonsDown[i] & PAD_BUTTON_B;
    chan.buttonHeld[InputChannel::Button3] |= GCButtonsHeld[i] & PAD_BUTTON_B;
  }
    
/*
    u32 ext;
    wiimoteactive[0]=(WPAD_Probe(WPAD_CHAN_0,&ext)!=WPAD_ERR_NO_CONTROLLER);
    wiimoteactive[1]=(WPAD_Probe(WPAD_CHAN_1,&ext)!=WPAD_ERR_NO_CONTROLLER);
    wiimoteactive[2]=(WPAD_Probe(WPAD_CHAN_2,&ext)!=WPAD_ERR_NO_CONTROLLER);
    wiimoteactive[3]=(WPAD_Probe(WPAD_CHAN_3,&ext)!=WPAD_ERR_NO_CONTROLLER);
    
//    LOG(DEBUG_GUTS, "wiimoteactive[0]:" << wiimoteactive[0] << " [1]:" << wiimoteactive[1] << " [2]:" << wiimoteactive[2] << "[3]:" << wiimoteactive[3] << endl)


    WiiButtonsHeld[0]=WPAD_ButtonsHeld(WPAD_CHAN_0);
    WiiButtonsDown[0]=WPAD_ButtonsDown(WPAD_CHAN_0);
    WiiButtonsUp[0]=WPAD_ButtonsUp(WPAD_CHAN_0);
    WiiButtonsHeld[1]=WPAD_ButtonsHeld(WPAD_CHAN_1);
    WiiButtonsDown[1]=WPAD_ButtonsDown(WPAD_CHAN_1);
    WiiButtonsUp[1]=WPAD_ButtonsUp(WPAD_CHAN_1);
    WiiButtonsHeld[2]=WPAD_ButtonsHeld(WPAD_CHAN_2);
    WiiButtonsDown[2]=WPAD_ButtonsDown(WPAD_CHAN_2);
    WiiButtonsUp[2]=WPAD_ButtonsUp(WPAD_CHAN_2);
    WiiButtonsHeld[3]=WPAD_ButtonsHeld(WPAD_CHAN_3);
    WiiButtonsDown[3]=WPAD_ButtonsDown(WPAD_CHAN_3);
    WiiButtonsUp[3]=WPAD_ButtonsUp(WPAD_CHAN_3);
    WPAD_IR(WPAD_CHAN_0,&ir[0]);
    WPAD_IR(WPAD_CHAN_1,&ir[1]);
    WPAD_IR(WPAD_CHAN_2,&ir[2]);
    WPAD_IR(WPAD_CHAN_3,&ir[3]);
//    LOG(DEBUG_GUTS, "ir[0].valid:" << ir[0].valid << " [1]:" << ir[1].valid << " [2]:" << ir[2].valid << "[3]:" << ir[3].valid << endl)
    
    WPAD_Expansion(WPAD_CHAN_0,&expans[0]);
    WPAD_Expansion(WPAD_CHAN_1,&expans[1]);
    WPAD_Expansion(WPAD_CHAN_2,&expans[2]);
    WPAD_Expansion(WPAD_CHAN_3,&expans[3]);
    #ifdef USEACCELEROMETER
    WPAD_GForce(0,&gforce[0]);
    WPAD_GForce(1,&gforce[1]);
    WPAD_GForce(2,&gforce[2]);
    WPAD_GForce(3,&gforce[3]);
    #endif
    PAD_ScanPads();
    GCButtonsHeld[0]=PAD_ButtonsHeld(0);
    GCButtonsDown[0]=PAD_ButtonsDown(0);
    GCButtonsUp[0]=PAD_ButtonsUp(0);
    if(ir[0].valid){cursorx[0]=ir[0].x*1.25-80;cursory[0]=ir[0].y*1.25-60;}
    if(ir[1].valid){cursorx[1]=ir[1].x*1.25-80;cursory[1]=ir[1].y*1.25-60;}
    if(ir[2].valid){cursorx[2]=ir[2].x*1.25-80;cursory[2]=ir[2].y*1.25-60;}
    if(ir[3].valid){cursorx[3]=ir[3].x*1.25-80;cursory[3]=ir[3].y*1.25-60;}
//  LOG(DEBUG_GUTS, "raw irx y: " << ir[0].x << " " << ir[0].y << " cursorx y:" << cursorx[0] << " " << cursory[0] << endl)
*/

//TODO: screen clipping not to be done at this low a level
// GUI class can do this if it wants...
/*
    for(int a=0;a<4;a++){
      if(cursorx[a]<0)cursorx[a]=0;
      if(cursorx[a]>screenWidth)cursorx[a]=screenWidth;
      if(cursory[a]<0)cursory[a]=0;
      if(cursory[a]>screenHeight)cursory[a]=screenHeight;
    }
    */
//    LOG(DEBUG_GUTS, "fin cursorx[0]:" << cursorx[0] << " cursory[0]:" << cursory[0] << endl)
//  #endif
}

void WiiInput::Cleanup()
{
  
}

}
