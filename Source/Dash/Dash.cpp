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
  LOG(DEBUG_MINOR, "Dash::Init()" << endl)
  if (!gfx.Init())
  {
    LOG(DEBUG_BASIC, "Dash::Init() returning false cause gfx.Init() failed" << endl)
    return false;
  }
  
  dashBg = SDL_DisplayFormatAlpha(sys.vid.screen);
  //prgb1 = (Uint8*)malloc(screenWidth * screenHeight * 3 * sizeof(Uint8));
  //prgb2 = (Uint8*)malloc(screenWidth * screenHeight * 3 * sizeof(Uint8));
  
  LOG(DEBUG_DETAIL, "Dash::Init() setting onOff to default value 0" << endl)
  displayDash = false;
  stateChanged = false;
  onOffPercent = 0.0;

  return true;
}

void Dash::Run()
{
  RunOnOff();
  if(!double_same(onOffPercent, 0))
  {
    time_t now;
    now = time(NULL);
    dashTime = localtime(&now);
    RunMenu();
    RunTopBottomBars();
  }

  if (stateChanged && displayDash == false)
  {
    stateChanged = false;
    // put old screen back
    gui.SetScreen(oldGuiScreen);
    gui.hideCursor = oldGuiCursorHidden;
  }
}

int Dash::OnOffSlide(int onPos, int offPos)
{
  return (int)(onPos*(onOffPercent/100) + offPos*((100-onOffPercent)/100));
}

void Dash::RunOnOff()
{
  if (sys.input.ButtonDown(-1, InputChannel::Button6))
  {
    // Home button pressed.  toggle dash display
    displayDash = !displayDash;
    stateChanged = true;
    
      //Blur code.  works but slow on Wii
        
      /* 
      if(double_same(onOffPercent,0))
      {
        
        Uint32 color;
        Uint16* pPosition=(Uint16*)sys.vid.screen->pixels;
        LOG(DEBUG_GUTS, "extracting separate rgb pixels into stash 1" << endl)

        for(int y = 0; y < screenHeight; ++y)
        {
          int inner_offset_base = y * screenWidth * 3;
          int lower_bound = inner_offset_base;
          int upper_bound = inner_offset_base + screenWidth * 3;
          for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
          {
            color=*pPosition;
            prgb1[inner_offset  ]=(((color&63488)>>11)<<3);
            prgb1[inner_offset+1]=(((color&2016)>>5)<<2);
            prgb1[inner_offset+2]=(((color&31)>>0)<<3);
            ++pPosition;
          }      
        }

        LOG(DEBUG_GUTS, "copying stash 1 into stash 2" << endl)
        memcpy(prgb2, prgb1, sys.vid.ScreenWidth() * sys.vid.ScreenHeight() * 3 * sizeof(Uint8));
        
        
        LOG(DEBUG_GUTS, "initial blur on stash 2" << endl)
        for(int y = 3; y < screenHeight - 3; ++y)
        {
          int inner_base =  y    * screenWidth * 3;
          int inner_up =   (y-3) * screenWidth * 3 + 3*3;
          int inner_down = (y+3) * screenWidth * 3 + 3*3;
          
          int lower_bound = inner_base + 3*3;
          int upper_bound = inner_base + (screenWidth-3)*3;
          int inner_left =  inner_base - 3*3;
          int inner_right = inner_base + 3*3;
          for (int inner = lower_bound; inner < upper_bound; inner += 3, inner_left += 3, inner_right += 3, inner_up += 3, inner_down += 3)
          {
            prgb2[inner  ] = (  prgb1[inner_up  ] + prgb1[inner_down  ] + prgb1[inner_left  ] + prgb1[inner_right  ]  )/4;
            prgb2[inner+1] = (  prgb1[inner_up+1] + prgb1[inner_down+1] + prgb1[inner_left+1] + prgb1[inner_right+1]  )/4;
            prgb2[inner+2] = (  prgb1[inner_up+2] + prgb1[inner_down+2] + prgb1[inner_left+2] + prgb1[inner_right+2]  )/4;
          }
        }
        
        for(int y = 3; y < screenHeight - 3; ++y)
        {
          int inner_base =  y    * screenWidth * 3;
          int inner_up =   (y-3) * screenWidth * 3 + 3*3;
          int inner_down = (y+3) * screenWidth * 3 + 3*3;
          
          int lower_bound = inner_base + 3*3;
          int upper_bound = inner_base + (screenWidth-3) * 3;
          int inner_left =  inner_base - 3*3;
          int inner_right = inner_base + 3*3;
          for (int inner = lower_bound; inner < upper_bound; inner += 3, inner_left += 3, inner_right += 3, inner_up += 3, inner_down += 3)
          {
            prgb1[inner  ] = (  prgb2[inner_up  ] + prgb2[inner_down  ] + prgb2[inner_left  ] + prgb2[inner_right  ] + 1 )/4;
            prgb1[inner+1] = (  prgb2[inner_up+1] + prgb2[inner_down+1] + prgb2[inner_left+1] + prgb2[inner_right+1] + 1 )/4;
            prgb1[inner+2] = (  prgb2[inner_up+2] + prgb2[inner_down+2] + prgb2[inner_left+2] + prgb2[inner_right+2] + 1 )/4;
          }
        }

      
        LOG(DEBUG_GUTS, "second blur back to stash 1" << endl)
        for(int a = 0; a < 2; ++a)
        {
          for(int y = 1; y < screenHeight - 1; ++y)
          {
            int inner_base =  y    * screenWidth * 3;
            int inner_up =   (y-1) * screenWidth * 3 + 1*3;
            int inner_down = (y+1) * screenWidth * 3 + 1*3;
            
            int lower_bound = inner_base + 1*3;
            int upper_bound = inner_base + (screenWidth-1)*3;
            int inner_left  = inner_base - 1*3;
            int inner_right = inner_base + 1*3;
            for (int inner = lower_bound; inner < upper_bound; ++inner, ++inner_left, ++inner_right, ++inner_up, ++inner_down)
            {
              prgb2[inner  ] = (  prgb1[inner_up  ] + prgb1[inner_down  ] + prgb1[inner_left  ] + prgb1[inner_right  ]  )/4;
              prgb2[inner+1] = (  prgb1[inner_up+1] + prgb1[inner_down+1] + prgb1[inner_left+1] + prgb1[inner_right+1]  )/4;
              prgb2[inner+2] = (  prgb1[inner_up+2] + prgb1[inner_down+2] + prgb1[inner_left+2] + prgb1[inner_right+2]  )/4;
            }
          }
          for(int y = 1; y < screenHeight - 1; ++y)
          {
            int inner_base =  y    * screenWidth * 3;
            int inner_up =   (y-1) * screenWidth * 3 + 1*3;
            int inner_down = (y+1) * screenWidth * 3 + 1*3;
            
            int lower_bound = inner_base + 1*3;
            int upper_bound = inner_base + (screenWidth-1)*3;
            int inner_left  = inner_base - 1*3;
            int inner_right = inner_base + 1*3;
            for (int inner = lower_bound; inner < upper_bound; inner ++, inner_left ++, inner_right ++, inner_up ++, inner_down ++)
            {
              prgb1[inner  ] = (  prgb2[inner_up  ] + prgb2[inner_down  ] + prgb2[inner_left  ] + prgb2[inner_right  ]  )/4;
              prgb1[inner+1] = (  prgb2[inner_up+1] + prgb2[inner_down+1] + prgb2[inner_left+1] + prgb2[inner_right+1]  )/4;
              prgb1[inner+2] = (  prgb2[inner_up+2] + prgb2[inner_down+2] + prgb2[inner_left+2] + prgb2[inner_right+2]  )/4;
            }
          }
        }
        
        // Brightness pixel by pixel
        
        LOG(DEBUG_GUTS, "brightness effect on stash 1" << endl)
        for(int y = 0; y < screenHeight; ++y)
        {
          int inner_offset_base = y * screenWidth * 3;
          int lower_bound = inner_offset_base;
          int upper_bound = inner_offset_base + screenWidth * 3;
          for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
          {
            prgb1[inner_offset  ] = In8bitRange( prgb1[inner_offset  ] - 50);
            prgb1[inner_offset+1] = In8bitRange( prgb1[inner_offset+1] - 50);
            prgb1[inner_offset+2] = In8bitRange( prgb1[inner_offset+2] - 50);
            prgb1[inner_offset  ] = In8bitRange( prgb1[inner_offset  ] + 100);
            prgb1[inner_offset+1] = In8bitRange( prgb1[inner_offset+1] + 100);
            prgb1[inner_offset+2] = In8bitRange( prgb1[inner_offset+2] + 100);
            prgb1[inner_offset  ] = In8bitRange( prgb1[inner_offset  ] - 100);
            prgb1[inner_offset+1] = In8bitRange( prgb1[inner_offset+1] - 100);
            prgb1[inner_offset+2] = In8bitRange( prgb1[inner_offset+2] - 100);
          }
        }
        
        
        LOG(DEBUG_GUTS, "recompose separated RGB values for blur into screen backgroundBlurred" << endl)
        pPosition=(Uint16*)gfx.backgroundBlurred->pixels;
        for(int y = 0; y < screenHeight; ++y)
        {
          int inner_offset_base = y * screenWidth * 3;
          int lower_bound = inner_offset_base;
          int upper_bound = inner_offset_base + screenWidth * 3;
          for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3)
          {
            *pPosition = 
              (prgb1[inner_offset  ] >> 3 << 11) |
              (prgb1[inner_offset+1] >> 2 << 5 ) |
              (prgb1[inner_offset+2] >> 3 << 0 );
            pPosition++;
          }
        }
        
        LOG(DEBUG_GUTS, "copy gfx.background pixels into stash 2" << endl)
        for(int y = 0; y < screenHeight; ++y)
        {
          int inner_offset_base = y * screenHeight * 3;
          int lower_bound = inner_offset_base;
          int upper_bound = inner_offset_base + screenHeight * 3;
          int x = 0;
          for (int inner_offset = lower_bound; inner_offset < upper_bound; inner_offset += 3, ++x)
          {
            SDL_GetRGB(sys.vid.GetPixel(gfx.background, x, y), gfx.background->format,
              &(prgb2[inner_offset  ]), &(prgb2[inner_offset+1]), &(prgb2[inner_offset+2]) );
          }
        }
      }
    }
    else
    {
      onOff = 0;
      stateChanged = true;
    }
    */
  }

  if(displayDash == false)
  {
    onOffPercent = (onOffPercent * 14) / 15 - 0.5;
    LOG(DEBUG_GUTS, "updated onOffPercent to :" << onOffPercent << " because onOff is 0" << endl)
    if(onOffPercent < 0)
    {
      onOffPercent = 0;
    }
  }
  else
  {
    onOffPercent = (onOffPercent * 14 + 100) / 15 + 0.5;
    LOG(DEBUG_GUTS, "updated onOffPercent to :" << onOffPercent << " because onOff is 1" << endl)
    if(onOffPercent > 100)
    {
      onOffPercent = 100;
    }
  }
}

Uint8 Dash::In8bitRange(int num)
{
  if(num<=0) return 0;
  else if(num>=255) return 255;
  else return num;
}

void Dash::Cleanup()
{
  /*
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
  */
  SDL_FreeSurface(dashBg);
  
  gfx.Cleanup();
}

bool Dash::Visible()
{
  return (onOffPercent > 0.01);
}

void Dash::RunMenu()
{
  if (displayDash == true)
  {
    if (stateChanged)
    {
      stateChanged = false;
      
      // set up dash screen.  make dash bg from darkened copy of current screen
      SDL_BlitSurface(sys.vid.screen, NULL, dashBg, NULL);
      SDL_Surface* temp = SDL_DisplayFormat(dashBg);
      SDL_FillRect(temp, NULL, SDL_MapRGB(sys.vid.screen->format, 0, 0, 0));
      SDL_SetAlpha(temp, SDL_SRCALPHA, 128);
      SDL_BlitSurface(temp, NULL, dashBg, NULL);
      SDL_FreeSurface(temp);
      
      // create Dash menu, saving existing gui screen
      oldGuiScreen = gui.screen;
      oldGuiCursorHidden = gui.hideCursor;
      
      gui.hideCursor = false;
      Image bg(dashBg, 0, 0, 0, 0);
      Container dash;
      dash.Add(bg);
      
      int drawY = 150;

      Button resume("Resume game", screenWidth / 2, drawY, 300, 15, resumeTag);
      resume.offsetMode = Element::VCenter | Element::HCenter;
      dash.Add(resume);
      drawY += 50;
      
      Button exit("Exit to loader", screenWidth / 2, drawY, 300, 15, exitTag);
      exit.offsetMode = Element::VCenter | Element::HCenter;
      dash.Add(exit);
      drawY += 50;
      
      #ifdef WII
      Button restart("Restart Wii", screenWidth / 2, drawY, 300, 15, restartTag);
      restart.offsetMode = Element::VCenter | Element::HCenter;
      dash.Add(restart);
      drawY += 50;
      
      Button shutdown("Shutdown Wii", screenWidth / 2, drawY, 300, 15, shutdownTag);
      shutdown.offsetMode = Element::VCenter | Element::HCenter;
      dash.Add(shutdown);
      drawY += 50;
      #endif

      Button screenshot("Save PNG Screenshot", screenWidth / 2, drawY, 300, 15, screenshotTag);
      screenshot.offsetMode = Element::VCenter | Element::HCenter;
      dash.Add(screenshot);
      drawY += 50;

      gui.SetScreen(dash);
    }
  
    vector<Button> buttons;
    buttons = gui.screen.AllButtons();
    for (unsigned int i = 0; i < buttons.size(); i++)
    {
      Button& b = buttons[i];
      if (b.clicked)
      {
        switch (b.tag)
        {
        case resumeTag:
          displayDash = false;
          stateChanged = true;
          break;
        case exitTag:
          userWantsOut = true;
          break;
        #ifdef WII
        case restartTag:
          SYS_ResetSystem(SYS_RESTART,0,0);
          break;
        case shutdownTag:
          SYS_ResetSystem(SYS_POWEROFF,0,0);
          break;
        #endif
        case screenshotTag:
          gui.SavePngScreenshot();
          break;
        default:
          break;
        }
      }
    }
  }
}

void Dash::RunTopBottomBars()
{
  char temptext[100]; 
  int trans=OnOffSlide(0,61);

  sys.vid.ApplySurface(0,-70-trans,gfx.topBottomBorderImage,sys.vid.screen,NULL);

  sprintf(temptext,"%s%s%s%s%s%s%d",
      wdayNames[dashTime->tm_wday], " ",
      monthNames[dashTime->tm_mon], " ",
      dayNames[dashTime->tm_mday-1], " ",
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
  gui.SpriteText(20,screenHeight-61+trans+23,(char*)"thatotherdev.wordpress.com",1);
}


bool Dash::UserWantsOut()
{
  return userWantsOut;
}
