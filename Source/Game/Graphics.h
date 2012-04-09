//      Graphics.h
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
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
using std::string;
#include <SDL_rotozoom.h>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <exception>
using std::exception;

#include "../Platform/OS.h"
using Platform::OS;

#include "Constants.h"

namespace DanceClone
{

class Graphics
{
public:
  enum Images
  {
    DefaultBg,
    SongSelectBg,
    ScoreBg,
    Title,
    ComboLabel,
    ComboNumbers,
    OkNg,
    Ratings,
    DefaultBanner,
    DefaultBannerMini,
    GetReady,
    DifficultyCursor,
    HomeArrows,
    QuarterArrows,
    EighthArrows,
    QuarterTripletArrows,
    SixteenthArrows,
    EighthTripletArrows,
    ThirtysecondArrows,
    SixteenthTripletArrows,
    SixtyfourthArrows,
    SixtyfourthTripletArrows,
    FreezeArrowsBody,
    FreezeArrowsTail,
    FreezeArrowsHead,
    MarvellousHit,
    PerfectHit,
    GreatHit,
    GoodHit,
    ComboHit,
    NUM_IMAGES
  };
  
private:

  OS& sys;
  void ExpandArrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames, int src_cols);
  void SetArrowFrame(SDL_Rect* dest, int index, int x, int y, int w, int h);
  Graphics();
  
public:

  int screenWidth;
  int screenHeight;
  SDL_Surface* screen;
  Uint8* pWDrgb1;
  Uint8* pWDrgb2;

  vector<SDL_Surface*> images;
  SDL_Rect difficultyCursorFrames[4];
  SDL_Rect comboNumbersFrames[10];
  SDL_Rect okNgFrames[2];
  SDL_Rect ratingsFrames[7];
  
  SDL_Rect homeArrowsFrames[24];
  // all non-hold arrows have same layout so use only one frames array
  SDL_Rect arrowsFrames[16];
  SDL_Rect freezeBodyFrames[12];
  SDL_Rect freezeTailFrames[12];
  SDL_Rect freezeHeadFrames[16];

  SDL_Rect arrowsHitFrames[32];

  int arrowWidth;
  int arrowHeight;

  Uint32 sdlWhite;
  Uint32 sdlBlack;
  Uint32 sdlDarkGrey;
  Uint32 sdlGrey;
  Uint32 sdlLightGrey;
  Uint32 sdlTeal;
  Uint32 sdlRed;
  Uint32 sdlDarkRed;
  Uint32 sdlGreen;
  Uint32 sdlMediumGreen;
  Uint32 sdlDarkGreen;
  Uint32 sdlLightGreen;
  Uint32 sdlBlue;
  Uint32 sdlYellow;
  Uint32 sdlCyan;
  Uint32 sdlLightBlue;
  Uint32 sdlBlueWhite;

  Graphics(OS& os);
  bool Init(string configFilePath = "");
  void Cleanup();
};

}

#endif
