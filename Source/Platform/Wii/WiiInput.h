//      WiiInput.h
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

#ifndef WIIINPUT_H
#define WIIINPUT_H

//#define USEACCELEROMETER

#include <SDL.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include "../LOG.H"

#include "../Input.h"

namespace Platform
{

class WiiInput : public Input
{
private:
  bool GCControllerPresent(u32 scan, int chan);
  
public:

  const static int maxChannels = WPAD_MAX_WIIMOTES;


  virtual void Init();
  virtual void Update();
  virtual void Cleanup();

  void WiiInputInit(int w, int h);
};

}
#endif
