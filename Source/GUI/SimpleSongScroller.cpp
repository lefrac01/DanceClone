//      SimpleSongScroller.cpp
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

#include "SimpleSongScroller.h"

namespace Gooey
{


SimpleSongScroller::SimpleSongScroller() :
  currentOffset(-1),
  selectionOffset(-1),
  scrollOffset(-1)
{
}

SimpleSongScroller::SimpleSongScroller(int _x, int _y, int _w, int _h, int ta) :
  Element(_x, _y, _w, _h, true, true, false, "", ta),
  currentOffset(-1),
  selectionOffset(-1),
  scrollOffset(-1)
{
}

SimpleSongScroller::~SimpleSongScroller()
{
}

bool SimpleSongScroller::Clicked(int testx, int testy)
{
  LOG(DEBUG_DETAIL, "SimpleSongScroller::Clicked() ..." << endl)
  //internal ui handling.  if scroll buttons clicked, scroll contents
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    Button& b = buttons[i];
    if (b.Clicked(testx, testy))
    {
      // is it a scroll button
      if (b.tag == scrollUpTag || b.tag == scrollDownTag)
      {
        LOG(DEBUG_DETAIL, "SimpleSongScroller::Clicked() on a scroll button" << endl)
        // yes, update state
        if (b.tag == scrollUpTag)
        {
          --scrollOffset;
        }
        else
        {
          ++scrollOffset;
        }
        Recalculate();
      }
      else
      {
        // no, register selection
        LOG(DEBUG_DETAIL, "SimpleSongScroller::Clicked() new selection offset: " << i << endl)
        selectionOffset = i;
      }
      return true;
    }
  }
  return false;
}

vector<Button>& SimpleSongScroller::Buttons()
{
  return buttons;
}


void SimpleSongScroller::AddSongChoice(string s, int t)
{
  songChoices.push_back(s);
  songChoiceTags.push_back(t);
}

void SimpleSongScroller::Recalculate(int startOffset)
{
  LOG(DEBUG_DETAIL, "SimpleSongScroller::Recalculate() with " << songChoices.size() << " song choices" << endl)

  // clear buttons and selection
  buttons.clear();
  selectionOffset = -1;
  
  // calculate limits based on item size and self extents
  //TODO: for now using original hardcoded values
//  int maxItems = 8;
  unsigned int maxItems = 4; //TODO: temp for testing
  
  if (startOffset != -1 && startOffset <= (int)songChoices.size())
  {
    currentOffset = startOffset;
//    scrollOffset = 0; ???  //TODO: set based on currentOffset.. for coming back to menu on old selection
  }
  else
  {
    currentOffset = 0;
  }
  
  if (scrollOffset == -1)
  {
    scrollOffset = currentOffset;
  }
  
  // draw a scroll up button if it is needed (e.g. reduce scrollOffset)
  if (scrollOffset > 0)
  {
    LOG(DEBUG_DETAIL, "SimpleSongScroller::Recalculate() adding a scroll up button" << endl)
    //TODO: graphical button
    Button b("^", 320, 55, 600, 10, scrollUpTag);
//TODO: temp: all original menus use centered buttons.  hit detection is screwy elsewise so for the moment
// center in-place during add
b.x=b.x-b.w/2;b.y=b.y-b.h/2;
    
    buttons.push_back(b);
  }
  
  for (unsigned int i = scrollOffset; i < songChoices.size() && i <(maxItems+scrollOffset); i++)
  {
//    else if ((int)i >= scrollOffset && i - scrollOffset < (maxItems-1))
//    {
      //else if current index is in displayable range make a button
      //using first 50 chars of the song file title
      LOG(DEBUG_DETAIL, "SimpleSongScroller::Recalculate() adding a button at song index " << i << endl)
      Button b(songChoices[i].substr(0, 50), 320, 55 + (i-scrollOffset+1)*40, 600, 10, songChoiceTags[i]);
//TODO: temp: all original menus use centered buttons.  hit detection is screwy elsewise so for the moment
// center in-place during add
b.x=b.x-b.w/2;b.y=b.y-b.h/2;
      buttons.push_back(b);
//    }
  }
  
  // draw a scroll down button if it is needed (e.g. increase scrollOffset)
  if (songChoices.size() - scrollOffset > maxItems)
  {
    LOG(DEBUG_DETAIL, "SimpleSongScroller::Recalculate() adding a scroll down button" << endl)
    //TODO: graphical button
    Button b("v", 320, 55 + (maxItems+1)*40, 600, 10, scrollDownTag);
//TODO: temp: all original menus use centered buttons.  hit detection is screwy elsewise so for the moment
// center in-place during add
b.x=b.x-b.w/2;b.y=b.y-b.h/2;
    buttons.push_back(b);
  }
}

int SimpleSongScroller::GetSelectionOffset()
{
  return selectionOffset;
}

}
