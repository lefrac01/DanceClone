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
    if (!gfx.Init())
    {
      return false;
    }
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
    RunCreditsScreen();
    break;
  case SCORE:
    //#Game_menu_score();
    break;
  case SONG_SELECT1:
    RunSongSelect1();
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
    RunDebugScreen();
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
void Game::RunTitleScreen()
{
  // if we just got here, construct the screen object then pass to gui for rendering
  const int playTag = 1;
  const int creditsTag = 2;
  const int debugTag = 3;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunTitleScreen setting up title)" << endl)
    
    // FOR NOW, IDEA 1.
    Container title;
    int border = 40;
    
    Image titleImage(gfx.title_image, sys.vid.ScreenWidth()/2-300/2, 30, 0, 0);
    title.Add(titleImage);

    Button play("Play", border,55+4*40,sys.vid.ScreenWidth()-border*2,10, playTag);
    title.Add(play);

    Button credits("Credits", border,55+5*40,sys.vid.ScreenWidth()-border*2,10, creditsTag);
    title.Add(credits);

    Button debug("Debug", border,55+6*40,sys.vid.ScreenWidth()-border*2,10, debugTag);
    title.Add(debug);
    
    //title.SetRenderAnimation(GUI::SCREEN_WIPE); // FADE_IN // FADING_WIPE, etc
    gui.SetScreen(title);
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
      case playTag:
        state = SONG_SELECT1;
        break;
      case creditsTag:
        state = CREDITS;
        break;
      case debugTag:
        state = DEBUG;
        break;
      default:
        break;
      }
    }
  }
  
  LOG(DEBUG_GUTS, "Game::RunTitleScreen() done" << endl)
}

void Game::RunCreditsScreen()
{
  const int backButtonTag = 4;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunCreditsScreen setting up" << endl)
    Container credits;

    Label cred1("v0.50 Programming and graphics by ThatOtherPerson", sys.vid.ScreenWidth()/2, 200+20*0, 0, 0);
    Label cred2("thatotherdev.wordpress.com", sys.vid.ScreenWidth()/2, 200+20*1, 0, 0);

    Label cred3("v0.57 enhancements by Carl Lefrançois", sys.vid.ScreenWidth()/2, 200+20*3, 0, 0);

    Label cred4("Dance, NOW! music by RekcahDam", sys.vid.ScreenWidth()/2, 200+20*5, 0, 0);
    Label cred5("rekcahdam.blogspot.com", sys.vid.ScreenWidth()/2, 200+20*6, 0, 0);

    credits.Add(cred1);
    credits.Add(cred2);
    credits.Add(cred3);
    credits.Add(cred4);
    credits.Add(cred5);
    
    Button back("Back", sys.vid.ScreenWidth()-100-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    credits.Add(back);
    
    gui.SetScreen(credits);
    
    Uint32 soWhite;
    soWhite = SDL_MapRGB(sys.vid.screen->format, 255, 255, 255);
    gui.SetSpriteTextColored(soWhite);
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
      case backButtonTag:
        state = TITLE;
        break;
      default:
        break;
      }
    }
  }
}

void Game::RunDebugScreen()
{
  const int backButtonTag = 5;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunDebugScreen setting up" << endl)
    Container debug;

    int y=40;

    Label title("debug", 25, y, 0, 0);
    debug.Add(title);

   
    Image freezeHit(gfx.freeze_hit_image, 30, 70, 0, 0);
    debug.Add(freezeHit);
    
    Button back("Back", sys.vid.ScreenWidth()-300-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    debug.Add(back);
    
    gui.SetScreen(debug);

    Uint32 soTeal;
    soTeal = SDL_MapRGB(sys.vid.screen->format, 200, 255, 250);
    gui.SetSpriteTextColored(soTeal);
  }


  sys.vid.ApplySurface(80, 260, gfx.freeze_arrows_tail_image, NULL, &gfx.freeze_tail_frames[3]); //12
  sys.vid.ApplySurface(80, 132, gfx.freeze_arrows_body_image, NULL, &gfx.freeze_body_frames[3]); //12
  sys.vid.ApplySurface(80, 100, gfx.freeze_arrows_head_image, NULL, &gfx.freeze_head_frames[3]); //16

  vector<Button> buttons;
  buttons = gui.screen.AllButtons();
  for (unsigned int i = 0; i < buttons.size(); i++)
  {
    Button& b = buttons[i];
    if (b.clicked)
    {
      switch (b.tag)
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

void Game::RunSongSelect1()
{
  //TODO: preload the song info into a nifty structure with images and meta info
  const int backButtonTag = 6;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunSongSelect1 setting up" << endl)
    Container songSelect1;

    int entryNumber = 0;
    string temp;
  
    string mp3Extension = ".mp3";  
    vector<DirectoryEntry> musicFileRootContents;
    musicFileRootContents = sys.ReadDirectory(constants.musicFileRoot);

    SimpleSongScroller fileList;
    //Container fileList;

    for(unsigned int i = 0; i < musicFileRootContents.size(); i++)
    {
      DirectoryEntry& en = musicFileRootContents[i];
      
      if (!en.folder)
      {
        if (en.filename.size() > mp3Extension.size() && boost::iequals(en.filename.substr(en.filename.size()-4, 4), mp3Extension))
        {
          //Button fileSelect((char*)en.filename.c_str(), sys.vid.ScreenWidth()/2, 55 + entryNumber*40, 500, 10, constants.baseFileButtonTag+entryNumber);
          fileList.AddSongChoice(en.filename, constants.baseFileButtonTag+entryNumber);
          ++entryNumber;
        }
      }
    }
    
    fileList.Recalculate();
    
    songSelect1.Add(fileList);

    Button back("Back", sys.vid.ScreenWidth()-100-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    songSelect1.Add(back);
    
    gui.SetScreen(songSelect1);
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
      case backButtonTag:
        state = TITLE;
        break;
      default:
        LOG(DEBUG_BASIC, "Game::RunSongSelect1 selected song # " << b.tag-constants.baseFileButtonTag << endl)
        break;
      }
    }
  }
}


}
