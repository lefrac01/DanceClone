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
  
  //allocate basic channel.  no, not in base class
  //inputChannels.resize(1);
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


/*
void Input::Update()
{
  LOG(DEBUG_GUTS, "Input::Update()" << endl)

  //TODO: platform specific subclassing of input agent

  //#SDL_Event event;
//#
  //#for(int a=0;a<keycount;a++)if(keystate[a]){keystate[a]=1;}else{keystate[a]=0;}
  //#while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYDOWN)))
    //#for(int a=0;a<keycount;a++)if(event.key.keysym.sym==a)keystate[a]=2;
  //#while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYUP)))
    //#for(int a=0;a<keycount;a++)if(event.key.keysym.sym==a)keystate[a]=0;
//#
  //#for(int a=0;a<mousecount;a++)if(mousestate[a]){mousestate[a]=1;}else{mousestate[a]=0;}
  //#while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)))
    //#for(int a=0;a<mousecount;a++)if(event.button.button==a)mousestate[a]=2;
  //#while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEBUTTONUP)))
    //#for(int a=0;a<mousecount;a++)if(event.button.button==a)mousestate[a]=0;

//TODO: subclassing per platform
//  #ifdef WIN
//    Uint8 mousestate=SDL_GetMouseState(&cursorx[0],&cursory[0]);
//  #endif
//  #ifdef WII

    //#WPAD_ScanPads();
    //#u32 ext;
    //#wiimoteactive[0]=(WPAD_Probe(WPAD_CHAN_0,&ext)!=WPAD_ERR_NO_CONTROLLER);
    //#wiimoteactive[1]=(WPAD_Probe(WPAD_CHAN_1,&ext)!=WPAD_ERR_NO_CONTROLLER);
    //#wiimoteactive[2]=(WPAD_Probe(WPAD_CHAN_2,&ext)!=WPAD_ERR_NO_CONTROLLER);
    //#wiimoteactive[3]=(WPAD_Probe(WPAD_CHAN_3,&ext)!=WPAD_ERR_NO_CONTROLLER);
//#//    LOG(DEBUG_GUTS, "wiimoteactive[0]:" << wiimoteactive[0] << " [1]:" << wiimoteactive[1] << " [2]:" << wiimoteactive[2] << "[3]:" << wiimoteactive[3] << endl)
//#
    //#WiiButtonsHeld[0]=WPAD_ButtonsHeld(WPAD_CHAN_0);
    //#WiiButtonsDown[0]=WPAD_ButtonsDown(WPAD_CHAN_0);
    //#WiiButtonsUp[0]=WPAD_ButtonsUp(WPAD_CHAN_0);
    //#WiiButtonsHeld[1]=WPAD_ButtonsHeld(WPAD_CHAN_1);
    //#WiiButtonsDown[1]=WPAD_ButtonsDown(WPAD_CHAN_1);
    //#WiiButtonsUp[1]=WPAD_ButtonsUp(WPAD_CHAN_1);
    //#WiiButtonsHeld[2]=WPAD_ButtonsHeld(WPAD_CHAN_2);
    //#WiiButtonsDown[2]=WPAD_ButtonsDown(WPAD_CHAN_2);
    //#WiiButtonsUp[2]=WPAD_ButtonsUp(WPAD_CHAN_2);
    //#WiiButtonsHeld[3]=WPAD_ButtonsHeld(WPAD_CHAN_3);
    //#WiiButtonsDown[3]=WPAD_ButtonsDown(WPAD_CHAN_3);
    //#WiiButtonsUp[3]=WPAD_ButtonsUp(WPAD_CHAN_3);
    //#WPAD_IR(WPAD_CHAN_0,&ir[0]);
    //#WPAD_IR(WPAD_CHAN_1,&ir[1]);
    //#WPAD_IR(WPAD_CHAN_2,&ir[2]);
    //#WPAD_IR(WPAD_CHAN_3,&ir[3]);
//#//    LOG(DEBUG_GUTS, "ir[0].valid:" << ir[0].valid << " [1]:" << ir[1].valid << " [2]:" << ir[2].valid << "[3]:" << ir[3].valid << endl)
    //#
    //#WPAD_Expansion(WPAD_CHAN_0,&expans[0]);
    //#WPAD_Expansion(WPAD_CHAN_1,&expans[1]);
    //#WPAD_Expansion(WPAD_CHAN_2,&expans[2]);
    //#WPAD_Expansion(WPAD_CHAN_3,&expans[3]);
    //##ifdef USEACCELEROMETER
    //#WPAD_GForce(0,&gforce[0]);
    //#WPAD_GForce(1,&gforce[1]);
    //#WPAD_GForce(2,&gforce[2]);
    //#WPAD_GForce(3,&gforce[3]);
    //##endif
    //#PAD_ScanPads();
    //#GCButtonsHeld[0]=PAD_ButtonsHeld(0);
    //#GCButtonsDown[0]=PAD_ButtonsDown(0);
    //#GCButtonsUp[0]=PAD_ButtonsUp(0);
    //#if(ir[0].valid){cursorx[0]=ir[0].x*1.25-80;cursory[0]=ir[0].y*1.25-60;}
    //#if(ir[1].valid){cursorx[1]=ir[1].x*1.25-80;cursory[1]=ir[1].y*1.25-60;}
    //#if(ir[2].valid){cursorx[2]=ir[2].x*1.25-80;cursory[2]=ir[2].y*1.25-60;}
    //#if(ir[3].valid){cursorx[3]=ir[3].x*1.25-80;cursory[3]=ir[3].y*1.25-60;}
//#//  LOG(DEBUG_GUTS, "raw irx y: " << ir[0].x << " " << ir[0].y << " cursorx y:" << cursorx[0] << " " << cursory[0] << endl)
    //#for(int a=0;a<4;a++){
      //#if(cursorx[a]<0)cursorx[a]=0;
      //#if(cursorx[a]>screenWidth)cursorx[a]=screenWidth;
      //#if(cursory[a]<0)cursory[a]=0;
      //#if(cursory[a]>screenHeight)cursory[a]=screenHeight;
    //#}
//#//    LOG(DEBUG_GUTS, "fin cursorx[0]:" << cursorx[0] << " cursory[0]:" << cursory[0] << endl)
//#//  #endif

}
*/
void Input::Cleanup()
{
  
}

}
