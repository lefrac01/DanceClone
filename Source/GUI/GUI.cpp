//      GUI.cpp
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

#include "GUI.h"

namespace Gooey
{

GUI::GUI(OS& os) :
  sys(os),
  gfx(os),
  defaultFontBitmap(FontBitmap(os, "fontname", 12, SDL_MapRGBA(os.vid.screen->format, 0, 0, 0, 255))),
  fonts(Element::NUM_FONTS)
{
}

bool GUI::Init()
{
  LOG(DEBUG_MINOR, "GUI::Init()" << endl)

  hideCursor = false;

  if (!gfx.Init())
  {
    LOG(DEBUG_BASIC, "ERROR: GUI::Init()   gfx.Init() failed" << endl)
    return false;
  }

  fonts[Element::DefaultFont]  = "Media/Gui/Hall Fetica.ttf";
  fonts[Element::AeroliteSky]  = "Media/Gui/Aerolite Sky.ttf";
  fonts[Element::CardewThree]  = "Media/Gui/CardewThreeRounded.ttf";
  fonts[Element::Cryta]        = "Media/Gui/Crysta.ttf";
  fonts[Element::D3Euronism]   = "Media/Gui/D3Euronism_b_i.ttf";
  fonts[Element::Future]       = "Media/Gui/Future World.ttf";
  fonts[Element::Research]     = "Media/Gui/Research Remix Normal.ttf";

  return true;
}

void GUI::Update()
{
  LOG(DEBUG_GUTS, "GUI::Update()" << endl)

  // fix: multi-user gui on Wii means multiple cursors.  CursorAt() is
  // called for each cursor and the hover state is set to false any time
  // the x,y is not over the element.  this means the hover state should be
  // valid only if for a given frame at least one cursor is over it.
  // therefore clear hover states at start of frame, then OR hover state
  // with each successive call to CursorAt()
  screen.CursorClear();
  
  if (!hideCursor)
  {
    for (unsigned int i = 0; i < sys.input.inputChannels.size(); ++i)
    {
      InputChannel& chan = sys.input.inputChannels[i];
      screen.CursorAt(chan.cursorX, chan.cursorY);
      if (chan.buttonDown[InputChannel::Button4])
      {
        screen.Clicked(chan.cursorX, chan.cursorY, i);
      }
    }
  }
  
  Render(screen); // render screen first
  
  // draw cursor
  if (!hideCursor)
  {
    for (unsigned int i = 0; i < sys.input.inputChannels.size(); i++)
    {
      InputChannel& chan = sys.input.inputChannels[i];
      if (chan.active)
      {
        sys.vid.ApplySurface(chan.cursorX-48, chan.cursorY-48, gfx.cursorImage, sys.vid.screen, &gfx.cursorFrames[6+i*3]);
      }
    }
  }
}

void GUI::SetScreen(Container& c)
{
  screen = c;
  // by the way the container must have its extents properly set or the click 
  // detection fails.  expect user to always supply screen extents?
  if (screen.x == -1 || screen.y == -1 || screen.w == -1 || screen.h == -1)
  {
    screen.x = 0;
    screen.y = 0;
    screen.w = sys.vid.ScreenWidth();
    screen.h = sys.vid.ScreenHeight();
  }
  LOG(DEBUG_MINOR, "GUI::SetScreen() screen button count now: " << screen.AllButtons().size() << endl)
}

void GUI::Render(Container &c)
{
  LOG(DEBUG_GUTS, "GUI::Render() " << endl)

  for (unsigned int i = 0; i < c.Images().size(); i++)
  {
    Image& im = c.Images()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is image " << endl)
    int drawX = im.x;
    int drawY = im.y;
    if (im.offsetMode & Element::HCenter)
    {
      drawX = im.x - (im.w == 0 ? im.surface->w/2 : im.w/2);
    }
    if (im.offsetMode & Element::VCenter)
    {
      drawY = im.y - (im.h == 0 ? im.surface->h/2 : im.h/2);
    }
    sys.vid.ApplySurface(drawX, drawY, im.surface, NULL, NULL);
  }
  for (unsigned int i = 0; i < c.Labels().size(); i++)
  {
    Label& l = c.Labels()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is label " << endl)
    DrawLabel(l);
  }
  for (unsigned int i = 0; i < c.Buttons().size(); i++)
  {
    Button& b = c.Buttons()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is button " << endl)
    DrawButton(b);
  }
  for (unsigned int i = 0; i < c.Containers().size(); i++)
  {
    Container& innerc = c.Containers()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is container " << endl)
    Render(innerc);
  }
}

void GUI::Cleanup()
{
  gfx.Cleanup();

  while (fontBitmapCache.size() > 0)
  {
    delete fontBitmapCache.begin()->second;
    fontBitmapCache.erase(fontBitmapCache.begin());
  }

  if (TTF_WasInit())
  {
    TTF_Quit();
  }
}

void GUI::DrawButton(Button& b)
{
  LOG(DEBUG_GUTS, "GUI::DrawButton(" << b.x << ", " << b.y << ", " << b.w << ", " << b.h << endl)
  
  int drawX = b.x;
  int drawY = b.y;
  if (b.offsetMode & Element::HCenter)
  {
    drawX = b.x - b.w/2;
  }
  if (b.offsetMode & Element::VCenter)
  {
    drawY = b.y - b.h/2;
  }

  // Button states are in a 3*9 array mapped like this:
  // row 0: normal
  // row 1: hover
  // row 2: not active
  int buttonRow = 0;
  if (!b.active)
  {
    buttonRow = 2;
  }
  else if (b.state == Button::HOVER)
  {
    buttonRow = 1;
  }
  
  
  //corners
  sys.vid.ApplySurface(drawX-21, drawY-21, gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[0+buttonRow*9]);
  sys.vid.ApplySurface(drawX-21, drawY+b.h,  gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[1+buttonRow*9]);
  sys.vid.ApplySurface(drawX+b.w,  drawY-21, gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[2+buttonRow*9]);
  sys.vid.ApplySurface(drawX+b.w,  drawY+b.h,  gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[3+buttonRow*9]);
  //edges
  for(int a=0; a<b.w/21; a++)
    sys.vid.ApplySurface(drawX+a*21,drawY+b.h,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[4+buttonRow*9]);
  SDL_Rect temprect1={4*21,buttonRow*21,b.w-b.w/21*21,21};
  sys.vid.ApplySurface(drawX+b.w/21*21,drawY+b.h,gfx.buttonImage,sys.vid.screen,&temprect1);
  for(int a=0; a<b.h/21; a++)
    sys.vid.ApplySurface(drawX+b.w,drawY+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[5+buttonRow*9]);
  SDL_Rect temprect2={5*21,buttonRow*21,21,b.h-b.h/21*21};
  sys.vid.ApplySurface(drawX+b.w,drawY+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect2);
  for(int a=0; a<b.w/21; a++)
    sys.vid.ApplySurface(drawX+a*21,drawY-21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[6+buttonRow*9]);
  SDL_Rect temprect3={6*21,buttonRow*21,b.w-b.w/21*21,21};
  sys.vid.ApplySurface(drawX+b.w/21*21,drawY-21,gfx.buttonImage,sys.vid.screen,&temprect3);
  for(int a=0; a<b.h/21; a++)
    sys.vid.ApplySurface(drawX-21,drawY+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[7+buttonRow*9]);
  SDL_Rect temprect4={7*21,buttonRow*21,21,b.h-b.h/21*21};
  sys.vid.ApplySurface(drawX-21,drawY+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect4);
  //insides
  for(int a=0; a<b.h/21; a++)for(int i=0; i<b.w/21; i++)
    sys.vid.ApplySurface(drawX+i*21,drawY+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[8+buttonRow*9]);
  SDL_Rect temprect5={8*21,buttonRow*21,b.w-b.w/21*21,21};
  for(int a=0; a<b.h/21; a++)
    sys.vid.ApplySurface(drawX+b.w/21*21,drawY+a*21,gfx.buttonImage,sys.vid.screen,&temprect5);
  SDL_Rect temprect6={8*21,buttonRow*21,21,b.h-b.h/21*21};
  for(int a=0; a<b.w/21; a++)
    sys.vid.ApplySurface(drawX+a*21,drawY+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect6);
  SDL_Rect temprect7={8*21,buttonRow*21,b.w-b.w/21*21,b.h-b.h/21*21};
  sys.vid.ApplySurface(drawX+b.w/21*21,drawY+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect7);

  // draw text!
  Label l(b.text, drawX + b.w/2, drawY + b.h/2);
  l.offsetMode = Element::HCenter | Element::VCenter;
  l.colour = b.colour;
  l.pointSize = b.pointSize;
  DrawLabel(l);
}

void GUI::DrawText(int x, int y, string text)
{
  DrawText(x, y, text, Element::DefaultFont, SDL_MapRGBA(sys.vid.screen->format, 0, 0, 0, 255), 16);
}

void GUI::DrawText(int x, int y, string text, Element::Font f, Uint32 colour, int pointSize)
{
  if (colour == Element::noColour)
  {
    colour = SDL_MapRGBA(sys.vid.screen->format, 0, 0, 0, 255);
  }
  int textWidth = 0;
  FontBitmap* fb = GetCachedFont(fonts[f], pointSize, colour);
  if (fb)
  {
    for (unsigned int i = 0; i < text.size(); ++i)
    {
      if (fb->CharExists(text.at(i)))
      {
        SDL_Rect destRect;
        destRect.x = x + textWidth;
        destRect.y = y;
        SDL_BlitSurface(fb->surface, fb->FrameFromChar(text.at(i)), sys.vid.screen, &destRect);
        textWidth += destRect.w;
      }
    }
  }
}

void GUI::DrawLabel(Label& l)
{
  int hOffset = 0;
  int vOffset = 0;
  int textWidth = 0;
  
  FontBitmap* fb = GetCachedFont(fonts[l.font], l.pointSize, l.colour);
  if (fb)
  {
    if (l.offsetMode & Element::HCenter)
    {
      for (unsigned int i = 0; i < l.text.size(); ++i)
      {
        if (fb->CharExists(l.text.at(i)))
        {
          textWidth += fb->FrameFromChar(l.text.at(i))->w;
        }
      }
      hOffset = textWidth / 2;
    }
    if (l.offsetMode & Element::VCenter)
    {
      vOffset = fb->height / 2;
    }
    
    DrawText(l.x - hOffset, l.y - vOffset, l.text, l.font, l.colour, l.pointSize);
  }
}

void GUI::SavePngScreenshot()
{
  //TODO: save png
  /*
  #ifdef WII
  ifstream test;char filename[64];int filenumber=1;
  sprintf(filename,"sd:/screenshot%d.png",filenumber);
  test.open(filename);test.close();
  if(!test.fail()){
    while(!test.fail()){
      filenumber=filenumber+1;
      sprintf(filename,"sd:/screenshot%d.png",filenumber);
      test.open(filename);test.close();
    }
  }
  IMG_SavePNG((char*)filename, gfx.background, Z_BEST_COMPRESSION);
  #endif
  */
}


FontBitmap* GUI::GetCachedFont(string filename, int pointSize, Uint32 colour)
{
  FontBitmap* result = NULL;
  
  FontBitmapKey key(filename, pointSize, colour);
  FontBitmapCacheMap::iterator it;
  it = fontBitmapCache.find(key);
  if (it == fontBitmapCache.end())
  {
    FontBitmap* temp = new FontBitmap(sys, filename, pointSize, colour);
    if (temp)
    {
      if (temp->ready)
      {
        fontBitmapCache[key] = temp;
        result = temp;
      }
      else
      {
        delete temp;
      }
    }
  }
  else
  {
    result = it->second;
  }
  
  return result;
}

}
