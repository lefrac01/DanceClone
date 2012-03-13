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

  useCursors = false;

  if (!gfx.Init())
  {
    return false;
  }

  return true;
}

void GUI::Update()
{
  LOG(DEBUG_GUTS, "GUI::Update()" << endl)

  //TODO: 1, Wii specific code
  //TODO: 2, input specific code


  
  //TODO: support cursor and "select" action for use with dance mat
  //TODO: tighten.  Users at platform level have inputs that may or may not be
  // mapped to Players at the game level
  int numPlayers = 1;
  for (int i = 0; i < numPlayers; i++)
  {
    if(sys.input.WiiButtonsDown[i] & WPAD_BUTTON_A)
    {
      screen.Clicked(sys.input.cursorx[i], sys.input.cursory[i]);
    }
  }
  
  Render(screen); // render screen first
  
  // draw cursor
  for(int a = 0; a < 4; a++)
  {
    useCursor[a] = 0;
  }
  
  if(useCursors){
//    #ifdef WII
    for(int a=0;a<4;a++){
      if(sys.input.ir[a].valid){
//        LOG(DEBUG_GUTS, "GUI::Update() ir[a] is valid" << endl)
        useCursor[a]=1;
      }else if(sys.input.expans[a].type==WPAD_EXP_CLASSIC){
        useCursor[a]=1;
        double mag = sys.input.expans[a].classic.rjs.mag;
        double ang = sys.input.expans[a].classic.rjs.ang;
        if(mag>0){
          ang=(ang-90)*0.0174532925;
          sys.input.cursorx[a]+=mag*cos(ang)*15;
          sys.input.cursory[a]+=mag*sin(ang)*15;
        }
      }
    }
    if(sys.input.wiimoteactive[0] && !sys.input.wiimoteactive[1] && !sys.input.wiimoteactive[2] && !sys.input.wiimoteactive[3]){
//      LOG(DEBUG_GUTS, "CURSOR: cursorx[0]-48:" << sys.input.cursorx[0]-48 << " cursory[0]-48:" << sys.input.cursory[0]-48 << endl)
      if(useCursor[0])sys.vid.ApplySurface(sys.input.cursorx[0]-48,sys.input.cursory[0]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[3]);
    }else{
      if(useCursor[0])sys.vid.ApplySurface(sys.input.cursorx[0]-48,sys.input.cursory[0]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[6]);
      if(useCursor[1])sys.vid.ApplySurface(sys.input.cursorx[1]-48,sys.input.cursory[1]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[9]);
      if(useCursor[2])sys.vid.ApplySurface(sys.input.cursorx[2]-48,sys.input.cursory[2]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[12]);
      if(useCursor[3])sys.vid.ApplySurface(sys.input.cursorx[3]-48,sys.input.cursory[3]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[15]);
    }
//    #endif
//   #ifdef WIN
//    if(SDL_GetAppState() & SDL_APPMOUSEFOCUS)useCursor[0]=1;
//    if(useCursor[0])sys.vid.ApplySurface(sys.input.cursorx[0]-48,cursory[0]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[3]);
//    #endif    
  }
  
  useCursors = true;
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

void GUI::Render(Container &c, int basex, int basey)
{
  LOG(DEBUG_GUTS, "GUI::Render() " << endl)
  

  for (unsigned int i = 0; i < c.Images().size(); i++)
  {
    Image& im = c.Images()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is image " << endl)
    sys.vid.ApplySurface(basex + im.x, basey + im.y, im.surface, NULL, NULL);
  }
  for (unsigned int i = 0; i < c.Labels().size(); i++)
  {
    Label& l = c.Labels()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is label " << endl)
    DrawSpriteText(basex + l.x, basey + l.y,(char*) l.text.c_str(), 1);
  }
  for (unsigned int i = 0; i < c.Buttons().size(); i++)
  {
    Button& b = c.Buttons()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is button " << endl)
    if (DoButton(basex + b.x, basey + b.y, b.w, b.h, false, true, (char*)b.text.c_str()))  //NOTE: first true is center
    {
      //b.clicked = true;   //TODO: clean up original code after clicking handled by new GUI
    }
  }
  for (unsigned int i = 0; i < c.Containers().size(); i++)
  {
    Container& innerc = c.Containers()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is container " << endl)
    Render(innerc, basex + innerc.x, basey + innerc.y);
  }
  for (unsigned int i = 0; i < c.SimpleSongScrollers().size(); i++)
  {
    SimpleSongScroller& sss = c.SimpleSongScrollers()[i];
    LOG(DEBUG_GUTS, "GUI::Render() element is simpleSongScroller " << endl)
    for (unsigned int j = 0; j < sss.Buttons().size(); j++)
    {
      Button& b = sss.Buttons()[j];
      LOG(DEBUG_GUTS, "GUI::Render() sss element is button " << endl)
      if (DoButton(basex + b.x, basey + b.y, b.w, b.h, true, true, (char*)b.text.c_str()))  //NOTE: first true is center
      {
        //b.clicked = true;   //TODO: clean up original code after clicking handled by new GUI
      }
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

void GUI::DrawButton(int x, int y,int w,int h,bool glow)
{
  LOG(DEBUG_GUTS, "GUI::DrawButton(" << x << ", " << y << ", " << w << ", " << h << ", " << glow << ")" << endl)
  //corners
  sys.vid.ApplySurface(x-21, y-21, gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[0+glow*9]);
  sys.vid.ApplySurface(x-21, y+h,  gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[1+glow*9]);
  sys.vid.ApplySurface(x+w,  y-21, gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[2+glow*9]);
  sys.vid.ApplySurface(x+w,  y+h,  gfx.buttonImage, sys.vid.screen, &gfx.buttonFrames[3+glow*9]);
  //edges
  for(int a=0; a<w/21; a++)
    sys.vid.ApplySurface(x+a*21,y+h,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[4+glow*9]);
  SDL_Rect temprect1={4*21,glow*21,w-w/21*21,21};
  sys.vid.ApplySurface(x+w/21*21,y+h,gfx.buttonImage,sys.vid.screen,&temprect1);
  for(int a=0; a<h/21; a++)
    sys.vid.ApplySurface(x+w,y+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[5+glow*9]);
  SDL_Rect temprect2={5*21,glow*21,21,h-h/21*21};
  sys.vid.ApplySurface(x+w,y+h/21*21,gfx.buttonImage,sys.vid.screen,&temprect2);
  for(int a=0; a<w/21; a++)
    sys.vid.ApplySurface(x+a*21,y-21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[6+glow*9]);
  SDL_Rect temprect3={6*21,glow*21,w-w/21*21,21};
  sys.vid.ApplySurface(x+w/21*21,y-21,gfx.buttonImage,sys.vid.screen,&temprect3);
  for(int a=0; a<h/21; a++)
    sys.vid.ApplySurface(x-21,y+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[7+glow*9]);
  SDL_Rect temprect4={7*21,glow*21,21,h-h/21*21};
  sys.vid.ApplySurface(x-21,y+h/21*21,gfx.buttonImage,sys.vid.screen,&temprect4);
  //insides
  for(int a=0; a<h/21; a++)for(int b=0; b<w/21; b++)
    sys.vid.ApplySurface(x+b*21,y+a*21,gfx.buttonImage,sys.vid.screen,&gfx.buttonFrames[8+glow*9]);
  SDL_Rect temprect5={8*21,glow*21,w-w/21*21,21};
  for(int a=0; a<h/21; a++)
    sys.vid.ApplySurface(x+w/21*21,y+a*21,gfx.buttonImage,sys.vid.screen,&temprect5);
  SDL_Rect temprect6={8*21,glow*21,21,h-h/21*21};
  for(int a=0; a<w/21; a++)
    sys.vid.ApplySurface(x+a*21,y+h/21*21,gfx.buttonImage,sys.vid.screen,&temprect6);
  SDL_Rect temprect7={8*21,glow*21,w-w/21*21,h-h/21*21};
  sys.vid.ApplySurface(x+w/21*21,y+h/21*21,gfx.buttonImage,sys.vid.screen,&temprect7);
}

bool GUI::DoButton(int x, int y,int w,int h,bool center,bool clickable,char* text)
{
  LOG(DEBUG_GUTS, "GUI::DoButton(" << x << ", " << y << ", " << w << ", " << h << ", " << center << ", " << clickable << ", " << text << ")" << endl)
  bool clicked=0;
  bool glow=0;
  if(center){x=x-w/2;y=y-h/2;}

//TODO: subclassing ?? probably not
//  #ifdef WIN
//  if(clickable && sys.input.cursorx[0]>x-14 && cursory[0]>y-14 && sys.input.cursorx[0]<x+w+13 && cursory[0]<y+h+13){
//    if(mousestate[SDL_BUTTON_LEFT]==2)clicked=1;
//    glow=1;}
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
      glow=1;
    }
  }
//  #endif

  DrawButton(x,y,w,h,glow);

  x=x+w/2;y=y+h/2;
  DrawSpriteText(x,y-21/2,text,2);
  return clicked;
}

void GUI::SetSpriteTextColored(Uint32 color)
{
  Uint8 r1,g1,b1;
  SDL_GetRGB(color,gfx.fontImage->format,&r1,&g1,&b1);
  Uint8 r2,g2,b2,a2;
  for(int x=0;x<gfx.fontImage->w;x++)for(int y=0;y<gfx.fontImage->h;y++){
    color=sys.vid.GetPixel(gfx.fontImage,x,y);
    SDL_GetRGBA(color,gfx.fontImage->format,&r2,&g2,&b2,&a2);
    color=SDL_MapRGBA(gfx.fontImageColored->format,r1,g1,b1,a2);
    sys.vid.PutPixel(gfx.fontImageColored,x,y,color);
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
  IMG_SavePNG((char*)filename,WDbackground,Z_BEST_COMPRESSION);
  #endif
}

}
