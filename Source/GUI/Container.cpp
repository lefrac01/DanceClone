//      Container.cpp
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

#include "Container.h"
#include "SimpleSongScroller.h"

namespace Gooey
{


Container::Container() : 
  Element()
{
}

Container::Container(int _x, int _y, int _w, int _h, int ta) :
  Element(_x, _y, _w, _h, true, true, false, "", ta)
{
}

Container::~Container()
{
}

vector<Container>& Container::Containers()
{
  return containers;
}

vector<SimpleSongScroller>& Container::SimpleSongScrollers()
{
  return simpleSongScrollers;
}

vector<Image>& Container::Images()
{
  return images;
}

vector<Button>& Container::Buttons()
{
  return buttons;
}

vector<Label>& Container::Labels()
{
  return labels;
}

void Container::Add(Element& e)
{
  // offset child to self
  e.x += x;
  e.y += y;
  try 
  {
    Container& c = dynamic_cast<Container&>(e);
    containers.push_back(c);
    LOG(DEBUG_MINOR, "Container::Add() container c: x: " << c.x << " y: " << c.y << endl)
  }
  catch (...){}
  try 
  {
    SimpleSongScroller& sss = dynamic_cast<SimpleSongScroller&>(e);
    simpleSongScrollers.push_back(sss);
    LOG(DEBUG_MINOR, "Container::Add() SimpleSongScroller sss: x: " << sss.x << " y: " << sss.y << endl)
  }
  catch (...){}
  {
  try 
  {
    //TODO: temp: all original menus use centered buttons.  hit detection is screwy elsewise so for the moment
    // center in-place during add
    
    Button& b = dynamic_cast<Button&>(e);
    //b.x=b.x-b.w/2;b.y=b.y-b.h/2;
    buttons.push_back(b);
    LOG(DEBUG_MINOR, "Container::Add() button b: x: " << b.x << " y: " << b.y << " w: " << b.w << " h: " << b.h << " text: " << b.text << " tag: " << b.tag << endl)
  }
  catch (...){}
  }
  try 
  {
    Image& i = dynamic_cast<Image&>(e);
    images.push_back(i);
    LOG(DEBUG_MINOR, "Container::Add() image i: x: " << i.x << " y: " << i.y << " w: " << i.w << " h: " << i.h << " text: " << i.text << " tag: " << i.tag << endl)
  }
  catch (...){}
  try 
  {
    Label& l = dynamic_cast<Label&>(e);
    labels.push_back(l);
    LOG(DEBUG_MINOR, "Container::Add() label i: x: " << l.x << " y: " << l.y << " w: " << l.w << " h: " << l.h << " text: " << l.text << " tag: " << l.tag << endl)
  }
  catch (...){}
  
}


bool Container::Clicked(int testx, int testy)
{
  LOG(DEBUG_GUTS, "Container::Clicked() x: " << testx << " y: " << testy << endl)
  //opt, if outside self extent, no need to check
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  if (!PointInSDLRect(testx, testy, r))
  {
    LOG(DEBUG_GUTS, "Container::Clicked() rejected point not in self" << endl)
    return false;
  }
  
  for (unsigned int i = 0; i < containers.size(); i++)
  {
    if (containers[i].Clicked(testx, testy))
    {
      return true;
    }
  }
  
  for (unsigned int i = 0; i < simpleSongScrollers.size(); i++)
  {
    if (simpleSongScrollers[i].Clicked(testx, testy))
    {
      return true;
    }
  }
  
  // check self
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    if (buttons[i].Clicked(testx, testy))
    {
      return true;
    }
  }
  
  return false;
}


void Container::CursorAt(int testx, int testy)
{
  LOG(DEBUG_GUTS, "Container::CursorAt() x: " << testx << " y: " << testy << endl)
  //opt, if outside self extent, no need to proceed
  SDL_Rect r;
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  if (!PointInSDLRect(testx, testy, r))
  {
    LOG(DEBUG_GUTS, "Container::CursorAt() rejected point not in self" << endl)
    return;
  }
  
  for (unsigned int i = 0; i < containers.size(); i++)
  {
    containers[i].CursorAt(testx, testy);
  }
  
  for (unsigned int i = 0; i < simpleSongScrollers.size(); i++)
  {
    simpleSongScrollers[i].CursorAt(testx, testy);
  }
  
  // check self
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    buttons[i].CursorAt(testx, testy);
  }
}

// return all buttons in self and any child containers
// functionality separate from Buttons() because that function is
// used for rendering.
vector<Button> Container::AllButtons()
{
  vector<Button> temp;
  
  for (unsigned int i = 0; i < containers.size(); i++)
  {
    temp.insert(temp.end(), containers[i].AllButtons().begin(), containers[i].AllButtons().end());
  }

  for (unsigned int i = 0; i < simpleSongScrollers.size(); i++)
  {
    temp.insert(temp.end(), simpleSongScrollers[i].Buttons().begin(), simpleSongScrollers[i].Buttons().end());
  }

  temp.insert(temp.end(), buttons.begin(), buttons.end());
  
  return temp;
}



}
