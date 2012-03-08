//      Platform.cpp
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


#include "Platform.h"

void Platform::Init()
{
  srand((int)time(NULL));

  platformGraphicsAgent.Init();
  
  // this was in generic.. so supposed to be multiplatform...
  //  WiiDash_init();
  dash.Init();

  SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
  SDL_ShowCursor(SDL_DISABLE);
}

void Platform::Pump()
{
  SDL_PumpEvents();
  platformGraphicsAgent.Pump();
  inputAgent.Update();    //updatecontrolinput();
}

void Platform::Cleanup()
{
  dash.Cleanup();
  SDL_Quit();
}

bool Platform::DashVisible()
{
  return dash.Visible();
}

void Platform::PumpDash()
{
  dash.Update();
}

//TODO: not here
void Platform::DrawButton(int x, int y,int w,int h,bool glow)
{
  //corners
  platformGraphicsAgent.ApplySurface(x-21, y-21, platformGraphicsAgent.WDbuttonimage, platformGraphicsAgent.screen, &platformGraphicsAgent.WDbuttonframes[0+glow*9]);
  platformGraphicsAgent.ApplySurface(x-21, y+h,  platformGraphicsAgent.WDbuttonimage, platformGraphicsAgent.screen, &platformGraphicsAgent.WDbuttonframes[1+glow*9]);
  platformGraphicsAgent.ApplySurface(x+w,  y-21, platformGraphicsAgent.WDbuttonimage, platformGraphicsAgent.screen, &platformGraphicsAgent.WDbuttonframes[2+glow*9]);
  platformGraphicsAgent.ApplySurface(x+w,  y+h,  platformGraphicsAgent.WDbuttonimage, platformGraphicsAgent.screen, &platformGraphicsAgent.WDbuttonframes[3+glow*9]);
  //edges
  for(int a=0; a<w/21; a++)
    platformGraphicsAgent.ApplySurface(x+a*21,y+h,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&platformGraphicsAgent.WDbuttonframes[4+glow*9]);
  SDL_Rect temprect1={4*21,glow*21,w-w/21*21,21};
  platformGraphicsAgent.ApplySurface(x+w/21*21,y+h,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect1);
  for(int a=0; a<h/21; a++)
    platformGraphicsAgent.ApplySurface(x+w,y+a*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&platformGraphicsAgent.WDbuttonframes[5+glow*9]);
  SDL_Rect temprect2={5*21,glow*21,21,h-h/21*21};
  platformGraphicsAgent.ApplySurface(x+w,y+h/21*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect2);
  for(int a=0; a<w/21; a++)
    platformGraphicsAgent.ApplySurface(x+a*21,y-21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&platformGraphicsAgent.WDbuttonframes[6+glow*9]);
  SDL_Rect temprect3={6*21,glow*21,w-w/21*21,21};
  platformGraphicsAgent.ApplySurface(x+w/21*21,y-21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect3);
  for(int a=0; a<h/21; a++)
    platformGraphicsAgent.ApplySurface(x-21,y+a*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&platformGraphicsAgent.WDbuttonframes[7+glow*9]);
  SDL_Rect temprect4={7*21,glow*21,21,h-h/21*21};
  platformGraphicsAgent.ApplySurface(x-21,y+h/21*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect4);
  //insides
  for(int a=0; a<h/21; a++)for(int b=0; b<w/21; b++)
    platformGraphicsAgent.ApplySurface(x+b*21,y+a*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&platformGraphicsAgent.WDbuttonframes[8+glow*9]);
  SDL_Rect temprect5={8*21,glow*21,w-w/21*21,21};
  for(int a=0; a<h/21; a++)
    platformGraphicsAgent.ApplySurface(x+w/21*21,y+a*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect5);
  SDL_Rect temprect6={8*21,glow*21,21,h-h/21*21};
  for(int a=0; a<w/21; a++)
    platformGraphicsAgent.ApplySurface(x+a*21,y+h/21*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect6);
  SDL_Rect temprect7={8*21,glow*21,w-w/21*21,h-h/21*21};
  platformGraphicsAgent.ApplySurface(x+w/21*21,y+h/21*21,platformGraphicsAgent.WDbuttonimage,platformGraphicsAgent.screen,&temprect7);
}

bool Platform::DoButton(int x, int y,int w,int h,bool center,bool clickable,char* text)
{
  bool clicked=0;
  bool glow=0;
  if(center){x=x-w/2;y=y-h/2;}

  #ifdef WIN
  if(clickable && cursorx[0]>x-14 && cursory[0]>y-14 && cursorx[0]<x+w+13 && cursory[0]<y+h+13){
    if(mousestate[SDL_BUTTON_LEFT]==2)clicked=1;
    glow=1;}
  #endif

  #ifdef WII
  if(clickable)for(int a=0;a<4;a++){
    if(WDusecursor[a] && cursorx[a]>x-14 && cursory[a]>y-14 && cursorx[a]<x+w+13 && cursory[a]<y+h+13){
      if(WiiButtonsDown[a] & WPAD_BUTTON_A)clicked=1;
      if(WiiButtonsDown[a] & WPAD_CLASSIC_BUTTON_A)clicked=1;
      glow=1;
    }
  }
  #endif

  DrawButton(x,y,w,h,glow);

  x=x+w/2;y=y+h/2;
  platformGraphicsAgent.DrawSpriteText(x,y-21/2,text,2);
  return clicked;
}
