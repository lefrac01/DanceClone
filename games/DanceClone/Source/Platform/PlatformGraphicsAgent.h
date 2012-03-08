//      PlatformGraphicsAgent.h
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

#include <string>
using std::string;
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <exception>
using std::exception;
#include <gccore.h>

//TODO: tighten.  this is input stuff but the function call needs screen extents
#include <wiiuse/wpad.h>


class PlatformGraphicsAgent
{
private:

  SDL_Surface* LoadOptimize(string filename);
  SDL_Surface* LoadOptimizeAlpha(string filename);
  void ExpandArrows(string source_file, SDL_Surface* dest, SDL_Rect* dest_frames, int src_cols);
  void SetArrowFrame(SDL_Rect* dest, int index, int x, int y, int w, int h);
  
public:
  SDL_Surface* screen;
  GXRModeObj* rmode;
  Uint8* pWDrgb1;
  Uint8* pWDrgb2;

  SDL_Surface *WDbackground;
  SDL_Surface *WDbackgroundblurred;
  SDL_Surface *WDborderimage;
  SDL_Surface *WDtopbottomborderimage;
  SDL_Surface *WDcursorimage;
  SDL_Rect WDcursorframes[16];
  SDL_Surface *WDfontimage;
  SDL_Rect WDfontframes[95];
  SDL_Surface *WDbuttonimage;
  SDL_Rect WDbuttonframes[18];
  SDL_Surface *WDarrowsimage;
  SDL_Rect WDarrowsframes[4];
  SDL_Surface *WDfontimagecolored;

  PlatformGraphicsAgent();
  bool Init();
  void Cleanup();
  void ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
  void Pump();
  int ScreenHeight();
  int ScreenWidth();
  void DrawSpriteText(int posx,int posy,char* texttosprite,int leftmiddleright);

};



