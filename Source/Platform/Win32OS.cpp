//      Win32OS.cpp
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


#include "Win32OS.h"

namespace Platform
{

void Win32OS::Init()
{
  OS::Init();
}

void Win32OS::Cleanup()
{
  OS::Cleanup();
}

vector<DirectoryEntry> Win32OS::ReadDirectory(string path)
{
  vector<DirectoryEntry> contents;
  
  //TODO: wind stuff
  /*
  while(n_direntries) deletedirentry(0);
  
  char filename[MAX_PATH];
  WIN32_FIND_DATAA findFileData;
  HANDLE hFind = FindFirstFileA(directory, &findFileData);
  if(hFind != INVALID_HANDLE_VALUE)
  {
    bool endoflist = 0;
    while(endoflist == 0)
    {
      sprintf(filename, "%s", findFileData.cFileName);

      direntries[n_direntries] = new direntry((char*)filename);
      n_direntries++;
      
      if(findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      {
        direntries[n_direntries]->folder = 1;
      }
      
      if(FindNextFileA(hFind, &findFileData) == 0)
      {
        endoflist = 1;
      }
    }
    //CloseHandle(hFind); //not compiled under win yet so leaving commented...
  }
  */
  return contents;
}

}
