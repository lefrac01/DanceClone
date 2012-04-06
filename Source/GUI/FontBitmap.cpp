//      FontBitmap.cpp
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

#include "FontBitmap.h"

namespace Gooey
{

//TODO: for at least one font, the % character is much wider than the rest and causes spacing issues :(
//const char* FontBitmap::fontCharsAll[]={" ","!","\"","#","$","%","&","\\","(",")","*","+",",","-",".","/","0","1","2","3","4","5","6","7","8","9",":",";","<","=",">","?","@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","[","\\",";","]","^","_","`","'","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","{","|","}","~"};
const char* FontBitmap::fontCharsAll[]={" ","!","\"","#","$","&","\\","(",")","*","+",",","-",".","/","0","1","2","3","4","5","6","7","8","9",":",";","<","=",">","?","@","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","[","\\",";","]","^","_","`","'","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","{","|","}","~"};
const char* FontBitmap::fontCharsNumeric[]={"0","1","2","3","4","5","6","7","8","9"};


FontBitmap::FontBitmap(OS& os, string f, int p, Uint32 c) : 
  sys(os),
  filename(f),
  pointSize(p),
  colour(c),
  height(-1),
  ttfFont(NULL),
  ready(false),
  surface(NULL)
{
  if (!TTF_WasInit())
  {
    TTF_Init();
  }

  ttfFont = TTF_OpenFont(filename.c_str(), pointSize);
  if (ttfFont)
  {
    // calculate character extents
    height = TTF_FontHeight(ttfFont);
    int totalWidth = 0;
    for (unsigned int i = 0; i < sizeof fontCharsAll / sizeof fontCharsAll[0]; ++i)
    {
      int w = 0;
      int h = 0;
      if (TTF_SizeText(ttfFont, fontCharsAll[i], &w, &h) == 0)
      {
        totalWidth += w;
      }
      else
      {
        LOG(DEBUG_DETAIL, "font " << filename << " does not support glyph " << fontCharsAll[i] << endl)
      }
    }

/*
    int numGlyphs = 0;
    int widest = 0;
    for (unsigned int i = 0; i < sizeof fontCharsAll / sizeof fontCharsAll[0]; ++i)
    {
      int w = 0;
      int h = 0;
      if (TTF_SizeText(ttfFont, fontCharsAll[i], &w, &h) == 0)
      {
        ++numGlyphs;
        if (w > widest)
        {
          widest = w;
        }
      }
      else
      {
        LOG(DEBUG_DETAIL, "font " << filename << " does not support glyph " << fontCharsAll[i] << endl)
      }
    }
    charW = widest;
*/    
    // create bitmap and rects
//    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, charW * numGlyphs, charH, 32,
    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, totalWidth, height, 32,
                    0xff000000, 0x00ff0000, 0x0000ff00, SDL_ALPHA_OPAQUE);
    if (surface)
    {
      SDL_SetAlpha(surface, 0, SDL_ALPHA_OPAQUE);
      SDL_Color sdlColour;
      SDL_GetRGB(colour, sys.vid.screen->format, &sdlColour.r, &sdlColour.g, &sdlColour.b);


      SDL_Rect destRect;
      destRect.y = 0;
      destRect.h = height;
      int xOffset = 0;
      for (unsigned int i = 0; i < sizeof fontCharsAll / sizeof fontCharsAll[0]; ++i)
      {
        SDL_Surface* temp;
        int w = 0;
        int h = 0;
        if (TTF_SizeText(ttfFont, fontCharsAll[i], &w, &h) == 0)
        {
          temp = TTF_RenderText_Blended(ttfFont, fontCharsAll[i], sdlColour);
          if (temp)
          {
            SDL_SetAlpha(temp, 0, SDL_ALPHA_OPAQUE);
            destRect.x = xOffset;
            destRect.w = w;
            charFrames[fontCharsAll[i][0]] = destRect;
            SDL_BlitSurface(temp, NULL, surface, &destRect);
            SDL_FreeSurface(temp);
            xOffset += w;
          }
        }
      }
  
/*      
      int currentGlyph = 0;
      SDL_Rect destRect;
      destRect.y = 0;
      destRect.w = charW;
      destRect.h = charH;
      for (unsigned int i = 0; i < sizeof fontCharsAll / sizeof fontCharsAll[0]; ++i)
      {
        SDL_Surface* temp;
        int w = 0;
        int h = 0;
        if (TTF_SizeText(ttfFont, fontCharsAll[i], &w, &h) == 0)
        {
          temp = TTF_RenderText_Blended(ttfFont, fontCharsAll[i], sdlColour);
          if (temp)
          {
            SDL_SetAlpha(temp, 0, SDL_ALPHA_OPAQUE);
            destRect.x = charW * currentGlyph;
            destRect.w = charW;
            charFrames[fontCharsAll[i][0]] = destRect;
            // center smaller characters horizontally
            destRect.x += (int)((charW - w) / 2.0);
            destRect.w -= (int)((charW - w) / 2.0);
            SDL_BlitSurface(temp, NULL, surface, &destRect);
            SDL_FreeSurface(temp);
            ++currentGlyph;
          }
        }
      }
      */
      
      SDL_Surface* temp = surface;
      surface = SDL_DisplayFormatAlpha(surface);
      SDL_FreeSurface(temp);
    }
  }

  ready = surface != NULL && charFrames.size() > 0;
}
  
FontBitmap::~FontBitmap()
{
  if (ttfFont)
  {
    TTF_CloseFont(ttfFont);
  }
  if (surface)
  {
    SDL_FreeSurface(surface);
  }
}

bool FontBitmap::CharExists(char c)
{
  // character exists in font
  return charFrames.find(c) != charFrames.end();
}

SDL_Rect* FontBitmap::FrameFromChar(char c)
{
  SDL_Rect* result = NULL;
  if (charFrames.find(c) != charFrames.end())
  {
    result = &charFrames.find(c)->second;
  }
  return result;
}

}
