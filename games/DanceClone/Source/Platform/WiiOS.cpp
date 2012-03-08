//      WiiOS.cpp
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


#include "WiiOS.h"



namespace Platform
{

s8 HWButton;

void WiiOS::WiiResetPressed(){HWButton = SYS_RETURNTOMENU;}
void WiiOS::WiiPowerPressed(){HWButton = SYS_POWEROFF;}
void WiiOS::WiimotePowerPressed(s32 chan){HWButton = SYS_POWEROFF;}

void WiiOS::Init()
{  
  OS::Init();
  HWButton = -1;
  input.WiiSetScreenExtents(vid.ScreenWidth(), vid.ScreenHeight());
  //TODO: call this function until a successful file read can be done
  fatInitDefault();
  //TEMP
  sleep(2);

  WPAD_Init();
  WPAD_SetVRes(WPAD_CHAN_ALL,vid.ScreenWidth(),vid.ScreenHeight());  
  WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);  
  
  SYS_SetResetCallback(WiiResetPressed);
  SYS_SetPowerCallback(WiiPowerPressed);
  WPAD_SetPowerButtonCallback(WiimotePowerPressed);  
}

void WiiOS::Pump()
{
  OS::Pump();
//    SYS_SetResetCallback(WiiResetPressed);  //TODO:???
//    SYS_SetPowerCallback(WiiPowerPressed);  //TODO:???
//    WPAD_SetPowerButtonCallback(WiimotePowerPressed); //TODO:???
  if (HWButton != -1)
  {
    SYS_ResetSystem(HWButton, 0, 0);
  }
}

void WiiOS::Cleanup()
{
  OS::Cleanup();
  fatUnmount(0); 
}

}