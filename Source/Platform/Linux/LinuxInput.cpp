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
  inputChannels.resize(2);  // TEMP fake 2 players
  
  inputChannels[0].active = true;
  inputChannels[0].danceMatActive = true;   // using keyboard

  //TEMP: this would have to be configured in a menu
  inputChannels[1].active = true;                             
  inputChannels[1].danceMatActive = true;   // using keyboard
}

void LinuxInput::Update()
{
  LOG(DEBUG_GUTS, "LinuxInput::Update()" << endl)

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

  // just like C64 :)
  static bool debounceMouse = false;
  Uint8 mousestate = SDL_GetMouseState(&inputChannels[0].cursorX, &inputChannels[0].cursorY);
  
  if (debounceMouse)
  {
    inputChannels[0].buttonDown[InputChannel::Button4] = false;
    if ((mousestate & SDL_BUTTON(1)) == 0)    // Wiimote button A
    {
      debounceMouse = false;
    }
  }
  else
  {
    if (mousestate & SDL_BUTTON(1))
    {
      inputChannels[0].buttonDown[InputChannel::Button4] = true;    // Wiimote button A
      debounceMouse = true;
    }
  }
  inputChannels[0].buttonDown[InputChannel::Button6] = keystate[SDLK_ESCAPE] == 2;
  inputChannels[0].directionDown[InputChannel::UP] = keystate[SDLK_UP] == 2;
  inputChannels[0].directionDown[InputChannel::DOWN] = keystate[SDLK_DOWN] == 2;
  inputChannels[0].directionDown[InputChannel::LEFT] = keystate[SDLK_LEFT] == 2;
  inputChannels[0].directionDown[InputChannel::RIGHT] = keystate[SDLK_RIGHT] == 2;
  
  inputChannels[0].directionHeld[InputChannel::UP] = keystate[SDLK_UP] == 1 || keystate[SDLK_UP] == 2 ;
  inputChannels[0].directionHeld[InputChannel::DOWN] = keystate[SDLK_DOWN] == 1 || keystate[SDLK_DOWN] == 2 ;
  inputChannels[0].directionHeld[InputChannel::LEFT] = keystate[SDLK_LEFT] == 1 || keystate[SDLK_LEFT] == 2 ;
  inputChannels[0].directionHeld[InputChannel::RIGHT] = keystate[SDLK_RIGHT] == 1 || keystate[SDLK_RIGHT] == 2 ;
  
  inputChannels[0].buttonDown[InputChannel::Button4] |= keystate[SDLK_RETURN] == 2;   // A button
  inputChannels[0].buttonHeld[InputChannel::Button5] = keystate[SDLK_MINUS] == 1;   // Abort keys
  inputChannels[0].buttonHeld[InputChannel::Button7] = keystate[SDLK_EQUALS] == 1;    // Abort keys
  
  //TEMP: player 2 testing:
  inputChannels[1].directionDown[InputChannel::UP] = keystate[SDLK_w] == 2;
  inputChannels[1].directionDown[InputChannel::DOWN] = keystate[SDLK_s] == 2;
  inputChannels[1].directionDown[InputChannel::LEFT] = keystate[SDLK_a] == 2;
  inputChannels[1].directionDown[InputChannel::RIGHT] = keystate[SDLK_d] == 2;
  inputChannels[1].directionHeld[InputChannel::UP] = keystate[SDLK_w] == 1 || keystate[SDLK_w] == 2 ;
  inputChannels[1].directionHeld[InputChannel::DOWN] = keystate[SDLK_s] == 1 || keystate[SDLK_s] == 2 ;
  inputChannels[1].directionHeld[InputChannel::LEFT] = keystate[SDLK_a] == 1 || keystate[SDLK_a] == 2 ;
  inputChannels[1].directionHeld[InputChannel::RIGHT] = keystate[SDLK_d] == 1 || keystate[SDLK_d] == 2 ;
  
  inputChannels[1].buttonDown[InputChannel::Button4] = keystate[SDLK_q] == 2;   // A button


}

void LinuxInput::Cleanup()
{
  
}

}
