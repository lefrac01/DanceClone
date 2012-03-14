//      LinuxOS.cpp
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


#include "LinuxOS.h"



namespace Platform
{

LinuxVideo linuxVideo;
LinuxInput linuxInput;

LinuxOS::LinuxOS() :
  OS(linuxVideo, linuxInput)
{
}


void LinuxOS::Init()
{  
  OS::Init();
}

void LinuxOS::Pump()
{
  OS::Pump();
}

void LinuxOS::Cleanup()
{
  OS::Cleanup();
}

vector<DirectoryEntry> LinuxOS::ReadDirectory(string path)
{
  vector<DirectoryEntry> contents;

  struct stat st;
  struct dirent* dirEntry = NULL;
  DIR* dirStruct;

  if ((dirStruct = opendir(path.c_str())))
  {
//    if(dirStruct->dirData != NULL)
    if(dirStruct != NULL)
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
