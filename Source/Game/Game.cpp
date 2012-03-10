//      Game.cpp
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

#include "Game.h"

namespace DanceClone
{

Game::Game(OS& os, GUI& g) :
sys(os),
gui(g),
gfx(os)
{
}

bool Game::Init(string ConfigFilePath)
{
  try
  {
    constants.Init();
    gfx.Init();
  }
  catch (exception& e)
  {
    // log
    LOG(DEBUG_BASIC, "Game::Init error (" << e.what() << ")" << endl)
    ofstream error_log;
    error_log.open("errors", std::ios_base::app);
    error_log << " Game::Init() failed due to exception: " << endl;
    error_log << e.what();
    error_log.close();
    return false;
  }

  sound.Init();
  
  state = TITLE;
  gameStateChanged = true;
  
  return true;
}

void Game::Cleanup()
{
  gfx.Cleanup();
}

void Game::Run()
{
  //TODO: gfx are a resource.  have a class responsible for all the 
  // game drawing logic
  gfx.DrawBackground();

  GameState oldState = state;

  switch (state)
  {
  case TITLE:
    RunTitleScreen();
    break;
  case CREDITS:
    //#Game_menu_credits();
    break;
  case SCORE:
    //#Game_menu_score();
    break;
  case SONG_SELECT1:
    //#Game_menu_songselectp1();
    break;
  case SONG_SELECT2:
    //#Game_menu_songselectp2();
    break;
  case STEP_CREATE:
    //#Game_menu_stepcreate();
    break;
  case PLAY_PREP1:
    //#Game_playprep();
    break;
  case PLAY_PREP2:
    //#Game_playprep();
    break;
  case PLAY:
    //#Game_play();
    break;
  case DEBUG:
    //#Game_menu_debug();
    break;
  case NONE:
  default:
    state = TITLE;
    break;
  }

  gameStateChanged = oldState != state;
}

Game::GameState Game::State()
{
  return state;
}

void Game::RunTitleScreen()
{
  // if we just got here, construct the screen object then pass to gui for rendering
  const int debugTag = 1;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunTitleScreen setting up title)" << endl)
    
/*
    //IDEA 1 - basic
    // specify all position details statically, closest to actual code e.g.
    GUI::Image titleImage(gfx.title_image);
    titleImage.SetExtents(sys.vid.ScreenWidth()/2-300/2, 30, NULL, NULL); // NULL meaning image width + height
    
    //IDEA 2 - semi OO
    // specify position details in a relative way, referencing gui details by parameters e.g.
    GUI::Image titleImage(gfx.title_image);
    titleImage.SetExtents(-300/2, 30, NULL, NULL); // NULL meaning image width + height
    titleImage.SetAutoPosition(GUI::HORIZ_CENTER);   // GUI::ORIGIN would be 0,0, GUI::HORIZ_CENTER | GUI::VERT_CENTER would be sw/2, sh/2
      // GUI::HORIZ_AUTO_CENTER will set origin to center the element based on width and height

    //IDEA 3 - full OO
    // specify a container which will internally place elements based on a flow style e.g.
    GUI::Button but1(1, "but1");
    but1.SetHeight(GUI::PIXEL, 20);
    GUI::Button but2(2, "but2");
    but2.SetHeight(GUI::PIXEL, 20);
    GUI::Button but3(3, "but3");
    but3.SetHeight(GUI::PIXEL, 20);
    
    GUI::Container vertList(VERTICAL_LIST);
    vertList.SetWidth(GUI::PIXEL, 400);
    vertList.SetHeight(GUI::PERCENT, 75);
    vertList.SetLeft(GUI::PERCENT, 50);   // x = sw/2
    vertList.SetHCenter(true);            // x -= w/2
    vertList.SetTop(GUI::PERCENT, 20);
    vertList.SetVCenter(false);           // which is default?
    vertList.Add(but1);
    vertList.Add(but2);
    vertList.Add(but3);
    
    title.Add(vertList);
*/

    
    // FOR NOW, IDEA 1.  I WANT TO PLAY DAMMIT!
    Screen* title = new Screen();
    Button* debug = new Button("Debug", sys.vid.ScreenWidth()/2-300/2,55+6*40,sys.vid.ScreenWidth()-40,10, debugTag);
    title->Add(debug);

    Image* titleImage = new Image(gfx.title_image, sys.vid.ScreenWidth()/2-300/2, 30, 0, 0);
    title->Add(titleImage);
    
    //title.SetRenderAnimation(GUI::SCREEN_WIPE); // FADE_IN // FADING_WIPE, etc
    gui.SetScreen(title);
  }
  
  
  // call on the gui to give us an updated state of the inputs
  // execute logic based on these inputs
  // update screen components based on inputs
  // send back to gui
  
  for (unsigned int i = 0; i < gui.Elements().size(); i++)
  {
    Element* e = gui.Elements()[i];
    if (e->clicked)
    {
      switch (e->tag)
      {
      case debugTag:
LOG(DEBUG_BASIC, "Game::RunTitleScreen detected click on debugTag ged button)" << endl)
        state = DEBUG;
        break;
      default:
        break;
      }
    }
  }
  /*
  // for interactive screens, the state of elements is directly modified
  // not very protective of internal information...
  e.g.
  switch (el.tag)
  {
  case addPlayer:
    if (numPlayers < maxPlayers)
    {
      ++numPlayers;
      string n;
      n << numPlayers;
      // update player number panel
      for (int j = 0; j < s.Elements().size(); j++)
      {
        GUI::Element& el2 = s.Elements[j];
        if (el2.tag == playerNumberPanel)
        {
          el2.SetText(n);
        }
      }
    }
    else
    {
      sound.Play(snd.guiFailDing);
    }
    ...
  }
  */
  
  
  
  
  // PROCEDURAL:
  //sys.vid.ApplySurface(sys.vid.ScreenWidth()/2-300/2, 30, gfx.title_image, sys.vid.screen, NULL);
  
  //if(gui.DoButton(sys.vid.ScreenWidth()/2,55+4*40,sys.vid.ScreenWidth()-40,10,1,1,(char*)"Play")){state=PLAY;}
  //if(gui.DoButton(sys.vid.ScreenWidth()/2,55+5*40,sys.vid.ScreenWidth()-40,10,1,1,(char*)"Credits")){state=CREDITS;}
  //if(gui.DoButton(sys.vid.ScreenWidth()/2,55+6*40,sys.vid.ScreenWidth()-40,10,1,1,(char*)"Debug")){state=DEBUG;}
  LOG(DEBUG_GUTS, "Game::RunTitleScreen() done" << endl)

}


void Game::RunDebugScreen()
{
  const int backButtonTag = 1;
  if (gameStateChanged)
  {
    Screen debug;

    int y=40;

    //WiiDash_spritetext(25,y,(char*)temp_text.c_str(),1);
    //WiiDash_spritetext(180,420,(char*)temp_text.c_str(),1);
    Element* title = new Element(25, y, 0, 0, true, true, false, "debug");
    debug.Add(title);

    //apply_surface(80, 260, freeze_arrows_tail_image, screen, &freeze_tail_frames[3]); //12
    //apply_surface(80, 132, freeze_arrows_body_image, screen, &freeze_body_frames[3]); //12
    //apply_surface(80, 100, freeze_arrows_head_image, screen, &freeze_head_frames[3]); //16

    
    Image* freezeHit = new Image(gfx.freeze_hit_image, 30, 70, 0, 0);
    debug.Add(freezeHit);
    
    Button back("Back", sys.vid.ScreenWidth()-300-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
  }


  for (unsigned int i = 0; i < gui.Elements().size(); i++)
  {
    Element* e = gui.Elements()[i];
    if (e->clicked)
    {
      switch (e->tag)
      {
      case backButtonTag:
        state = TITLE;
        break;
      default:
        break;
      }
    }
  }
}

}
