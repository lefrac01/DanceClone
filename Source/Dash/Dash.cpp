//      Dash.cpp
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

#include "Dash.h"

const char* Dash::wdayNames[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
const char* Dash::monthNames[]={"January","February","March","April","May","June","July","August","September","October","November","December"};
const char* Dash::dayNames[]={"1st","2nd","3rd","4th","5th","6th","7th","8th","9th","10th","11th","12th","13th","14th","15th","16th","17th","18th","19th","20th","21st","22nd","23rd","24th","25th","26th","27th","28th","29th","30th","31st"};

Dash::Dash(OS& os, GUI& g) :
  sys(os),
  gui(g),
  gfx(os),
  userWantsOut(false),
  screenWidth(sys.vid.ScreenWidth()),
  screenHeight(sys.vid.ScreenHeight())
{
}

bool Dash::Init()
{
  LOG(DEBUG_BASIC, "Dash::Init()" << endl)
  if (!gfx.Init())
  {
    LOG(DEBUG_DETAIL, "Dash::Init() returning false cause gfx.Init() failed" << endl)
    return false;
  }
  
  prgb1 = (Uint8*)malloc(screenWidth * screenHeight * 3 * sizeof(Uint8));
  prgb2 = (Uint8*)malloc(screenWidth * screenHeight * 3 * sizeof(Uint8));
  
  LOG(DEBUG_DETAIL, "Dash::Init() setting onOff to default value 0" << endl)
  onOff = 0;
  onOffPercent = 0.0;

  runTime = 0;
  gameTime = 0;
  frameTime = 0;
  framerate = 0;
  timespeed = 0;
  
  return true;
}

void Dash::Run()
{
  int oldRunTime = runTime;
  runTime=SDL_GetTicks();
  frameTime=runTime-oldRunTime;
  //frameRate=(int)(1000/(double)frameTime);
  if(double_same(onOffPercent,0))gameTime=gameTime+frameTime;
  //if(onOffPercent==0)timeSpeed=(double)framecap/(double)frameRate;
  //if(onOffPercent!=0)timeSpeed=60/(double)frameRate;

  RunOnOff();
  if(!double_same(onOffPercent,0)){
    time_t now;
    now = time(NULL);
    dashTime = localtime(&now);
    RunMenu(); 
    RunTopBottomBars();
  }
  
  //WiiDash_cursor();   // handled now but GUI::Update()
  
  /*#ifdef WIN
  sys.vid.ApplySurface(0,0,WDborderimage,screen,NULL); 
  #endif*/
  
  /*SDL_Rect temprect = {22,30,241,17};
  SDL_FillRect(screen,&temprect,SDL_MapRGB(screen->format,150,150,150));
  char temptext[100]; 
  sprintf(temptext,"%s%d","Frames Per Second = ",frameRate);
  gui.SpriteText(20,30,(char*)temptext,1);*/
}

int Dash::OnOffSlide(int onPos, int offPos)
{
  return (int)(onPos*(onOffPercent/100) + offPos*((100-onOffPercent)/100));
}

void Dash::RunOnOff()
{
  if (sys.input.ButtonDown(-1, InputChannel::Button6))
  {
  if(onOff==0){
    LOG(DEBUG_GUTS, "Dash::RunOnOff() setting onOff to 1 because it is 0" << endl)
    onOff=1;
    if(double_same(onOffPercent,0)){
      Uint32 color;
      Uint16* pPosition=(Uint16*)sys.vid.screen->pixels;
      LOG(DEBUG_GUTS, "extracting separate rgb pixels into stash 1" << endl)

      /*
      for(int y=0;y<screenHeight;y++)
      {
        for(int x=0;x<screenWidth;x++)
        {
          color=*pPosition;
          
          
          prgb1[x][y][0]=(((color&63488)>>11)<<3);
          prgb1[x][y][1]=(((color&2016)>>5)<<2);
          prgb1[x][y][2]=(((color&31)>>0)<<3);
          pPosition++;
        }
      }
      */
     
      for(int x=0;x<screenWidth;x++)
      {
        int inner_offset_base = x*screenHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + screenHeight*3;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
        {
          color=*pPosition;
          prgb1[inner_offset  ]=(((color&63488)>>11)<<3);
          prgb1[inner_offset+1]=(((color&2016)>>5)<<2);
          prgb1[inner_offset+2]=(((color&31)>>0)<<3);
          pPosition++;
        }      
      }
//      #endif
      LOG(DEBUG_GUTS, "copying stash 2 into stash 1" << endl)
      memcpy(prgb2,prgb1,sys.vid.ScreenWidth()*sys.vid.ScreenHeight()*3*sizeof(Uint8));
      LOG(DEBUG_GUTS, "initial blur on stash 2" << endl)

      //TODO: something is bugged about the blur code.  recheck these loops.
      // the result has a horizontal smear ...
      /*
      for(int a=0;a<1;a++){
        for(int x=3;x<screenWidth-3;x++)for(int y=3;y<screenHeight-3;y++){
          prgb2[x][y][0]=(prgb1[x][y-3][0]+prgb1[x][y+3][0]+prgb1[x-3][y][0]+prgb1[x+3][y][0])/4;
          prgb2[x][y][1]=(prgb1[x][y-3][1]+prgb1[x][y+3][1]+prgb1[x-3][y][1]+prgb1[x+3][y][1])/4;
          prgb2[x][y][2]=(prgb1[x][y-3][2]+prgb1[x][y+3][2]+prgb1[x-3][y][2]+prgb1[x+3][y][2])/4;
        }for(int x=3;x<screenWidth-3;x++)for(int y=3;y<screenHeight-3;y++){
          prgb1[x][y][0]=(prgb2[x][y-3][0]+prgb2[x][y+3][0]+prgb2[x-3][y][0]+prgb2[x+3][y][0]+1)/4;
          prgb1[x][y][1]=(prgb2[x][y-3][1]+prgb2[x][y+3][1]+prgb2[x-3][y][1]+prgb2[x+3][y][1]+1)/4;
          prgb1[x][y][2]=(prgb2[x][y-3][2]+prgb2[x][y+3][2]+prgb2[x-3][y][2]+prgb2[x+3][y][2]+1)/4;
        }
      }
      */
      for(int x=3;x<screenWidth-3;x++)
      {
        int inner_base = x*screenHeight*3;
        int inner_left_base = (x-3)*screenHeight*3;
        int inner_right_base = (x+3)*screenHeight*3;
        
        int lower_bound = inner_base + 3*3;
        int upper_bound = inner_base + (screenHeight-3)*3;
        int inner_up = inner_base;
        int inner_down = inner_base + 6*3;
        int inner_left = inner_left_base + 3*3;
        int inner_right = inner_right_base + 3*3;
        for (int inner = lower_bound; inner < upper_bound; inner += 3, inner_left += 3, inner_right += 3)
        {
          prgb2[inner  ] = (  prgb1[inner_up  ] + prgb1[inner_down  ] + prgb1[inner_left  ] + prgb1[inner_right  ]  )/4;
          prgb2[inner+1] = (  prgb1[inner_up+1] + prgb1[inner_down+1] + prgb1[inner_left+1] + prgb1[inner_right+1]  )/4;
          prgb2[inner+2] = (  prgb1[inner_up+2] + prgb1[inner_down+2] + prgb1[inner_left+2] + prgb1[inner_right+2]  )/4;
        }
      }
      for(int x=3;x<screenWidth-3;x++)
      {
        int inner_base = x*screenHeight*3;
        int inner_left_base = (x-3)*screenHeight*3;
        int inner_right_base = (x+3)*screenHeight*3;
        
        int lower_bound = inner_base + 3*3;
        int upper_bound = inner_base + (screenHeight-3)*3;
        int inner_up = inner_base;
        int inner_down = inner_base + 6*3;
        int inner_left = inner_left_base + 3*3;
        int inner_right = inner_right_base + 3*3;
        for (int inner = lower_bound; inner < upper_bound; inner += 3, inner_left += 3, inner_right += 3, inner_up += 3, inner_down += 3)
        {
          prgb1[inner  ] = (  prgb2[inner_up  ] + prgb2[inner_down  ] + prgb2[inner_left  ] + prgb2[inner_right  ] + 1 )/4;
          prgb1[inner+1] = (  prgb2[inner_up+1] + prgb2[inner_down+1] + prgb2[inner_left+1] + prgb2[inner_right+1] + 1 )/4;
          prgb1[inner+2] = (  prgb2[inner_up+2] + prgb2[inner_down+2] + prgb2[inner_left+2] + prgb2[inner_right+2] + 1 )/4;
        }
      }
      
      
      LOG(DEBUG_GUTS, "second blur back to stash 1" << endl)
      /*
      for(int a=0;a<2;a++){
        for(int x=1;x<screenWidth-1;x++)for(int y=1;y<screenHeight-1;y++){
          prgb2[x][y][0]=(prgb1[x][y-1][0]+prgb1[x][y+1][0]+prgb1[x-1][y][0]+prgb1[x+1][y][0])/4;
          prgb2[x][y][1]=(prgb1[x][y-1][1]+prgb1[x][y+1][1]+prgb1[x-1][y][1]+prgb1[x+1][y][1])/4;
          prgb2[x][y][2]=(prgb1[x][y-1][2]+prgb1[x][y+1][2]+prgb1[x-1][y][2]+prgb1[x+1][y][2])/4;
        }for(int x=1;x<screenWidth-1;x++)for(int y=1;y<screenHeight-1;y++){
          prgb1[x][y][0]=(prgb2[x][y-1][0]+prgb2[x][y+1][0]+prgb2[x-1][y][0]+prgb2[x+1][y][0])/4;
          prgb1[x][y][1]=(prgb2[x][y-1][1]+prgb2[x][y+1][1]+prgb2[x-1][y][1]+prgb2[x+1][y][1])/4;
          prgb1[x][y][2]=(prgb2[x][y-1][2]+prgb2[x][y+1][2]+prgb2[x-1][y][2]+prgb2[x+1][y][2])/4;
        }
      }
      */
      for(int a=0;a<2;a++){
        for(int x=1;x<screenWidth-1;x++)
        {
          int inner_base = x*screenHeight*3;
          int inner_left_base = (x-1)*screenHeight*3;
          int inner_right_base = (x+1)*screenHeight*3;
          
          int lower_bound = inner_base + 1*3;
          int upper_bound = inner_base + (screenHeight-1)*3;
          int inner_up = inner_base;
          int inner_down = inner_base + 2*3;
          int inner_left = inner_left_base + 1*3;
          int inner_right = inner_right_base + 1*3;
          for (int inner = lower_bound; inner < upper_bound; inner ++, inner_left ++, inner_right ++, inner_up ++, inner_down ++)
          {
            prgb2[inner  ] = (  prgb1[inner_up  ] + prgb1[inner_down  ] + prgb1[inner_left  ] + prgb1[inner_right  ]  )/4;
            prgb2[inner+1] = (  prgb1[inner_up+1] + prgb1[inner_down+1] + prgb1[inner_left+1] + prgb1[inner_right+1]  )/4;
            prgb2[inner+2] = (  prgb1[inner_up+2] + prgb1[inner_down+2] + prgb1[inner_left+2] + prgb1[inner_right+2]  )/4;
          }
        }
        for(int x=1;x<screenWidth-1;x++)
        {
          int inner_base = x*screenHeight*3;
          int inner_left_base = (x-1)*screenHeight*3;
          int inner_right_base = (x+1)*screenHeight*3;
          
          int lower_bound = inner_base + 1*3;
          int upper_bound = inner_base + (screenHeight-1)*3;
          int inner_up = inner_base;
          int inner_down = inner_base + 2*3;
          int inner_left = inner_left_base + 1*3;
          int inner_right = inner_right_base + 1*3;
          for (int inner = lower_bound; inner < upper_bound; inner ++, inner_left ++, inner_right ++, inner_up ++, inner_down ++)
          {
            prgb1[inner  ] = (  prgb2[inner_up  ] + prgb2[inner_down  ] + prgb2[inner_left  ] + prgb2[inner_right  ]  )/4;
            prgb1[inner+1] = (  prgb2[inner_up+1] + prgb2[inner_down+1] + prgb2[inner_left+1] + prgb2[inner_right+1]  )/4;
            prgb1[inner+2] = (  prgb2[inner_up+2] + prgb2[inner_down+2] + prgb2[inner_left+2] + prgb2[inner_right+2]  )/4;
          }
        }
      }
      
      
      LOG(DEBUG_GUTS, "finalise blur effect on stash 1" << endl)
      /*
      for(int y=0;y<screenHeight;y++)for(int x=0;x<screenWidth;x++){
        prgb1[x][y][0]=In8bitRange(prgb1[x][y][0]+50);
        prgb1[x][y][1]=In8bitRange(prgb1[x][y][1]+50);
        prgb1[x][y][2]=In8bitRange(prgb1[x][y][2]+50);
        prgb1[x][y][0]=In8bitRange(prgb1[x][y][0]-100);
        prgb1[x][y][1]=In8bitRange(prgb1[x][y][1]-100);
        prgb1[x][y][2]=In8bitRange(prgb1[x][y][2]-100);
        prgb1[x][y][0]=In8bitRange(prgb1[x][y][0]+100);
        prgb1[x][y][1]=In8bitRange(prgb1[x][y][1]+100);
        prgb1[x][y][2]=In8bitRange(prgb1[x][y][2]+100);
      }
      */
      
      for(int x = 0; x < screenWidth; x++)
      {
        int inner_offset_base = x*screenHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + screenHeight*3;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
        {
          prgb1[inner_offset  ] = In8bitRange( prgb1[inner_offset  ] + 50);
          prgb1[inner_offset+1] = In8bitRange( prgb1[inner_offset+1] + 50);
          prgb1[inner_offset+2] = In8bitRange( prgb1[inner_offset+2] + 50);
          prgb1[inner_offset  ] = In8bitRange( prgb1[inner_offset  ] - 100);
          prgb1[inner_offset+1] = In8bitRange( prgb1[inner_offset+1] - 100);
          prgb1[inner_offset+2] = In8bitRange( prgb1[inner_offset+2] - 100);
          prgb1[inner_offset  ] = In8bitRange( prgb1[inner_offset  ] + 100);
          prgb1[inner_offset+1] = In8bitRange( prgb1[inner_offset+1] + 100);
          prgb1[inner_offset+2] = In8bitRange( prgb1[inner_offset+2] + 100);
        }
      }
      
      
      LOG(DEBUG_GUTS, "recompose separated RGB values for blur into screen backgroundBlurred" << endl)
//      #ifdef WIN
//      for(int y=0;y<screenHeight;y++)for(int x=0;x<screenWidth;x++)
//        putpixel(gfx.backgroundBlurred,x,y,
//          SDL_MapRGB(gfx.backgroundBlurred->format,
//          prgb1[x][y][0],prgb1[x][y][1],prgb1[x][y][2]));
//      #endif
//      #ifdef WII
      pPosition=(Uint16*)gfx.backgroundBlurred->pixels;
      for(int x=0;x<screenWidth;x++)
      {
        int inner_offset_base = x*screenHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + screenHeight*3;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
        {
          *pPosition = 
            (prgb1[inner_offset  ] >> 3 << 11) |
            (prgb1[inner_offset+1] >> 2 << 5 ) |
            (prgb1[inner_offset+2] >> 3 << 0 );
          pPosition++;
        }
      }
//      #endif
      
      
      LOG(DEBUG_GUTS, "copy gfx.background pixels into stash 2" << endl)
      for(int x=0;x<screenWidth;x++)
      {
        int inner_offset_base = x*screenHeight*3;
        int lower_bound = inner_offset_base;
        int upper_bound = inner_offset_base + screenHeight*3;
        int y = 0;
        for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3, y++)
        {
          SDL_GetRGB(sys.vid.GetPixel(gfx.background,x,y),gfx.background->format,
            &(prgb2[inner_offset  ]), &(prgb2[inner_offset+1]), &(prgb2[inner_offset+2]) );
        }
      }
    }
  }else{onOff=0;}
}

if(double_same(onOffPercent,0)){
}else if(double_same(onOffPercent,100)){
  sys.vid.ApplySurface(0,0,gfx.backgroundBlurred,sys.vid.screen,NULL);
}else{
//#ifdef WIN
//  Uint32 color;
//  Uint8 r,g,b;
//  SDL_PixelFormat *fmt = screen->format;
//  int part1=100-(int)onOffPercent;
//  int part2=(int)onOffPercent;
//  for(int y=0;y<screenHeight;y++)for(int x=0;x<screenWidth;x++){
//    r=In8bitRange((prgb2[x][y][0]*part1+prgb1[x][y][0]*part2)/100);
//    g=In8bitRange((prgb2[x][y][1]*part1+prgb1[x][y][1]*part2)/100);
//    b=In8bitRange((prgb2[x][y][2]*part1+prgb1[x][y][2]*part2)/100);
//    color=
//      (r>>fmt->Rloss<<fmt->Rshift)|
//      (g>>fmt->Gloss<<fmt->Gshift)|
//      (b>>fmt->Bloss<<fmt->Bshift);
//    putpixel(screen,x,y,color);
//  }
//#endif
//#ifdef WII

  LOG(DEBUG_GUTS, "perform wiidash open/close animation because onOffPercent = " << onOffPercent << endl)

  /*
  Uint8 r,g,b;
  int part1=100-(int)onOffPercent;   int part2=(int)onOffPercent;
  //int change=(int)(onOffPercent/100*255);
 // Uint16* pPosition=(Uint16*)screen->pixels;
  for(int y=0;y<screenHeight;y++)for(int x=0;x<screenWidth;x++){
    r=In8bitRange((WDrgb2[x][y][0]*part1+WDrgb1[x][y][0]*part2)/100);
    g=In8bitRange((WDrgb2[x][y][1]*part1+WDrgb1[x][y][1]*part2)/100);
    b=In8bitRange((WDrgb2[x][y][2]*part1+WDrgb1[x][y][2]*part2)/100);
    r=In8bitRange(uint8percentage[(signed int)WDrgb2[x][y][0]][part1]);
    g=In8bitRange(uint8percentage[(signed int)WDrgb2[x][y][1]][part1]);
    b=In8bitRange(uint8percentage[(signed int)WDrgb2[x][y][2]][part1]);
    r=prgb1[x][y][0];
    g=prgb1[x][y][1];
    b=prgb1[x][y][2];
    if(WDrgb1[x][y][0]<WDrgb2[x][y][0]){
      r=highnumber(WDrgb1[x][y][0],WDrgb2[x][y][0]-change);}
    else{
      r=lownumber(WDrgb1[x][y][0],WDrgb2[x][y][0]+change);}
    if(WDrgb1[x][y][1]<WDrgb2[x][y][1]){
      g=highnumber(WDrgb1[x][y][1],WDrgb2[x][y][1]-change);}
    else{
      g=lownumber(WDrgb1[x][y][1],WDrgb2[x][y][1]+change);}
    if(WDrgb1[x][y][2]<WDrgb2[x][y][2]){
      b=highnumber(WDrgb1[x][y][2],WDrgb2[x][y][2]-change);}
    else{
      b=lownumber(WDrgb1[x][y][2],WDrgb2[x][y][2]+change);}
      
      
    *pPosition=(r>>3<<11)|(g>>2<<5)|(b>>3<<0);
    pPosition++;
  }
  */




  Uint8 r,g,b;
  // int part1=100-(int)onOffPercent;   int part2=(int)onOffPercent;
  // int change=(int)(onOffPercent/100*255);
  Uint16* pPosition=(Uint16*)sys.vid.screen->pixels;
  
  for(int x=0;x<screenWidth;x++)
  {
    int inner_offset_base = x*screenHeight*3;
    int lower_bound = inner_offset_base;
    int upper_bound = inner_offset_base + screenHeight*3;
    for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
    {
      // r=In8bitRange((   prgb2[inner_offset  ] * part1 + prgb1[inner_offset  ] * part2)/100);
      // g=In8bitRange((   prgb2[inner_offset+1] * part1 + prgb1[inner_offset+1] * part2)/100);
      // b=In8bitRange((   prgb2[inner_offset+2] * part1 + prgb1[inner_offset+2] * part2)/100);
      
      // r=In8bitRange(uint8percentage[(signed int)  prgb2[inner_offset  ] ][part1]);
      // g=In8bitRange(uint8percentage[(signed int)  prgb2[inner_offset+1] ][part1]);
      // b=In8bitRange(uint8percentage[(signed int)  prgb2[inner_offset+2] ][part1]);

      r=prgb1[inner_offset  ];
      g=prgb1[inner_offset+1];
      b=prgb1[inner_offset+2];      
      
      //  if(WDrgb1[x][y][0]<WDrgb2[x][y][0]){
      //  r=highnumber(WDrgb1[x][y][0],WDrgb2[x][y][0]-change);}
      //else{
      //  r=lownumber(WDrgb1[x][y][0],WDrgb2[x][y][0]+change);}
      //if(WDrgb1[x][y][1]<WDrgb2[x][y][1]){
      //  g=highnumber(WDrgb1[x][y][1],WDrgb2[x][y][1]-change);}
      //else{
      //  g=lownumber(WDrgb1[x][y][1],WDrgb2[x][y][1]+change);}
      //if(WDrgb1[x][y][2]<WDrgb2[x][y][2]){
      //  b=highnumber(WDrgb1[x][y][2],WDrgb2[x][y][2]-change);}
      //else{
      //  b=lownumber(WDrgb1[x][y][2],WDrgb2[x][y][2]+change);}
      
      *pPosition=(r>>3<<11)|(g>>2<<5)|(b>>3<<0);
      pPosition++;
    }
  }

  
//#endif
}

if(onOff==0){

  onOffPercent=(onOffPercent*14+0*1)/15-0.5;
LOG(DEBUG_GUTS, "updated onOffPercent to :" << onOffPercent << " because onOff is 0" << endl)
  if(onOffPercent<0)onOffPercent=0;}
if(onOff==1){
  onOffPercent=(onOffPercent*14+100*1)/15+0.5;
LOG(DEBUG_GUTS, "updated onOffPercent to :" << onOffPercent << " because onOff is 1" << endl)
  if(onOffPercent>100)onOffPercent=100;}

}

Uint8 Dash::In8bitRange(int num)
{
  if(num<=0) return 0;
  else if(num>=255) return 255;
  else return num;
}

void Dash::Cleanup()
{
  if (prgb1)
  {
    free(prgb1);
    prgb1 = NULL;
  }
  if (prgb2)
  {
    free(prgb2);
    prgb2 = NULL;
  }
  
  gfx.Cleanup();
}

bool Dash::Visible()
{
  return (onOffPercent > 0.01); // hehe double comparison... mepsilon anybody?
}

void Dash::RunMenu()
{
  int y = OnOffSlide(screenHeight/2-125,screenHeight/2-125+screenHeight);
  ++y;
//TODO: reimplement properly.  original redesign failed.
/*
  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Resume game"))onOff=0;
  y+=50;

//  #ifdef WIN
//  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Exit"))done=true;
//  y+=50;
//  #endif

  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Exit to loader"))
  {
    userWantsOut = true;
  }
  y+=50;

  #ifdef WII
  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Restart Wii"))
  {
    SYS_ResetSystem(SYS_RESTART,0,0);
  }
  y+=50;

  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Shutdown Wii"))
  {
    SYS_ResetSystem(SYS_POWEROFF,0,0);
  }
  y+=50;
  #endif

//  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Save BMP screenshot"))WiiDash_savebmpscreenshot();
//  y+=50;

//  #ifdef WII
  if(gui.DoButton(screenWidth/2,y,300,15,1,1,(char*)"Save PNG screenshot"))gui.SavePngScreenshot();
  y+=50;
//  #endif




  //TODO: fix this duplication of GUI::Update() code
  // draw cursor
  //#
  //#for(int a=0;a<4;a++){
    //#if(sys.input.ir[a].valid && sys.input.wiimoteactive[a])
    //#{
      //#sys.vid.ApplySurface(sys.input.cursorx[a]-48,sys.input.cursory[a]-48,gfx.cursorImage,sys.vid.screen,&gfx.cursorFrames[3]);
    //#}
  //#}
  //#
  for (unsigned int i = 0; i < sys.input.inputChannels.size(); i++)
  {
    InputChannel& chan = sys.input.inputChannels[i];
    if (chan.active)
    {
      LOG(DEBUG_DETAIL, "GUI::Update() chan[" << i <<"] ACTIVE" << endl)
      sys.vid.ApplySurface(chan.cursorX-48, chan.cursorY-48, gfx.cursorImage, NULL, &gfx.cursorFrames[6+i*3]);
    }      
  }
  */
}

void Dash::RunTopBottomBars()
{
  char temptext[100]; 
  int trans=OnOffSlide(0,61);

  sys.vid.ApplySurface(0,-70-trans,gfx.topBottomBorderImage,sys.vid.screen,NULL);

  sprintf(temptext,"%s%s%s%s%s%s%d",
      wdayNames[dashTime->tm_wday],
      " ",
      monthNames[dashTime->tm_mon],
      " ",
      dayNames[dashTime->tm_mday-1],
      " ",
      1900+dashTime->tm_year
      );
  gui.SpriteText(screenWidth-20,-70-trans+131-43,temptext,3);

  strcpy(temptext,"");
  if(dashTime->tm_hour>12){
  sprintf(temptext,"%s%d",temptext,dashTime->tm_hour-12);}else{
  if(dashTime->tm_hour==0){
  sprintf(temptext,"%s%d",temptext,12);}else{
  sprintf(temptext,"%s%d",temptext,dashTime->tm_hour);}}
  strcat(temptext,":");
  if(dashTime->tm_min<10)strcat(temptext,"0");
  sprintf(temptext,"%s%d",temptext,dashTime->tm_min);
  strcat(temptext,":");
  if(dashTime->tm_sec<10)strcat(temptext,"0");
  sprintf(temptext,"%s%d",temptext,dashTime->tm_sec);
  if(dashTime->tm_hour>11){
  strcat(temptext," PM");}else{
  strcat(temptext," AM");}
  gui.SpriteText(screenWidth-20,-70-trans+131-23,temptext,3);

  sys.vid.ApplySurface(0,screenHeight-61+trans,gfx.topBottomBorderImage,sys.vid.screen,NULL);
  gui.SpriteText(20,screenHeight-61+trans+3,(char*)extraCredits,1);
  gui.SpriteText(20,screenHeight-61+trans+23,(char*)"thatotherdev.wordpress.com",1);

}


bool Dash::UserWantsOut()
{
  return userWantsOut;
}
