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
gfx(os)
{
}

bool GUI::Init()
{
  LOG(DEBUG_BASIC, "GUI::Init()" << endl)

  hideCursor = false;

  if (!gfx.Init())
  {
    LOG(DEBUG_BASIC, "ERROR: GUI::Init()   gfx.Init() failed" << endl)
    return false;
  }

  spriteTextColour = SDL_MapRGBA(gfx.fontImageColored->format, 255, 255, 255, 255);
  
  return true;
}

void GUI::Update()
{
  LOG(DEBUG_GUTS, "GUI::Update()" << endl)

  //TODO: support cursor and "select" action for use with dance mat
  int numUsers = 1;
  for (int i = 0; i < numUsers; i++)
  {
    InputChannel& chan = sys.input.inputChannels[0];
    screen.CursorAt(chan.cursorX, chan.cursorY);
    if (chan.buttonDown[InputChannel::Button4])
    {
      screen.Clicked(chan.cursorX, chan.cursorY);
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
        char buf[100];
        sprintf(buf, "%lX", (unsigned long)gfx.cursorImage);
        LOG(DEBUG_GUTS, "cursor("<< buf <<") to x: " << chan.cursorX-48 << " y: " << chan.cursorY-48 << " using src rect x:" << gfx.cursorFrames[6+i*3].x <<"  y: " <<gfx.cursorFrames[6+i*3].y<<" w: " << gfx.cursorFrames[6+i*3].w << " h: " <<gfx.cursorFrames[6+i*3].h << " whew!E" << endl)
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
    sys.vid.ApplySurface(im.x, im.y, im.surface, NULL, NULL);
  }
  for (unsigned int i = 0; i < c.Labels().size(); i++)
  {
    Label& l = c.Labels()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is label " << endl)
    if (l.colour != Element::noColour)
    {
      Uint32 oldColour = GetSpriteTextColour();
      SetSpriteTextColour(l.colour);
      SpriteTextColored(l.x, l.y, (char*)l.text.c_str(), 1);
      SetSpriteTextColour(oldColour);
    }
    else
    {
      SpriteTextColored(l.x, l.y, (char*)l.text.c_str(), 1);
    }
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
  for (unsigned int i = 0; i < c.SimpleSongScrollers().size(); i++)
  {
    SimpleSongScroller& sss = c.SimpleSongScrollers()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is simpleSongScroller " << endl)
    for (unsigned int j = 0; j < sss.Buttons().size(); j++)
    {
      Button& b = sss.Buttons()[j];
      LOG(DEBUG_GUTS, "GUI::Render() sss element is button " << endl)
      DrawButton(b);
    }
  }
}

void GUI::Cleanup()
{
  gfx.Cleanup();
}

void GUI::DrawSpriteText(int posx,int posy,char* texttosprite,int leftmiddleright)
{
  int textlength=strlen(texttosprite);
  int textnumber;
  int letterprintx=0;
  int letterprinty=posy;
  if(leftmiddleright==1)letterprintx=posx;
  if(leftmiddleright==2)letterprintx=posx-textlength*11/2;
  if(leftmiddleright==3)letterprintx=posx-textlength*11;
  for(int b=0;b<textlength;b++){
    textnumber=getnumberfromchar(texttosprite[b]);
    sys.vid.ApplySurface(letterprintx, letterprinty, gfx.fontImage, sys.vid.screen, &gfx.fontFrames[textnumber]);
    letterprintx+=11;
  }
}

void GUI::DrawButton(Button& b)
{
  LOG(DEBUG_GUTS, "GUI::DrawButton(" << b.x << ", " << b.y << ", " << b.w << ", " << b.h << endl)
  
  // is it buttonRowing?
//  if(useCursor[a] && sys.input.cursorx[a]>x-14 && sys.input.cursory[a]>y-14 && sys.input.cursorx[a]<x+w+13 && sys.input.cursory[a]<y+h+13){
  //bool buttonRow = b.state == Button::HOVER;
  //#bool buttonRow = false;
  //#for (unsigned int i = 0; i < sys.input.inputChannels.size(); i++)
  //#{
    //#InputChannel& chan = sys.input.inputChannels[i];
    //#if (chan.active && chan.cursorX>b.x-14 && chan.cursorY>b.y-14 && chan.cursorX<b.x+b.w+13 && chan.cursorY<b.y+b.h+13)
    //#{
      //#buttonRow = true;
      //#break;
    //#}      
  //#}


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
  sys.vid.ApplySurface(b.x-21, b.y-21, gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[0+buttonRow*9]);
  sys.vid.ApplySurface(b.x-21, b.y+b.h,  gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[1+buttonRow*9]);
  sys.vid.ApplySurface(b.x+b.w,  b.y-21, gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[2+buttonRow*9]);
  sys.vid.ApplySurface(b.x+b.w,  b.y+b.h,  gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[3+buttonRow*9]);
  //edges
  for(int a=0; a<b.w/21; a++)
    sys.vid.ApplySurface(b.x+a*21,b.y+b.h,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[4+buttonRow*9]);
  SDL_Rect temprect1={4*21,buttonRow*21,b.w-b.w/21*21,21};
  sys.vid.ApplySurface(b.x+b.w/21*21,b.y+b.h,gfx.buttonImage,sys.vid.screen,&temprect1);
  for(int a=0; a<b.h/21; a++)
    sys.vid.ApplySurface(b.x+b.w,b.y+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[5+buttonRow*9]);
  SDL_Rect temprect2={5*21,buttonRow*21,21,b.h-b.h/21*21};
  sys.vid.ApplySurface(b.x+b.w,b.y+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect2);
  for(int a=0; a<b.w/21; a++)
    sys.vid.ApplySurface(b.x+a*21,b.y-21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[6+buttonRow*9]);
  SDL_Rect temprect3={6*21,buttonRow*21,b.w-b.w/21*21,21};
  sys.vid.ApplySurface(b.x+b.w/21*21,b.y-21,gfx.buttonImage,sys.vid.screen,&temprect3);
  for(int a=0; a<b.h/21; a++)
    sys.vid.ApplySurface(b.x-21,b.y+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[7+buttonRow*9]);
  SDL_Rect temprect4={7*21,buttonRow*21,21,b.h-b.h/21*21};
  sys.vid.ApplySurface(b.x-21,b.y+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect4);
  //insides
  for(int a=0; a<b.h/21; a++)for(int i=0; i<b.w/21; i++)
    sys.vid.ApplySurface(b.x+i*21,b.y+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[8+buttonRow*9]);
  SDL_Rect temprect5={8*21,buttonRow*21,b.w-b.w/21*21,21};
  for(int a=0; a<b.h/21; a++)
    sys.vid.ApplySurface(b.x+b.w/21*21,b.y+a*21,gfx.buttonImage,sys.vid.screen,&temprect5);
  SDL_Rect temprect6={8*21,buttonRow*21,21,b.h-b.h/21*21};
  for(int a=0; a<b.w/21; a++)
    sys.vid.ApplySurface(b.x+a*21,b.y+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect6);
  SDL_Rect temprect7={8*21,buttonRow*21,b.w-b.w/21*21,b.h-b.h/21*21};
  sys.vid.ApplySurface(b.x+b.w/21*21,b.y+b.h/21*21,gfx.buttonImage,sys.vid.screen,&temprect7);
  
  // draw text!
  int tempx = b.x+b.w/2; 
  int tempy = b.y+b.h/2;
  //x=x+w/2;y=y+h/2;
  //DrawSpriteText(x,y-21/2,text,2);
  //SpriteText(tempx,tempy-21/2,(char*)b.text.c_str(),2);
  //SpriteTextColored(tempx,tempy-21/2,(char*)b.text.c_str(),2);
  
  Uint32 oldColour = GetSpriteTextColour();
  if (!b.active)
  {
    Uint32 inactive;
    inactive = SDL_MapRGB(sys.vid.screen->format, 80, 80, 80);
    SetSpriteTextColour(inactive);
  }
  else if (b.colour != Element::noColour)
  {
    SetSpriteTextColour(b.colour);
  }
  SpriteTextColored(tempx,tempy-21/2,(char*)b.text.c_str(),2);
  if (!b.active || b.colour != Element::noColour)
  {
    SetSpriteTextColour(oldColour);
  }
}

/*
bool GUI::DoButton(int x, int y,int w,int h,bool center,bool clickable,char* text)
{
  LOG(DEBUG_GUTS, "GUI::DoButton(" << x << ", " << y << ", " << w << ", " << h << ", " << center << ", " << clickable << ", " << text << ")" << endl)
  bool clicked=0;
  bool buttonRow=0;
  if(center){x=x-w/2;y=y-h/2;}

//TODO: subclassing ?? probably not
//  #ifdef WIN
//  if(clickable && sys.input.cursorx[0]>x-14 && cursory[0]>y-14 && sys.input.cursorx[0]<x+w+13 && cursory[0]<y+h+13){
//    if(mousestate[SDL_BUTTON_LEFT]==2)clicked=1;
//    buttonRow=1;}
//  #endif

//  #ifdef WII
  if(clickable)for(int a=0;a<4;a++){
    if(useCursor[a] && sys.input.cursorx[a]>x-14 && sys.input.cursory[a]>y-14 && sys.input.cursorx[a]<x+w+13 && sys.input.cursory[a]<y+h+13){
      if(sys.input.WiiButtonsDown[a] & WPAD_BUTTON_A)
      {
        clicked=1;    //TODO multi-user input
        LOG(DEBUG_DETAIL, "GUI::DoButton()... WPAD_BUTTON_A " << endl)
      }
      if(sys.input.WiiButtonsDown[a] & WPAD_CLASSIC_BUTTON_A)
      {
        clicked=1;  //TODO multi-user
        LOG(DEBUG_DETAIL, "GUI::DoButton()... WPAD_CLASSIC_BUTTON_A " << endl)
      }
      buttonRow=1;
    }
  }
//  #endif

  DrawButton(x,y,w,h,buttonRow);

  x=x+w/2;y=y+h/2;
  DrawSpriteText(x,y-21/2,text,2);
  return clicked;
}
*/

Uint32 GUI::GetSpriteTextColour()
{
  return spriteTextColour;
}
  
void GUI::SetSpriteTextColour(Uint32 colour)
{
  spriteTextColour = colour;
  Uint8 r1,g1,b1;
  SDL_GetRGB(colour,gfx.fontImage->format,&r1,&g1,&b1);
  Uint8 r2,g2,b2,a2;
  for(int x=0;x<gfx.fontImage->w;x++)for(int y=0;y<gfx.fontImage->h;y++){
    colour=sys.vid.GetPixel(gfx.fontImage,x,y);
    SDL_GetRGBA(colour,gfx.fontImage->format,&r2,&g2,&b2,&a2);
    colour=SDL_MapRGBA(gfx.fontImageColored->format,r1,g1,b1,a2);
    sys.vid.PutPixel(gfx.fontImageColored,x,y,colour);
  }
}

void GUI::SpriteTextColored(int posx,int posy,char* texttosprite,int leftmiddleright)
{
  int textlength=strlen(texttosprite);
  int textnumber;
  int letterprintx=0;
  int letterprinty=posy;
  if(leftmiddleright==1)letterprintx=posx;
  if(leftmiddleright==2)letterprintx=posx-textlength*11/2;
  if(leftmiddleright==3)letterprintx=posx-textlength*11;
  for(int b=0;b<textlength;b++){
    textnumber=getnumberfromchar(texttosprite[b]);
    sys.vid.ApplySurface(letterprintx,letterprinty,gfx.fontImageColored,sys.vid.screen,&gfx.fontFrames[textnumber]);
    letterprintx+=11;
  }
}

void GUI::SpriteText(int posx,int posy,char* texttosprite,int leftmiddleright)
{
  int textlength=strlen(texttosprite);
  int textnumber;
  int letterprintx=0;
  int letterprinty=posy;
  if(leftmiddleright==1)letterprintx=posx;
  if(leftmiddleright==2)letterprintx=posx-textlength*11/2;
  if(leftmiddleright==3)letterprintx=posx-textlength*11;
  for(int b=0;b<textlength;b++){
    textnumber=getnumberfromchar(texttosprite[b]);
    sys.vid.ApplySurface(letterprintx,letterprinty,gfx.fontImage,sys.vid.screen,&gfx.fontFrames[textnumber]);
    letterprintx+=11;
  }
}

int GUI::SpriteTextWrapped(int posx,int posy,char* texttosprite,int length)
{
  int textlength=strlen(texttosprite);
  int textnumber;
  int letterprintx=posx;
  int letterprinty=posy-20;
  int linecount=0;
  int lineend=0;
  for(int b=0;b<textlength;b++){
    if(b==lineend){
      lineend=b+length;
      if(b+length<textlength)
      for(int a=b;a<b+length;a++)if(texttosprite[a]==' ')lineend=a+1;
      linecount++;
      letterprintx=posx;
      letterprinty+=20;
    }
    textnumber=getnumberfromchar(texttosprite[b]);
    sys.vid.ApplySurface(letterprintx,letterprinty,gfx.fontImage,sys.vid.screen,&gfx.fontFrames[textnumber]);
    letterprintx+=11;
  }
  return linecount;
}
/*
void WiiDash_savebmpscreenshot(){
  #ifdef WII
  ifstream test;char filename[64];int filenumber=1;
  sprintf(filename,"sd:/screenshot%d.bmp",filenumber);
  test.open(filename);test.close();
  if(!test.fail()){
    while(!test.fail()){
      filenumber=filenumber+1;
      sprintf(filename,"sd:/screenshot%d.bmp",filenumber);
      test.open(filename);test.close();
    }
  }
  SDL_SaveBMP(WDbackground,filename);
  #endif
  #ifdef WIN
  ifstream test;char filename[64];int filenumber=1;
  sprintf(filename,"screenshot%d.bmp",filenumber);
  test.open(filename);test.close();
  if(!test.fail()){
    while(!test.fail()){
      filenumber=filenumber+1;
      sprintf(filename,"screenshot%d.bmp",filenumber);
      test.open(filename);test.close();
    }
  }
  SDL_SaveBMP(WDbackground,filename);
  #endif
}
*/
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

}
