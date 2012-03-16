//      WiiOS.cpp
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


#include "WiiOS.h"



namespace Platform
{

s8 HWButton;

void WiiOS::WiiResetPressed(){HWButton = SYS_RETURNTOMENU;}
void WiiOS::WiiPowerPressed(){HWButton = SYS_POWEROFF;}
void WiiOS::WiimotePowerPressed(s32 chan){HWButton = SYS_POWEROFF;}


WiiVideo wiiVideo;
WiiInput wiiInput;

WiiOS::WiiOS() :
  OS(wiiVideo, wiiInput)
{
}

void WiiOS::Init()
{  
  OS::Init();
  HWButton = -1;
  //input.WiiSetScreenExtents(vid.ScreenWidth(), vid.ScreenHeight());
  //TODO: call this function until a successful file read can be done
  fatInitDefault();
  //TEMP
  sleep(1);


  wiiInput.WiiInputInit(vid.ScreenWidth(), vid.ScreenHeight());
  SYS_SetResetCallback(WiiResetPressed);
  SYS_SetPowerCallback(WiiPowerPressed);
  WPAD_SetPowerButtonCallback(WiimotePowerPressed);  
}

void WiiOS::Pump()
{
  OS::Pump();

  if (HWButton != -1)
  {
    SYS_ResetSystem(HWButton, 0, 0);
  }
}

void WiiOS::Cleanup()
{
  OS::Cleanup();
  fatUnmount(0); 
}

vector<DirectoryEntry> WiiOS::ReadDirectory(string path)
{
  vector<DirectoryEntry> contents;

  struct stat st;
  struct dirent* dirEntry = NULL;
  DIR* dirStruct;

  if ((dirStruct = opendir(path.c_str())))
  {
    if(dirStruct->dirData != NULL)
    {
      while(  (dirEntry = readdir(dirStruct)) != NULL ) 
      {
        string filepath = "";
        bool isFolder = false;
        bool statFailed = true;
        int extra = 0;
        
        if (path == "/")
        {
          filepath = path;
          filepath += dirEntry->d_name;
        }
        else
        {
          filepath = path;
          filepath += "/";
          filepath += dirEntry->d_name;
        }
        if(stat(filepath.c_str(), &st) == 0) 
        {
          statFailed = false;
          if(st.st_mode & S_IFDIR)
          {
            isFolder = true;
          }
        }
        else
        {
          statFailed = true;
        }
        contents.push_back(DirectoryEntry(dirEntry->d_name, isFolder, statFailed, extra));
      }
    }
  }
  
  return contents;
}

}
