//      SimpleSongScroller.h
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

#ifndef GUI_SIMPLE_SONG_SCROLLER_H
#define GUI_SIMPLE_SONG_SCROLLER_H

#include <vector>
using std::vector;

#include "../Platform/LOG.H"

#include "Button.h"

namespace Gooey
{
  
class SimpleSongScroller : public Element
{
private:
  vector<Button> buttons;
  vector<string> songChoices;
  vector<int> songChoiceTags;
  int currentOffset;
  int selectionOffset;
  int scrollOffset;
  const static int scrollUpTag = 1;
  const static int scrollDownTag = 2;
  
public:
  SimpleSongScroller();
  virtual ~SimpleSongScroller();
  SimpleSongScroller(int _x, int _y, int _w, int _h, int ta = -1);
  bool Clicked(int testx, int testy);
  void CursorAt(int testx, int testy);
  void AddSongChoice(string s, int t);
  void Recalculate(int startOffset = -1);
  int GetSelectionOffset();
  vector<Button>& Buttons();
};


}

#endif
