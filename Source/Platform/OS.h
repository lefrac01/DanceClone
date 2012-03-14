//      OS.h
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


//#define WIN
#define WII
//#define LINUX


#ifndef OS_H
#define OS_H

#include <vector>
using std::vector;

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Input.h"
#include "Video.h"


namespace Platform
{

class DirectoryEntry
{
public:
  string filename;
  bool folder;
  bool statfailed;
  int extrainfoint;
  DirectoryEntry(char* f, bool isf, bool sf, int extra);
};



class OS
{
private:
  
  OS();
  
public:

  Video& vid;
  Input& input;
  OS(Video& v, Input& i);

  virtual void Init();
  virtual void Cleanup();
  virtual void Pump();
  
  virtual vector<DirectoryEntry> ReadDirectory(string path)=0;  // yes, copying on stack...
};

}
#endif
