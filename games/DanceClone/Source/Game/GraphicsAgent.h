//      GraphicsAgent.h
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
#ifndef GRAPHICSAGENT_H
#define GRAPHICSAGENT_H

#include <string>
using std::string;
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <exception>
using std::exception;
#include <gccore.h>

#include "../Platform/Platform.h"

namespace DanceClone
{

class GraphicsAgent
{
private:

  Platform& platform;
  SDL_Surface* LoadOptimize(string filename);
  SDL_Surface* LoadOptimizeAlpha(string filename);
  void ExpandArrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames, int src_cols);
  void SetArrowFrame(SDL_Rect* dest, int index, int x, int y, int w, int h);
  GraphicsAgent();
  int screenWidth;
  int screenHeight;
  int goalOffset;
  
public:

  SDL_Surface* screen;
  GXRModeObj* rmode;
  Uint8* pWDrgb1;
  Uint8* pWDrgb2;

  SDL_Surface* background_image;
  SDL_Surface* title_image;
  SDL_Surface* ratings_image;
  SDL_Rect ratings_frames[3];
  SDL_Surface* get_ready_image;

  // all non-hold arrows have same layout so use only one frames array
  SDL_Rect arrows_frames[16];
  SDL_Surface* temp_arrows_image_src; 
  SDL_Surface* home_arrows_image; 
  SDL_Surface* quarter_arrows_image; 
  SDL_Surface* eighth_arrows_image; 
  SDL_Surface* quarter_triplet_arrows_image; 
  SDL_Surface* sixteenth_arrows_image; 
  SDL_Surface* eighth_triplet_arrows_image; 
  SDL_Surface* thirtysecond_arrows_image; 
  SDL_Surface* sixteenth_triplet_arrows_image; 
  SDL_Surface* sixtyfourth_arrows_image; 
  SDL_Surface* sixtyfourth_triplet_arrows_image; 

  SDL_Surface* freeze_arrows_body_image;
  SDL_Rect freeze_body_frames[12];
  SDL_Surface* freeze_arrows_tail_image;
  SDL_Rect freeze_tail_frames[12];
  SDL_Surface* freeze_arrows_head_image;
  SDL_Rect freeze_head_frames[16];

  SDL_Rect arrows_hit_frames[32];
  SDL_Surface* marvellous_hit_image; 
  SDL_Surface* perfect_hit_image; 
  SDL_Surface* great_hit_image; 
  SDL_Surface* good_hit_image; 
  SDL_Surface* freeze_hit_image; 

  int arrowWidth;
  int arrowHeight;

  float pixelsPerMsAt1BPM;


  GraphicsAgent(Platform& p);
  bool Init(string configFilePath = "");
  void Cleanup();
  void ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
  void DrawBackground();
};

}

#endif
