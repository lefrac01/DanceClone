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
using Gooey::Container;
using Gooey::Element;
using Gooey::Button;
using Gooey::Image;
using Gooey::Label;
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
  bool displayDash;
  bool stateChanged;
  SDL_Surface* dashBg;
  Container oldGuiScreen;
  bool oldGuiCursorHidden;
  int screenWidth;
  int screenHeight;
  struct tm* dashTime;

  static const char* wdayNames[];
  static const char* monthNames[];
  static const char* dayNames[];

  static const int resumeTag = 1;
  static const int exitTag = 2;
  static const int restartTag = 3;
  static const int shutdownTag = 4;
  static const int screenshotTag = 5;
  
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
