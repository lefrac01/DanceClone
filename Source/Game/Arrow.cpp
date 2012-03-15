//      Arrow.cpp
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

#include "Arrow.h"

namespace DanceClone
{

Arrow::Arrow(int d, long t, long y, long l, int ty) : 
  direction(d),
  time(t),
  yPos(y),
  length(l),
  type(ty),
  rating(RATING_NONE),
  freezeRating(FREEZE_NONE),
  jump(false),
  hidden(false),
  animStartTime(-1)
{
}

  
void Arrow::Init()
{
  LOG(DEBUG_MINOR, "DanceClone::Arrow::Init()" << endl)
}

}
