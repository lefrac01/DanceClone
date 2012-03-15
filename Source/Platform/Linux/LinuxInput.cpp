//      LinuxInput.cpp
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

#include "LinuxInput.h"

namespace Platform
{

void LinuxInput::Init()
{
  LOG(DEBUG_GUTS, "Input::Init(" << endl)
  //there is a way to reinit SDL adding joystick...
  //SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);
  
  //allocate basic channel.
  inputChannels.resize(1);
  
  inputChannels[0].active = true;
}

void LinuxInput::Update()
{
  LOG(DEBUG_GUTS, "LinuxInput::Update()" << endl)

  //TODO: platform specific subclassing of input agent

  SDL_Event event;

  for(int a=0;a<keycount;a++)if(keystate[a]){keystate[a]=1;}else{keystate[a]=0;}
  while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYDOWN)))
    for(int a=0;a<keycount;a++)if(event.key.keysym.sym==a)keystate[a]=2;
  while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_KEYUP)))
    for(int a=0;a<keycount;a++)if(event.key.keysym.sym==a)keystate[a]=0;
/*
  for(int a=0;a<mousecount;a++)if(mousestate[a]){mousestate[a]=1;}else{mousestate[a]=0;}
  while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEBUTTONDOWN)))
    for(int a=0;a<mousecount;a++)if(event.button.button==a)mousestate[a]=2;
  while(SDL_PeepEvents(&event,1,SDL_GETEVENT,SDL_EVENTMASK(SDL_MOUSEBUTTONUP)))
    for(int a=0;a<mousecount;a++)if(event.button.button==a)mousestate[a]=0;
*/

  Uint8 mousestate = SDL_GetMouseState(&inputChannels[0].cursorX, &inputChannels[0].cursorY);
  inputChannels[0].buttonDown[InputChannel::Button4] = mousestate & SDL_BUTTON(1);    // Wiimote button A
  
  inputChannels[0].buttonDown[InputChannel::Button6] = keystate[SDLK_ESCAPE] == 2;
  inputChannels[0].directionDown[InputChannel::UP] = keystate[SDLK_UP] == 2;
  inputChannels[0].directionDown[InputChannel::DOWN] = keystate[SDLK_DOWN] == 2;
  inputChannels[0].directionDown[InputChannel::LEFT] = keystate[SDLK_LEFT] == 2;
  inputChannels[0].directionDown[InputChannel::RIGHT] = keystate[SDLK_RIGHT] == 2;
  
  inputChannels[0].directionHeld[InputChannel::UP] = keystate[SDLK_UP] == 1 || keystate[SDLK_UP] == 2 ;
  inputChannels[0].directionHeld[InputChannel::DOWN] = keystate[SDLK_DOWN] == 1 || keystate[SDLK_DOWN] == 2 ;
  inputChannels[0].directionHeld[InputChannel::LEFT] = keystate[SDLK_LEFT] == 1 || keystate[SDLK_LEFT] == 2 ;
  inputChannels[0].directionHeld[InputChannel::RIGHT] = keystate[SDLK_RIGHT] == 1 || keystate[SDLK_RIGHT] == 2 ;
}

void LinuxInput::Cleanup()
{
  
}

}
