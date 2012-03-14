//      Dash.h
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

#ifndef DASH_H
#define DASH_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "../Platform/OS.h"
using Platform::OS;
#include "../Platform/LOG.H"
#include "../Platform/Tools.h"
#include "../GUI/GUI.h"
using Gooey::GUI;
#include "DashGraphics.h"
#ifdef WII
#include <gccore.h>
#endif

class Dash
{
private:
  
  Dash();
  OS& sys;
  GUI& gui;
  DashGraphics gfx;
  bool userWantsOut;
  double onOffPercent;
  bool onOff;
  Uint8* prgb1;
  Uint8* prgb2;
  int screenWidth;
  int screenHeight;
  struct tm* dashTime;

  static const char* wdayNames[];
  static const char* monthNames[];
  static const char* dayNames[];

  int runTime;
  int gameTime;
  int frameTime;
  int framerate;
  double timespeed;

  char extraCredits[100];
  Uint8 In8bitRange(int num);
  
public:

  Dash(OS& os, GUI& g);
  bool Init();
  void Run();
  void Cleanup();
  bool Visible();
  int OnOffSlide(int onPos, int offPos);
  void RunOnOff();
  void RunMenu();
  void RunTopBottomBars();
  bool UserWantsOut();
};

#endif
