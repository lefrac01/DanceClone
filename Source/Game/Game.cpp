//TODO:
/*

1b  assist tick, sons d'atmosphère

1c  arranger ça pour que les flèches n'apparaissent pas déja sur l'écran
  au début de la chanson

                UNE CLASSE DOIT RÉPONDRE À LA QUESTION "QUELLE EST LA PROCHAINE FLÈCHE
                COTABLE APRÈS X?" (DANS LA MÊME DIRECTION)
                
1e  visualisation des cotes

1f  animation des visualisations de cote

*/
//TODO: constant for title position so screens have flow

//TODO: jumps only count as 1 for combos and points...

//TODO: still scroll screen and arrows during intro delay so arrows
// don't pop up already on-screen

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
  SDL_WM_SetCaption(GAMEVERSIONSTRING, NULL);

  numPlayers = 0;
  for (unsigned int i = 0; i < sys.input.inputChannels.size(); ++i)
  {
    Player temp(sys.input.inputChannels[i]);
    temp.Init(i + 1); //initialise as player # starting at 1
    players.push_back(temp);
  }
  
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

  if (!sound.Init())
  {
    return false;
  }
  
  state = TITLE;
  gameStateChanged = true;
  
  return true;
}

void Game::Cleanup()
{
  gfx.Cleanup();
  sound.Cleanup();
}

void Game::Run()
{
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
    RunScoreScreen();
    break;
  case CHOOSE_NUM_PLAYERS:
    RunChooseNumPlayers();
    break;
  case CHOOSE_RECORD_FILE:
    RunChooseRecordFile();
    break;
  case SELECT_SONG:
    RunSelectSong();
    break;
  case LOADING_SONG:
    RunLoadingSong();
    break;
  case SELECT_DIFFICULTY:
    RunSelectDifficulty();
    break;
  case STEP_CREATE:
    //#Game_menu_stepcreate();
    break;
  case PLAY_PREP1:
  case PLAY_PREP2:
    RunPlayPrep();
    break;
  case PLAY:
    RunPlay();
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

  if (gameStateChanged)
  {
    FLUSH_LOG
  }
}

Game::GameState Game::State()
{
  return state;
}

void Game::RunTitleScreen()
{
  const int playTag = 1;
  const int creditsTag = 2;
  const int debugTag = 3;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunTitleScreen setting up" << endl)
    
    gui.SetSpriteTextColour(gfx.sdlBlack);
    
    Container title;
    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    title.Add(bg);
    
    Image titleImage(gfx.images[Graphics::Title], sys.vid.ScreenWidth()/2-300/2, 30, 0, 0);
    title.Add(titleImage);

    Button play("Play", sys.vid.ScreenWidth()/2, 55+4*40, 200, 10, playTag);
    play.offsetMode |= Element::HCenter;
    title.Add(play);

    Button credits("Credits", sys.vid.ScreenWidth()/2, 55+5*40, 200, 10, creditsTag);
    credits.offsetMode |= Element::HCenter;
    title.Add(credits);

    Button debug("Debug", sys.vid.ScreenWidth()/2, 55+6*40, 200, 10, debugTag);
    debug.offsetMode |= Element::HCenter;
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
        state = CHOOSE_NUM_PLAYERS;
        sound.PlaySample(Sound::Select);
        break;
      case creditsTag:
        state = CREDITS;
        sound.PlaySample(Sound::Select);
        break;
      case debugTag:
        state = DEBUG;
        sound.PlaySample(Sound::Select);
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
    LOG(DEBUG_MINOR, "Game::RunCreditsScreen setting up" << endl)
    Container credits;

    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    credits.Add(bg);
    
    Label cred1("v0.50 Programming and graphics by ThatOtherPerson", 40, 60+20*0, 0, 0);
    Label cred2("thatotherdev.wordpress.com", 40, 60+20*1);

    char temp[100];
    sprintf(temp, "%s enhancements by Carl Lefrancois", GAMEVERSION);
    Label cred3(temp, 40, 60+20*3);

    Label cred4("Dance, NOW! music by RekcahDam", 40, 60+20*5);
    Label cred5("rekcahdam.blogspot.com", 40, 60+20*6);
    
    credits.Add(cred1);
    credits.Add(cred2);
    credits.Add(cred3);
    credits.Add(cred4);
    credits.Add(cred5);
    
    Button back("Back", sys.vid.ScreenWidth()-100-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    back.colour = gfx.sdlBlack;
    credits.Add(back);
    
    gui.SetScreen(credits);
    
    gui.SetSpriteTextColour(gfx.sdlWhite);
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

void Game::RunLoadingSong()
{
  //funny temp screen! hihi
  static int c = -2;
  const int backButtonTag = 7;
  static bool ready = false;

  if (gameStateChanged)
  {
    LOG(DEBUG_MINOR, "Game::RunLoadingSong setting up" << endl)
    Container loadingSong;

    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    loadingSong.Add(bg);
    
    Label message("LOADING...", 240, 200+20*5);

    loadingSong.Add(message);
    
    gui.SetScreen(loadingSong);
    gui.SetSpriteTextColour(gfx.sdlWhite);
    c=-2;
    ready = false;
  }
  
  if (c==-2) c=-3;
  else if (c==-3)
  {
    LOG(DEBUG_MINOR, "Game::RunLoadingSong pt 2 of setup" << endl)
    Container loadingSong;

    Label message("LOADING...  READY!", 240, 200+20*5);

    loadingSong.Add(message);
    
    if(songs[currentSong].Prepare())
    {
      Button back("Continue", sys.vid.ScreenWidth()-200-40, sys.vid.ScreenHeight()-10-40, 100, 10);
      loadingSong.Add(back);
      ready = true;
    }
    else
    {
      Button back("Back", sys.vid.ScreenWidth()-200-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
      loadingSong.Add(back);
    }
    gui.SetScreen(loadingSong);
    
    c=-1;
  }
  else
  {
    switch(++c)
    {
      case 0: gui.SetSpriteTextColour(gfx.sdlWhite); break;
      case 1: gui.SetSpriteTextColour(gfx.sdlYellow); break;
      case 2: gui.SetSpriteTextColour(gfx.sdlGrey); break;
      case 3: gui.SetSpriteTextColour(gfx.sdlGreen); break;
    }
    if (c==4)c=-1;
  }  

  if (ready && sys.input.ButtonDown(-1, InputChannel::Button4))
  {
    sound.PlaySample(Sound::Select);
    state = PLAY_PREP1;
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
        state = SELECT_SONG;
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
    LOG(DEBUG_MINOR, "Game::RunDebugScreen setting up" << endl)
    Container debug;

    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    debug.Add(bg);
    
    int y=40;

    Label title("debug", 25, y);
    debug.Add(title);

    Image freezeHit(gfx.images[Graphics::ComboHit], 30, 70, 0, 0);
    debug.Add(freezeHit);
    
    Button back("Back", sys.vid.ScreenWidth()-200-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    debug.Add(back);
    
    gui.SetScreen(debug);

    gui.SetSpriteTextColour(gfx.sdlCyan);
    
    //#LOG(DEBUG_BASIC, "Custom spacer 0.2 to 112:" << endl)
    //#for (int i = 0; i <= 1000; i++)
    //#{
      //#float p = i/1000.0;
      //#float p2 = PFunc::Parametric(PFunc::Linear, p, 0.0, 2.0);
      //#LOG(DEBUG_BASIC, "p:" << p2 << " F:" << exp(sin(p2*p2)) << endl)
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << (sin(p*p*p*p*p*p*2*PI)+1.0)/2.0 << endl)
    //#}
    //#LOG(DEBUG_BASIC, "Sine 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << PFunc::Parametric(PFunc::Sine, p, 0.0, 1.0) << endl)
    //#}
    //#LOG(DEBUG_BASIC, "Cosine 0 to 1:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << PFunc::Parametric(PFunc::Cosine, p, 0.0, 1.0) << endl)
    //#}
    //#LOG(DEBUG_BASIC, "Cosine 1 to 0.5:" << endl)
    //#for (int i = 0; i <= 100; i++)
    //#{
      //#float p = i/100.0;
      //#LOG(DEBUG_BASIC, "p:" << p << " F:" << PFunc::Parametric(PFunc::Cosine, p, 1.0, 0.5) << endl)
    //#}
    //#
  }

  int y = 120;
  for (int i = 0; i < Element::NUM_FONTS; ++i)
  {
//    gui.DrawText(136, y, "abciWWiMMi#%jklABCDE", (Element::Font)i, SDL_MapRGBA(sys.vid.screen->format, 255, 255, 255, 255), 24);
    gui.DrawText(136, y, "The quick brown fox...", (Element::Font)i, SDL_MapRGBA(sys.vid.screen->format, 255, 255, 255, 255), 24);
    y += 30;
  }
   
  sys.vid.ApplySurface(80, 260, gfx.images[Graphics::FreezeArrowsTail], NULL, &gfx.freezeTailFrames[3]); //12
  sys.vid.ApplySurface(80, 132, gfx.images[Graphics::FreezeArrowsBody], NULL, &gfx.freezeBodyFrames[3]); //12
  sys.vid.ApplySurface(80, 100, gfx.images[Graphics::FreezeArrowsHead], NULL, &gfx.freezeHeadFrames[3]); //16

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

void Game::RunChooseNumPlayers()
{
  const int backButtonTag = 9;
  const int basePlayerChoiceTag = 100;
  static int numPlayersDetected = 0;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_MINOR, "Game::RunChooseNumPlayers setting up" << endl)
    
    numPlayersDetected = 0;
  }

  // detect number of dance mat connections
  int num = 0;
  for (unsigned int i = 0; i < sys.input.inputChannels.size() && i < 4; ++i)
  {
    if (sys.input.inputChannels[i].danceMatActive)
    {
      ++num;
    }
  }

  if (gameStateChanged || numPlayersDetected != num)
  {
    numPlayersDetected = num;
    
    Container chooseNumPlayers;
    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    chooseNumPlayers.Add(bg);
    
    Label title("Choose number of players", sys.vid.ScreenWidth() / 2, 25);
    title.colour = gfx.sdlWhite;
    title.offsetMode = Element::HCenter;
    chooseNumPlayers.Add(title);

    gui.SetSpriteTextColour(gfx.sdlBlack);
    
    // Detect max input channels and those with an active dance mat
    // add i < 4 to not use the wii balance board
    int tempx = 55;
    for (unsigned int i = 0; i < sys.input.inputChannels.size() && i < 4; ++i)
    {
      char temp[100];
      sprintf(temp, "%d", i + 1);
      Button b(temp, tempx, 200, 80, 60, basePlayerChoiceTag + i);
      b.active = sys.input.inputChannels[i].danceMatActive;
      if (b.active)
      {
        b.colour = gfx.sdlDarkGreen;
      }
      else
      {
        b.colour = gfx.sdlGrey;
      }
      chooseNumPlayers.Add(b);
      tempx += 100+20;
    }
   
    Button back("Back", sys.vid.ScreenWidth()-100-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    chooseNumPlayers.Add(back);
    
    gui.SetScreen(chooseNumPlayers);
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
        if (b.tag >= basePlayerChoiceTag)
        {
          numPlayers = b.tag - basePlayerChoiceTag + 1;
          sound.PlaySample(Sound::Select);
          state = CHOOSE_RECORD_FILE;
          LOG(DEBUG_BASIC, "Game::RunChooseNumPlayers got " << numPlayers << endl)
        }
        break;
      }
    }
  }
}

void Game::RunChooseRecordFile()
{
  const int backButtonTag = 10;
  const int baseRecordFileTag = 100;
  static bool redraw = false;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_MINOR, "Game::RunChooseRecordFile setting up" << endl)
    
    for (int i = 0; i < numPlayers; ++i)
    {
      players[i].recordFileNumber = -1;
    }
    redraw = true;
  }
  
  if (redraw)
  {
    redraw = false;
    Container chooseRecordFile;
    
    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    chooseRecordFile.Add(bg);
    
    gui.SetSpriteTextColour(gfx.sdlBlack);

    Label title("Choose record file", sys.vid.ScreenWidth() / 2, 25);
    title.colour = gfx.sdlWhite;
    title.offsetMode = Element::HCenter;
    chooseRecordFile.Add(title);

    // draw a button for each record file
    int drawX = 55;
    int drawY = 200;
    for (int i = 0; i < constants.maxRecordFile; ++i)
    {
      char temp[100];
      sprintf(temp, "Score %d", i + 1);
      Button b(temp, drawX, drawY, 80, 60, baseRecordFileTag + i);
      b.active = true;
      for (int j = 0; j < numPlayers; ++j)
      {
        if (players[j].recordFileNumber == i)
        {
          b.active = false;
          break;
        }
      }
      
      chooseRecordFile.Add(b);
      drawX += 120;
    }

   
    Button back("Back", sys.vid.ScreenWidth()-100-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    chooseRecordFile.Add(back);
    
    gui.SetScreen(chooseRecordFile);
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
        state = CHOOSE_NUM_PLAYERS;
        break;
      default:
        if (b.tag >= baseRecordFileTag)
        {
          for (int i = 0; i < numPlayers; ++i)
          {
            if (players[i].inputs.number == b.clickedBy)
            {
              LOG(DEBUG_MINOR, "Game::RunChooseRecordFile player at index " << i << " on input channel " << b.clickedBy << " picked score number " << b.tag - baseRecordFileTag << endl)
              players[i].recordFileNumber = b.tag - baseRecordFileTag;
              redraw = true;
              sound.PlaySample(Sound::Select);
              break;
            }
          }
          
          // if all players have chosen, change state...
          bool done = true;
          for (int i = 0; i < numPlayers; ++i)
          {
            if (players[i].recordFileNumber == -1)
            {
              done = false;
              break;
            }
          }
          
          if (done)
          {
            state = SELECT_SONG;
          }
        }
        break;
      }
    }
  }
}

void Game::RunSelectSong()
{
  static int preload = 0;
  static int menuIndex = -1;
  static int oldMenuIndex = -1;
  static bool setupAnim = false;
  static long animStartTime = -1;
  static long animEndTime = -1;
  static long animLength = 200;
  int elementPadding = constants.bannerSize * constants.bannerMiniZoomFactor / 3.0;
  static float elementIndexDistance = 0.0;
  static vector<float> elementAnimParameters;
  static int arcBase = sys.vid.ScreenHeight() / 2.0;
  static int arcHeight = 100;
  static int itemCenterPad = constants.bannerSize - constants.bannerSize / 4.0;
  static int numElements = -1;
  static int newFocusIndex = -1;
  static int oldFocusIndex = -1;
  static bool animToLeft = true;
  static SDL_Surface* newFocusZoomSurface = NULL;
  static SDL_Surface* oldFocusZoomSurface = NULL;
  long animTime = SDL_GetTicks();
  
  if (gameStateChanged)
  {
    preload = 1;
    
    LOG(DEBUG_MINOR, "Game::RunSelectSong setting up" << endl)
  }
  
  if (preload > 0)
  {
    if (preload == 1)
    {
      preload = 2;
      Container preload;
      Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
      preload.Add(bg);
      Label preloadMessage("Preloading file data...", 40, 40+20*2);
      preloadMessage.colour = gfx.sdlWhite;
      preload.Add(preloadMessage);
      gui.SetScreen(preload);
    }
    else if (preload == 2)
    {
      preload = 0;
      
      animStartTime = -1;

      if (menuIndex == -1)
      {
        // initial cursor position.  if already previously set, 
        // keep last cursor position.
        menuIndex = 0;
      }

      gui.SetSpriteTextColour(gfx.sdlBlack);
      
      // if there are no choices, preload songs.  worst case there are no songs
      // and the preload is done every time the menu is entered...
      if (songMenuItems.size() == 0)
      {
        PreloadSongs();
      }

      
      if (songMenuItems.size() == 0)
      {
        gui.hideCursor = false;
        Container noFileScreen;
        Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
        noFileScreen.Add(bg);
        Label noFiles1("No files with valid dance-single steps could be read", 40, 40+20*2);
        noFiles1.colour = gfx.sdlWhite;
        char temp[100];
        sprintf(temp, "%d mp3 files were found in the following directory:", (int)songs.size());
        Label noFiles2(temp, 40, 40+20*4);
        noFiles2.colour = gfx.sdlWhite;
        Label noFiles3(constants.musicFileRoot.c_str(), 40, 40+20*5);
        noFiles3.colour = gfx.sdlWhite;

        noFileScreen.Add(noFiles1);
        noFileScreen.Add(noFiles2);
        noFileScreen.Add(noFiles3);
        
        gui.SetScreen(noFileScreen);
      }
      else
      {
        gui.hideCursor = true;
        // don't actually animate, just do last frame and leave it there
        setupAnim = true;
        animStartTime = animTime;
        animEndTime = animStartTime;
      }
    }
  }

  if (setupAnim)
  {
    setupAnim = false;
    
    // calculate vector of element positions
    // calculate how many elements will appear on-screen, making sure it
    // is an odd number so the focus element is in the center, then add
    // two elements an off-screen element on each side
    numElements = sys.vid.ScreenWidth() / (constants.bannerSize / 2 * constants.bannerMiniZoomFactor + elementPadding);
    if (numElements % 2 == 0)
    {
      --numElements;
    }
    
    oldFocusIndex = numElements / 2;
    if (animToLeft)
    {
      newFocusIndex = oldFocusIndex + 1;
      oldMenuIndex = menuIndex - 1;
      if (oldMenuIndex < 0)
      {
        oldMenuIndex = songMenuItems.size() - 1;
      }
    }
    else
    {
      newFocusIndex = oldFocusIndex - 1;
      oldMenuIndex = menuIndex + 1;
      if (oldMenuIndex >= (int)songMenuItems.size())
      {
        oldMenuIndex = 0;
      }
    }
    
    elementIndexDistance = 1.0 / (numElements - 1);

    elementAnimParameters.resize(numElements);
    for (int i = 0; i < numElements; ++i)
    {
      elementAnimParameters[i] = PFunc::ParamByVal(PFunc::Linear, i, 0, numElements-1);
    }
  }
  if (animStartTime != -1)
  {
    
    if (animTime >= animEndTime)
    {
      animStartTime = -1;
    }

    // generate images based on menu index and place them 
    // relative to their position in the element parameter vector
    // if an element parameter is 0 it means extreme left and 1 means
    // extreme right.

    // given that menuIndex points at the songMenuItem in the
    // newFocusIndex slot, figure out the menu index corresponding to
    // the leftmost element
    int elementMenuIndex = menuIndex;
    for (int i = newFocusIndex; i > 0; --i)
    {
      --elementMenuIndex;
      if (elementMenuIndex < 0)
      {
        elementMenuIndex = songMenuItems.size() - 1;
      }
    }

    // build screen
    Container songSelect;

    Image bg(gfx.images[Graphics::SongSelectBg], 0, 0, 0, 0);
    songSelect.Add(bg);

    float animPct = PFunc::ParamByVal(PFunc::Square, animTime, animStartTime, animEndTime);
    
    for (int i = 0; i < numElements; ++i)
    {
      // position the element according to its base position stored
      // in the positions vector plus how much it has moved toward the
      // next position since the animation has started
      float elementBaseP = elementAnimParameters[i];
      float elementPOffset = elementIndexDistance * animPct;
      if (animToLeft)
      {
        elementPOffset *= -1;
      }
      
      // 0..1 value representing element's position from offscreen left to offscreen right
      // if < 0 or > 1 no need to create and draw the image
      float p = elementBaseP + elementPOffset;
      if (p > 0 && p < 1)
      {
        int screenX = 0;
        
        float arcP = PFunc::Parametric(PFunc::Linear, p, 0.05, 0.45);
        int screenY = arcBase - arcHeight * PFunc::Parametric(PFunc::Sine, arcP, 0, 1);
        
        // increase the spacing between mini banners at the center of the screen
        // the offset should be greater at center relative than sides
        float xP = 0;
        if (p < 0.45)
        {
          xP = PFunc::Parametric(PFunc::Square, p*2.222222222, 0.0, 1.0);
          screenX = PFunc::Parametric(PFunc::Linear, xP, 0 - constants.bannerSize / 2, sys.vid.ScreenWidth() / 2.0 - itemCenterPad);
        }
        else if (p < 0.5)
        {
          xP = PFunc::Parametric(PFunc::Linear, (p-0.45)*20, 0.0, 1.0);
          screenX = PFunc::Parametric(PFunc::Linear, xP, sys.vid.ScreenWidth() / 2.0 - itemCenterPad, sys.vid.ScreenWidth() / 2.0);
        }
        else if (p < 0.55)
        {
          xP = PFunc::Parametric(PFunc::Linear, (p-0.5)*20, 0.0, 1.0);
          screenX = PFunc::Parametric(PFunc::Linear, xP, sys.vid.ScreenWidth() / 2.0, sys.vid.ScreenWidth() / 2.0 + itemCenterPad);
        }
        else
        {
          float p2 = PFunc::ParamByVal(PFunc::Linear, p, 0.55, 1.0);
          xP = PFunc::Parametric(PFunc::Square, 1-p2, 0.0, 1.0);
          screenX = PFunc::Parametric(PFunc::Linear, xP, sys.vid.ScreenWidth() + constants.bannerSize / 2, sys.vid.ScreenWidth() / 2.0 + itemCenterPad);
        }

        // the item coming into focus zooms up and the item
        // going out of focus zooms down.
        if (i == newFocusIndex)
        {
          SDL_Surface* banner = songMenuItems[elementMenuIndex].banner ? songMenuItems[elementMenuIndex].banner : gfx.images[Graphics::DefaultBanner];
          SDL_Surface* final = NULL;

          // if a zoom surface was allocated, free it
          if (newFocusZoomSurface)
          {
            SDL_FreeSurface(newFocusZoomSurface);
            newFocusZoomSurface = NULL;
          }

          // if we are at the last step of drawing, use the preloaded image
          if (animStartTime == -1)
          {
            final = banner;
          }
          else
          {
            // else create a zoomed image according to the animation time and store pointer in a special var.
            // this creates a surface that must be freed.
            float zoomFactor = PFunc::Parametric(PFunc::Linear, animPct, constants.bannerMiniZoomFactor, 1.0);
            newFocusZoomSurface = zoomSurface(banner, zoomFactor, zoomFactor, 0);
            final = newFocusZoomSurface;
          }
          
          Image ban(final, screenX, screenY, 0, 0);
          ban.offsetMode = Element::HCenter | Element::VCenter;
          songSelect.Add(ban);
        }
        else if (i == oldFocusIndex)
        {
          SDL_Surface* banner = songMenuItems[elementMenuIndex].banner ? songMenuItems[elementMenuIndex].banner : gfx.images[Graphics::DefaultBanner];
          SDL_Surface* final = NULL;

          // if a zoom surface was allocated, free it
          if (oldFocusZoomSurface)
          {
            SDL_FreeSurface(oldFocusZoomSurface);
            oldFocusZoomSurface = NULL;
          }

          // if we are at the last step of drawing, use the preloaded image
          if (animStartTime == -1)
          {
            final = songMenuItems[elementMenuIndex].bannerMini ? songMenuItems[elementMenuIndex].bannerMini : gfx.images[Graphics::DefaultBannerMini];
          }
          else
          {
            // else create a zoomed image according to the animation time and store pointer in a special var
            float zoomFactor = PFunc::Parametric(PFunc::Linear, animPct, 1.0, constants.bannerMiniZoomFactor);
            oldFocusZoomSurface = zoomSurface(banner, zoomFactor, zoomFactor, 0);
            final = oldFocusZoomSurface;
          }

          Image ban(final, screenX, screenY, 0, 0);
          ban.offsetMode = Element::HCenter | Element::VCenter;
          songSelect.Add(ban);
        }
        else
        {
          // just draw a normal mini banner
          SDL_Surface* bannerMini = songMenuItems[elementMenuIndex].bannerMini ? songMenuItems[elementMenuIndex].bannerMini : gfx.images[Graphics::DefaultBannerMini];
          Image mini(bannerMini, screenX, screenY, 0, 0);
          mini.offsetMode = Element::HCenter | Element::VCenter;
          songSelect.Add(mini);
        }
        
      }
      
      // increment the menu index corresponding to the element
      ++elementMenuIndex;
      if (elementMenuIndex == (int)songMenuItems.size())
      {
        elementMenuIndex = 0;
      }
    }
    
    if (animStartTime == -1)
    {
      // add information about current selection
      // title, bpm(min and max),
      // difficulties available and for each: rating, record, full combo achieved
      Song& s = songMenuItems[menuIndex].song;
      Label title(s.Title(), sys.vid.ScreenWidth()/2, 23);
      title.font = Element::CardewThree;
      title.colour = gfx.sdlWhite;
      title.offsetMode |= Element::HCenter;
      songSelect.Add(title);
      
      char minBPM[100];
      char maxBPM[100];
      sprintf(minBPM, "%3.0f", s.MinBPM());
      sprintf(maxBPM, "%3.0f", s.MaxBPM());
      char bpmLabel[100];
      if (strcmp(minBPM, maxBPM) == 0)
      {
        sprintf(bpmLabel, "%s", minBPM);
      }
      else
      {
        sprintf(bpmLabel, "%s-%s", minBPM, maxBPM);
      }
      
      Label bpm(bpmLabel, 544, 58);
      bpm.font = Element::Cryta;
      bpm.colour = gfx.sdlBlueWhite;
      bpm.offsetMode = Element::HCenter | Element::VCenter;
      songSelect.Add(bpm);
      
      int diffY = 296;
      int diffX = 245;
      int diffH = 23;
      for (int i = 0; i < constants.numDifficulties; ++i)
      {
        if (s.DifficultyIsAvailable(i))
        {
          Label l(Constants::difficultyText[i], diffX, diffY + i * diffH, 100, 10);
          l.colour = gfx.sdlGrey;
          songSelect.Add(l);
          
          if (s.stepRatings[i] != -1)
          {
            char stepRatingLabel[100];
            sprintf(stepRatingLabel, "%d", s.stepRatings[i]);
            Label stepRating(stepRatingLabel, diffX + 110, diffY + i * diffH, 40, 10);
            stepRating.colour = gfx.sdlWhite;
            songSelect.Add(stepRating);
          }
          
          float topRecord = -1;
          bool fullCombo = false;
          for (int j = 0; j < numPlayers; ++j)
          {
            Player& p = players[j];
            if (s.GetRecord(p.recordFileNumber, i) > 0)
            {
              if (topRecord == -1 || s.GetRecord(p.recordFileNumber, i) > topRecord)
              {
                topRecord = s.GetRecord(p.recordFileNumber, i);
              }
            }
            if (s.GetFullCombo(p.recordFileNumber, i))
            {
              fullCombo = true;
            }
          }
          if (topRecord > 0)
          {
            char recordText[100];
            if (topRecord < 10)
            {
              sprintf(recordText, "%1.0f%%", topRecord);
            }
            else if (topRecord < 100)
            {
              sprintf(recordText, "%2.0f%%", topRecord);
            }
            else
            {
              sprintf(recordText, "%3.0f%%", topRecord);
            }
            Label record(recordText, diffX + 140, diffY + i * diffH, 60, 10);
            record.colour = gfx.sdlYellow;
            songSelect.Add(record);
          }
          if (fullCombo)
          {
            Label fc("FC", diffX + 180, diffY + i * diffH, 62, 10);
            fc.colour = gfx.sdlYellow;
            songSelect.Add(fc);
          }
        }
      }
      
      int radarY = 326;
      int radarX = 45;
      int radarH = 21;
      for (int i = constants.numDifficulties - 1; i >= 0; --i)
      {
        if (s.DifficultyIsAvailable(i))
        {
          char radarText[100];
          char radarPct[100];
          for (int j = 0; j < Song::NumRadarValues; ++j)
          {
            sprintf(radarPct, "%2.0f%%", s.radarValues[i][j]*100.0);
            switch(j)
            {
              case 0:
                sprintf(radarText, "Voltage %s", radarPct);
                break;
              case 1:
                sprintf(radarText, "Stream  %s", radarPct);
                break;
              case 2:
                sprintf(radarText, "Chaos   %s", radarPct);
                break;
              case 3:
                sprintf(radarText, "Freeze  %s", radarPct);
                break;
              case 4:
                sprintf(radarText, "Air     %s", radarPct);
                break;
            }
            Label radarInfo(radarText, radarX, radarY + j * radarH, 50, 10);
            radarInfo.colour = gfx.sdlBlueWhite;
            songSelect.Add(radarInfo);
          }
          break;
        }
      }
    }
    
    gui.SetScreen(songSelect);
  }

  if (sys.input.ButtonDown(-1, InputChannel::Button3))
  {
    gui.hideCursor = false;
    state = CHOOSE_RECORD_FILE;
  }
  else if (sys.input.ButtonDown(-1, InputChannel::Button4))
  {
    currentSong = songMenuItems[menuIndex].song.filename;
    state = SELECT_DIFFICULTY;
    sound.PlaySample(Sound::Select);
    LOG(DEBUG_BASIC, "chose song " << songMenuItems[menuIndex].song.filename << endl)
  }
  else if (sys.input.DirectionDown(-1, InputChannel::RIGHT))
  {
    if (++menuIndex == (int)songMenuItems.size())
    {
      menuIndex = 0;
    }
    setupAnim = true;
    animToLeft = true;
    animStartTime = SDL_GetTicks();
    animEndTime = animStartTime + animLength;
    sound.PlaySample(Sound::MenuNav);
  }
  else if (sys.input.DirectionDown(-1, InputChannel::LEFT))
  {
    if (--menuIndex < 0)
    {
      menuIndex = songMenuItems.size() - 1;
    }
    setupAnim = true;
    animToLeft = false;
    animStartTime = SDL_GetTicks();
    animEndTime = animStartTime + animLength;
    sound.PlaySample(Sound::MenuNav);
  }
}

void Game::RunSelectDifficulty()
{
  const int baseDifficultyTag = 200;

  const int baseY = 80;
  const int rowHeight = 40;
  const int x = 80;
  const int w = 150;
  static int maxDifficulty = 0;
  static int minDifficulty = 0;

  if (gameStateChanged)
  {
    LOG(DEBUG_MINOR, "Game::RunSelectDifficulty setting up" << endl)

    Container selectDifficulty;

    Image bg(gfx.images[Graphics::DefaultBg], 0, 0, 0, 0);
    selectDifficulty.Add(bg);
    
    bool firstDifficulty = true;
   
    for (int i = 0; i < constants.numDifficulties; ++i)
    {
      if (songs[currentSong].DifficultyIsAvailable(i))
      {
        if (firstDifficulty)
        {
          firstDifficulty = false;
          minDifficulty = i;
        }
        maxDifficulty = i;
        Button b(Constants::difficultyText[i], x, baseY + i * rowHeight, w, 10, baseDifficultyTag + i);
        selectDifficulty.Add(b);
      }
    }
    
    for (int i = 0; i < numPlayers; ++i)
    {
      players[i].difficulty = minDifficulty;
      players[i].ready = false;
    }
    
    gui.SetScreen(selectDifficulty);
  }

  // Apply player difficulty selection cursor according to currently selected difficulty
  for (int i = 0; i < numPlayers; ++i)
  {
    sys.vid.ApplySurface(x + w + 20 + i * 50, baseY - 16 + players[i].difficulty * rowHeight, gfx.images[Graphics::DifficultyCursor], NULL, &gfx.difficultyCursorFrames[players[i].ready ? 2+i : i]);
  }

  // directly check inputs for changing each player's difficulty
  for (int i = 0; i < numPlayers; ++i)
  {
    Player& p = players[i];
    if (!p.ready)
    {
      if (p.inputs.directionDown[InputChannel::UP] && p.difficulty > minDifficulty)
      {
        while (!songs[currentSong].DifficultyIsAvailable(--p.difficulty));
        sound.PlaySample(Sound::Switch);
        // getting double inputs on Wii only...
        usleep(10000);
      }
      else if (p.inputs.directionDown[InputChannel::DOWN] && p.difficulty < maxDifficulty)
      {
        while (!songs[currentSong].DifficultyIsAvailable(++p.difficulty));
        sound.PlaySample(Sound::Switch);
        // getting double inputs on Wii only...
        usleep(10000);
      }
      if (p.inputs.buttonDown[InputChannel::Button4])  // A button
      {
        p.ready = true;
        sound.PlaySample(Sound::Select);
      }
    }
    else if (p.inputs.buttonDown[InputChannel::Button3])  // B button
    {
      p.ready = false;
      sound.PlaySample(Sound::Select);
    }
  }
  

  bool allPlayersReady = true;
  for (int i = 0; i < numPlayers; ++i)
  {
    if (!players[i].ready)
    {
      allPlayersReady = false;
      break;
    }
  }
  if (allPlayersReady)
  {
    state = LOADING_SONG;
  }
  
  // check for back button
  if (sys.input.ButtonDown(-1, InputChannel::Button3))
  {
    //sound.PlaySample(Sound::Select);
    state = SELECT_SONG;
  }  
}
  
void Game::PreloadSongs()
{
  string mp3Extension = ".mp3";  
  vector<DirectoryEntry> musicFileRootContents;
  musicFileRootContents = sys.ReadDirectory(constants.musicFileRoot);

  for(unsigned int i = 0; i < musicFileRootContents.size(); i++)
  {
    DirectoryEntry& en = musicFileRootContents[i];
    
    if (!en.folder)
    {
      if (en.filename.size() > mp3Extension.size() && boost::iequals(en.filename.substr(en.filename.size()-4, 4), mp3Extension))
      {
        //map<string, Song> songs;        
        songs[en.filename].Init();
        if (songs[en.filename].Load(en.filename))
        {
          // song has loaded.  verify that it contains radar data.
          // if not, it will be prepared so radar data is created.
          songs[en.filename].VerifyRadarData();
          
          // song has valid steps so also create menu choice
          SongMenuItem temp(sys, songs[en.filename]);
          songMenuItems.push_back(temp);
        }
      }
    }
  }
}

void Game::RunPlayCleanup()
{
  sound.StopMusic();
  
  if (songs[currentSong].backgroundImageFilename != "")
  {
    SDL_FreeSurface(songBg);
  }
  songBg = NULL;
  
  gui.hideCursor = false;
}

void Game::RunPlayPrep()
{

  if (state == PLAY_PREP1)
  {
    LOG(DEBUG_MINOR, "Game::RunPlayPrep setting up" << endl)
    
    // Initialise music
    sound.PrepMusic(songs[currentSong].Path());
    
    songBg = NULL;
    if (songs[currentSong].backgroundImageFilename.size() > 0)
    {
      songBg = sys.vid.LoadOptimize(songs[currentSong].BackgroundImagePath());
      if (songBg == NULL)
      {
        // the path doesn't work!! noooes!
        // must set to blank or free surface will be called ...
        songs[currentSong].backgroundImageFilename = "";
        songBg = gfx.images[Graphics::DefaultBg];
      }
    }
    else
    {
      songBg = gfx.images[Graphics::DefaultBg];
    }
    
    preStartTime = -1;
  
    switch(numPlayers)
    {
      case 1:
        players[0].arrowFieldXOffset = sys.vid.ScreenWidth() / 2 - constants.playerArrowColumnWidth * 4 / 2;

      break;
      case 2:
        players[0].arrowFieldXOffset = constants.playerArrowFieldMargin;
        players[1].arrowFieldXOffset = sys.vid.ScreenWidth() - constants.playerArrowColumnWidth * 4 - constants.playerArrowFieldMargin;
      break;
    }
  
    Container blank;
    gui.SetScreen(blank);
    state = PLAY_PREP2;
  }  
  
  //TEMP: ripped code from drawing routine to at least show home arrows during pre-play pause
  // other 15 16ths of quarter, draw lit home arrow only if player presses it

  // bg
  sys.vid.ApplySurface(0, 0, songBg, sys.vid.screen, NULL);
  
  
  for (int i = 0; i < numPlayers; ++i)
  {
    Player& p = players[i];
    sys.vid.ApplySurface(p.arrowFieldXOffset + 0 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[6]);
    sys.vid.ApplySurface(p.arrowFieldXOffset + 1 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[2]);
    sys.vid.ApplySurface(p.arrowFieldXOffset + 2 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[10]);
    sys.vid.ApplySurface(p.arrowFieldXOffset + 3 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[14]);
  }
  
  // getready.png 300 x 120
  sys.vid.ApplySurface(gfx.screenWidth/2 - 300/2, gfx.screenHeight/2 - 120/2, gfx.images[Graphics::GetReady], NULL, NULL);
  
  if (PreStartDelayFinished())
  {
    LOG(DEBUG_MINOR, "preparing for PLAY state" << endl)
    currentBpmChange = -1;
    currentMeasure = 0;
    currentBeat = 0;
    currentBeatFraction = 0.0;
    partialBeatFraction = 0.0;
    lastBeatTime = 0;
    beatTimeElapsedAtPreviousBPMs = 0;
    beatFractionAtPreviousBPMs = 0.0;
    songStartTime = SDL_GetTicks();
    songTime = 0;
    songAbortStartTime = -1;
    
    for (int i = 0; i < numPlayers; i++)
    {
      Player&p = players[i];
      p.Prepare();
      p.arrows = songs[currentSong].arrows[p.difficulty];
      LOG(DEBUG_MINOR, "copied " << p.arrows.size() << " arrows" << " for player " << i << " at difficulty " << p.difficulty << endl)
      p.numArrows = p.arrows.size();
      if (p.numArrows)
      {
          p.nextOffscreenArrow = 0;
      }
    }

    numBpmChanges = songs[currentSong].bpmChanges.size();
    if (numBpmChanges > 0)
    {
      currentBpmChange = 0;
      pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * songs[currentSong].bpmChanges.front().bpm;
    }
    
    if (songs[currentSong].prepared && currentBpmChange == 0)
    {
      LOG(DEBUG_MINOR, "RunPlayPrep() finished ok with initial bpm: " << songs[currentSong].bpmChanges.front().bpm << endl)
      sound.StartMusic();

      // synchronise play data just after mp3 code has had a chance
      // to finish beginning play
      
      InitialFrame();
      
      // switch to play state
      state = PLAY;
    }
    else
    {
      // game state should not be moving forward.  time to go back to the menu!
      state = SELECT_SONG;
      RunPlayCleanup();
    }
  }
}

bool Game::PreStartDelayFinished()
{
  if (preStartTime == -1)
  {
    preStartTime = SDL_GetTicks();
    LOG(DEBUG_GUTS, "Game::PreStartDelayFinished initialised to " << preStartTime << endl)
  }
  LOG(DEBUG_GUTS, "checking if " << SDL_GetTicks() << " - " << preStartTime << " > " << constants.preStartDelay << endl)
  if ((long)SDL_GetTicks() - preStartTime > constants.preStartDelay)
  {
    preStartTime = -1;
    return true;
  }
  else
  {
    return false;
  }
}

void Game::RunScoreScreen()
{
  //TODO: animation
  
  const int sideMargin = 130;
  const int topMargin = 70;
  int playerColumnWidth = (sys.vid.ScreenWidth() - sideMargin * 2) / numPlayers;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_MINOR, "Game::RunScoreScreen()" << endl)
  
    Container score;
    Image bg(gfx.images[Graphics::ScoreBg], 0, 0, 0, 0);
    score.Add(bg);

    for (int i = 0; i < numPlayers; ++i)
    {
      Player& p = players[i];
      
      char temp[100];
      sprintf(temp, "Player %d", i+1);
      Label pTitle(temp, sideMargin + playerColumnWidth * i, topMargin);
      pTitle.offsetMode = Element::Top | Element::Left;
      pTitle.colour = gfx.sdlWhite;
      score.Add(pTitle);
      
      // score total
      // using formulas generously published at http://aij.ddruk.com/taren/scoring/ddrscoreframe.html
      int numArrows = p.arrows.size();
      int numMarvellous = 0;
      int numPerfect = 0;
      int numGreat = 0;
      int numGood = 0;
      int numBoo = 0;
      int numMissed = 0;
      int numNG = 0;
      int numOK = 0;
      for (int j = 0; j < numArrows; ++j)
      {
        Arrow& ar = p.arrows[j];
        if (ar.rating == Arrow::MARVELLOUS)
        {
          ++numMarvellous;
        }
        else if (ar.rating == Arrow::PERFECT)
        {
          ++numPerfect;
        }
        else if (ar.rating == Arrow::GREAT)
        {
          ++numGreat;
        }
        else if (ar.rating == Arrow::GOOD)
        {
          ++numGood;
        }
        else if (ar.rating == Arrow::BOO)
        {
          ++numBoo;
        }
        else
        {
          ++numMissed;
        }
        if (ar.type == Arrow::HOLD)
        {
          if (ar.freezeRating == Arrow::FREEZE_FAILED)
          {
            ++numNG;
          }
          else
          {
            ++numOK;
          }
        }
      }

      //TODO: support larger font d00d!
      int scoreOutHeight = 20;
      float maxDancePoints = numArrows*2 + numOK*6 + numMissed*6;
      float playerDancePoints = numMarvellous*2 + numPerfect*2 + numGreat + numOK*6 - numGood*4 - numMissed*8;
      float scorePct = maxDancePoints == 0 ? 0 : playerDancePoints / maxDancePoints * 100.0;
      //NOTE: very easy to get negative scoring.  at least because my test songs are very short, the penalties allow
      // for going negative.  in DDR, this DancePoint score is only used to display a letter grade from
      // AAA to E with D being less than 45%.  a negative score would then be D.
      // the options are to not use % scoring in this code or to do the entire DDR play point calculation
      // and display that on the score screen.
      
      string letterRank = "E";
      if (scorePct > 99.9)
      {
        letterRank = "AAA";
      }
      else if (scorePct > 93.0)
      {
        letterRank = "AA";
      }
      else if (scorePct > 80.0)
      {
        letterRank = "A";
      }
      else if (scorePct > 65.0)
      {
        letterRank = "B";
      }
      else if (scorePct > 45.0)
      {
        letterRank = "C";
      }
      else if (scorePct > 0.0)
      {
        letterRank = "D";
      }
      sprintf(temp, " %s   (%% %3.2f)", letterRank.c_str(), scorePct);
      Label pScorePct(temp,  sideMargin + playerColumnWidth * i, topMargin + 50, 0, 0);
      pScorePct.colour = gfx.sdlYellow;
      score.Add(pScorePct);

      sprintf(temp, "Marvellous:  %d", numMarvellous);
      int drawX = sideMargin + playerColumnWidth * i;
      int drawY = topMargin + 120;
      Label pNumMarvellous(temp, drawX, drawY);
      pNumMarvellous.colour = gfx.sdlWhite;
      score.Add(pNumMarvellous);

      drawY += scoreOutHeight;
      sprintf(temp, "Perfect:     %d", numPerfect);
      Label pNumPerfect(temp, drawX, drawY);
      pNumPerfect.colour = gfx.sdlLightBlue;
      score.Add(pNumPerfect);

      drawY += scoreOutHeight;
      sprintf(temp, "Great:       %d", numGreat);
      Label pNumGreat(temp, drawX, drawY);
      pNumGreat.colour = gfx.sdlDarkGreen;
      score.Add(pNumGreat);

      drawY += scoreOutHeight;
      sprintf(temp, "Good:        %d", numGood);
      Label pNumGood(temp, drawX, drawY);
      pNumGood.colour = gfx.sdlBlue;
      score.Add(pNumGood);

      drawY += scoreOutHeight;
      sprintf(temp, "Boo:         %d", numBoo);
      Label pNumBoo(temp, drawX, drawY);
      pNumBoo.colour = gfx.sdlRed;
      score.Add(pNumBoo);

      drawY += scoreOutHeight;
      sprintf(temp, "Missed:      %d", numMissed);
      Label pNumMissed(temp, drawX, drawY);
      pNumMissed.colour = gfx.sdlDarkRed;
      score.Add(pNumMissed);

      drawY += scoreOutHeight + 10;
      sprintf(temp, "Max Combo:   %d", p.longestCombo);
      Label pMaxCombo(temp, drawX, drawY);
      pMaxCombo.colour = gfx.sdlWhite;
      score.Add(pMaxCombo);

      if (numMissed == 0)
      {
        drawY += scoreOutHeight;
        Label pFullCombo("FULL COMBO!", drawX, drawY);
        pFullCombo.colour = gfx.sdlWhite;
        score.Add(pFullCombo);
      }
      
      
      bool write = false;
      if (scorePct > songs[currentSong].GetRecord(p.recordFileNumber, p.difficulty))
      {
        songs[currentSong].SetRecord(p.recordFileNumber, p.difficulty, scorePct);
        write = true;
        
        Label pNewScore("New record!",  sideMargin + playerColumnWidth * i, topMargin + 80, 0, 0);
        pNewScore.colour = gfx.sdlWhite;
        score.Add(pNewScore);
      }
      if (numMissed == 0)
      {
        songs[currentSong].SetFullCombo(p.recordFileNumber, p.difficulty);
        write = true;
      }
      if (write)
      {
        songs[currentSong].WriteRecords();
      }      
    }
    
    gui.SetScreen(score);
  }
  
  if (sys.input.ButtonDown(-1, InputChannel::Button4))
  {
    sound.PlaySample(Sound::Select);
    state = SELECT_SONG;
  }
}

void Game::RunPlay()
{
  LOG(DEBUG_MINOR, "Game::RunPlay" << endl)
  for (unsigned int i = 0; i < players.size(); i++)
  {
    players[i].DoJumpProcessing(songTime);
  }

  LOG(DEBUG_GUTS, "Game::Play" << endl)

  // update 
  Frame();
  
  // bg
  sys.vid.ApplySurface(0, 0, songBg, sys.vid.screen, NULL);
  
  // animate home arrows
  int current16thTick = (int)(currentBeatFraction / 0.25);
  for (int i = 0; i < numPlayers; ++i)
  {
    Player& p = players[i];  
  
    if (current16thTick == 0)
    {
      // first 16th of a quarter note, draw flashing home arrows
      sys.vid.ApplySurface(p.arrowFieldXOffset + 0 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[4]);
      sys.vid.ApplySurface(p.arrowFieldXOffset + 1 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[0]);
      sys.vid.ApplySurface(p.arrowFieldXOffset + 2 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[8]);
      sys.vid.ApplySurface(p.arrowFieldXOffset + 3 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[12]);
    }
    else
    {
      // other 15 16ths of quarter, draw lit home arrow only if player presses it
      sys.vid.ApplySurface(p.arrowFieldXOffset + 0 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::LEFT]  ? 4  : 6]);
      sys.vid.ApplySurface(p.arrowFieldXOffset + 1 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::DOWN]  ? 0  : 2]);
      sys.vid.ApplySurface(p.arrowFieldXOffset + 2 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::UP]    ? 8  : 10]);
      sys.vid.ApplySurface(p.arrowFieldXOffset + 3 * constants.playerArrowColumnWidth, constants.goalOffset, gfx.images[Graphics::HomeArrows], NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::RIGHT] ? 12 : 14]);
    }
  }    


  // ratings
  
  // clean expired ratings.  oldest ratings are at the front.
  while (ratingDecals.size() > 0 && ratingDecals.front().animStartTime + ratingDecals.front().animDuration < songTime)
  {
    ratingDecals.erase(ratingDecals.begin());
  }
  
  for (int i = 0; i < numPlayers; ++i)
  {
    Player& p = players[i];  
    
    // draw ratings from oldest to newest
    // center in player's field and just below home arrows
    // rating decals drift downward over the course of their animation
    int decalX = p.arrowFieldXOffset + 2 * constants.playerArrowColumnWidth - constants.playerArrowColumnWidth / 2;
    int decalY = constants.goalOffset + 100;
    int decalAnimH = 32;
    for (unsigned int j = 0; j < ratingDecals.size(); ++j)
    {
      Decal& d = ratingDecals[j];
      if (d.player == i)
      {
        SDL_Rect* tempSrcRect = d.CurrentFrameRect(songTime);
        if (tempSrcRect)
        {
          int tempY = decalY + (int)(decalAnimH * ((songTime - d.animStartTime) / (float)d.animDuration));
          sys.vid.ApplySurface(decalX, tempY, d.surface, sys.vid.screen, tempSrcRect);
        }
      }
    }
  }  

  
  // animate arrows
  long offscreenHigh = viewportOffset + sys.vid.ScreenHeight() - constants.goalOffset;
  long offscreenLow = viewportOffset - gfx.arrowHeight - constants.goalOffset;
  for (int i = 0; i < numPlayers; ++i)
  {
    Player&p = players[i];
        
    LOG(DEBUG_DETAIL, "begin animation player " << i << " arrows. base_arrow = " << p.baseArrow << endl)
    
    // update first and last visible arrows
    while (p.nextOffscreenArrow != -1 && p.arrows[p.nextOffscreenArrow].yPos < offscreenHigh)
    {
      p.lastVisibleArrow = p.nextOffscreenArrow;
      if (p.firstVisibleArrow == -1)
      {
        p.firstVisibleArrow = p.lastVisibleArrow;
      }
      ++p.nextOffscreenArrow;
      if (p.nextOffscreenArrow == p.numArrows)
      {
        p.nextOffscreenArrow = -1;
      }
    }

    // update first visible arrow   NOTE: hold arrow isn't offscreen until tail is!
    if (p.firstVisibleArrow != -1)
    {
      while (p.arrows[p.firstVisibleArrow].yPos + p.arrows[p.firstVisibleArrow].length <= offscreenLow)
      {
        if (p.firstVisibleArrow == p.lastVisibleArrow)
        {
          LOG(DEBUG_DETAIL, "clearing last, first visible " << endl)
          p.firstVisibleArrow = -1;
          p.lastVisibleArrow = -1;
          break;
        }
        else
        {
          ++p.firstVisibleArrow;
        }
      }
    }

    LOG(DEBUG_DETAIL, "entering draw: firstVisibleArrow: " << p.firstVisibleArrow << " lastVisibleArrow: = " << p.lastVisibleArrow << endl)
    
    // draw arrows from last to first, updating first if it is offscreen
    if (p.firstVisibleArrow != -1)
    {
      for (int a = p.lastVisibleArrow; a >= p.firstVisibleArrow; a--)
      {
        Arrow& ar = p.arrows[a];
        
        //arrows become hidden if the player clears them
        if (!ar.hidden)
        {
          int screenYPos = ar.yPos - viewportOffset + constants.goalOffset;
          
          int arrowsFramesIndex = -1;
          int xpos = -1;
          switch (ar.direction)
          {
            case 0: xpos = p.arrowFieldXOffset + 0 * constants.playerArrowColumnWidth; arrowsFramesIndex =  4+current16thTick; break;
            case 1: xpos = p.arrowFieldXOffset + 1 * constants.playerArrowColumnWidth; arrowsFramesIndex =  0+current16thTick; break;
            case 2: xpos = p.arrowFieldXOffset + 2 * constants.playerArrowColumnWidth; arrowsFramesIndex =  8+current16thTick; break;
            case 3: xpos = p.arrowFieldXOffset + 3 * constants.playerArrowColumnWidth; arrowsFramesIndex = 12+current16thTick; break;
            default: break;
          }
          
          LOG(DEBUG_DETAIL, "draw arrow index:" << a << " screenYPos:" << screenYPos << " direction:" << ar.direction  << " arrowsFramesIndex:" << arrowsFramesIndex  << endl)
          
          if (ar.type == Arrow::HOLD)
          {
            // freeze arrow has three states, unhit, held and missed
            // the bitmaps are organised so that each state is in a separate
            // block.  the frame index pointer begins at a base value that 
            // points at the correct block for the correct state
            int freeze_tail_frame_index = -1;
            int freeze_body_frame_index = -1;
            int freeze_head_frame_index = -1;
            if (ar.freezeRating == Arrow::FREEZE_FAILED)
            {
              //dark index, failed state
              freeze_tail_frame_index = 4;
              freeze_body_frame_index = 4;
              freeze_head_frame_index = 4;
            }
            else if (ar.rating == Arrow::RATING_NONE)
            {
              // normal unhit state
              freeze_tail_frame_index = 0;
              freeze_body_frame_index = 0;
              freeze_head_frame_index = 0;
            }
            else
            {
              // being held
              freeze_tail_frame_index = 8;
              freeze_body_frame_index = 8;
              // animate the head based on time since start
              if ((songTime - ar.animStartTime) % 2)
              {
                freeze_head_frame_index = 8;
              }
              else
              {
                freeze_head_frame_index = 12;
              }
            }
            
            // draw tail from bottom up to head
            int current_freeze_top = screenYPos + 32;  // freeze arrows start from the middle of the head
            int current_freeze_bottom = screenYPos + 64 + ar.length; // bottom as in bottom of blit rect
            int current_freeze_length = ar.length + 32; // part of the freeze tail is overlapped by the head
            int blit_height = -1; // SDL_BlitSurface uses dest rect as output... just to be safe

            // all freeze bitmaps are indexed according to direction
            freeze_tail_frame_index += ar.direction;
            freeze_body_frame_index += ar.direction;
            freeze_head_frame_index += ar.direction;
            
            // if being held, arrows only draw up to the home arrows
            if ((ar.rating != Arrow::RATING_NONE) && (ar.freezeRating != Arrow::FREEZE_FAILED))
            {
              if (current_freeze_top < constants.goalOffset + 32)
              {
                current_freeze_length -= constants.goalOffset + 32 - current_freeze_top;
                current_freeze_top = constants.goalOffset + 32;
              }
            }

            
            // clip to screen - body and tail may be hidden or partial
            if (current_freeze_top < sys.vid.ScreenHeight())
            {
              SDL_Rect freeze_src_rect = gfx.freezeTailFrames[freeze_tail_frame_index];
              SDL_Rect freeze_dest_rect;
              freeze_dest_rect.x = xpos;
              freeze_dest_rect.w = 64;

              // tail
              if (current_freeze_length >= 64)
              {
                blit_height = 64;
                freeze_dest_rect.h = 64;
                freeze_dest_rect.y = current_freeze_bottom - 64;
              }
              else
              {
                blit_height = current_freeze_length;
                freeze_src_rect.h = current_freeze_length;
                freeze_src_rect.y += 64 - current_freeze_length;
                freeze_dest_rect.h = current_freeze_length;
                freeze_dest_rect.y = current_freeze_bottom - current_freeze_length;
              }

              SDL_BlitSurface(gfx.images[Graphics::FreezeArrowsTail], &freeze_src_rect, sys.vid.screen, &freeze_dest_rect);
              current_freeze_bottom -= blit_height;
              current_freeze_length -= blit_height;
              
              // body
              freeze_src_rect = gfx.freezeBodyFrames[freeze_body_frame_index];
              while (current_freeze_length > 0)
              {
                // just in case freeze dest rect gets clipped by the blit func (shouldn't happen)
                freeze_dest_rect.x = xpos;
                freeze_dest_rect.w = 64;
                
                // align blit source so that colours in freeze body line up
                if (current_freeze_length >= 128)
                {
                  blit_height = 128;
                  freeze_dest_rect.h = 128;
                  freeze_dest_rect.y = current_freeze_bottom - 128;
                }
                else
                {
                  // partial body.  source rect is bottom-up
                  blit_height = current_freeze_length;
                  freeze_src_rect.h = current_freeze_length;
                  freeze_src_rect.y += 128 - current_freeze_length;
                  freeze_dest_rect.h = current_freeze_length;
                  freeze_dest_rect.y = current_freeze_bottom - current_freeze_length;
                }
                SDL_BlitSurface(gfx.images[Graphics::FreezeArrowsBody], &freeze_src_rect, sys.vid.screen, &freeze_dest_rect);
                current_freeze_bottom -= blit_height;
                current_freeze_length -= blit_height;
              }
            }
            // head
            // if being held, arrow head is drawn over home arrow
            if ((ar.rating != Arrow::RATING_NONE) && (ar.freezeRating != Arrow::FREEZE_FAILED))
            {
              screenYPos = constants.goalOffset;
            }
            
            sys.vid.ApplySurface(xpos, screenYPos, gfx.images[Graphics::FreezeArrowsHead], NULL, &gfx.freezeHeadFrames[freeze_head_frame_index]);
          }
          else
          {
            SDL_Surface* arrowBitmapSrc = NULL;
            switch(ar.type)
            {
            case Arrow::QUARTER:
              arrowBitmapSrc = gfx.images[Graphics::QuarterArrows];
              break;
            case Arrow::EIGHTH:
              arrowBitmapSrc = gfx.images[Graphics::EighthArrows];
              break;
            case Arrow::QUARTER_TRIPLET:
              arrowBitmapSrc = gfx.images[Graphics::QuarterTripletArrows];
              break;
            case Arrow::SIXTEENTH:
              arrowBitmapSrc = gfx.images[Graphics::SixteenthArrows];
              break;
            case Arrow::EIGHTH_TRIPLET:
              arrowBitmapSrc = gfx.images[Graphics::EighthTripletArrows];
              break;
            case Arrow::THIRTYSECOND:
              arrowBitmapSrc = gfx.images[Graphics::ThirtysecondArrows];
              break;
            case Arrow::SIXTEENTH_TRIPLET:
              arrowBitmapSrc = gfx.images[Graphics::SixteenthTripletArrows];
              break;
            case Arrow::SIXTYFOURTH:
              arrowBitmapSrc = gfx.images[Graphics::SixtyfourthArrows];
              break;
            //NOTE: same graphics used for 96th and 192nd
            case Arrow::THIRTYSECOND_TRIPLET:
            case Arrow::SIXTYFOURTH_TRIPLET:
              arrowBitmapSrc = gfx.images[Graphics::SixtyfourthTripletArrows];
              break;

            default:
              LOG(DEBUG_BASIC, "fell to default arrow type.  type=" << ar.type << endl)
              arrowBitmapSrc = gfx.images[Graphics::QuarterArrows];
              break;
            }
            sys.vid.ApplySurface(xpos, screenYPos, arrowBitmapSrc, NULL, &gfx.arrowsFrames[arrowsFramesIndex]);
          }            
        }
        else
        {
          // arrow is hidden.  does it have an animation?
          if (ar.animStartTime != -1)
          {
            if (songTime - ar.animStartTime > constants.arrowsHitAnimMs)
            {
              ar.animStartTime = -1;
            }
            else
            {
              int screenYPos = constants.goalOffset;
              
              int arrowsHitIndex = -1;
              int xpos = -1;
              switch (ar.direction)
              {
                // set up xpos and arrow hit index row
                case 0: xpos = p.arrowFieldXOffset + 0 * constants.playerArrowColumnWidth; arrowsHitIndex = 8; break;
                case 1: xpos = p.arrowFieldXOffset + 1 * constants.playerArrowColumnWidth; arrowsHitIndex = 0; break;
                case 2: xpos = p.arrowFieldXOffset + 2 * constants.playerArrowColumnWidth; arrowsHitIndex = 16; break;
                case 3: xpos = p.arrowFieldXOffset + 3 * constants.playerArrowColumnWidth; arrowsHitIndex = 24; break;
                default: break;
              }
              SDL_Surface* animBitmapSrc = NULL;
              if (ar.type == Arrow::HOLD && ar.length != 0)
              {
                animBitmapSrc = gfx.images[Graphics::ComboHit];
                arrowsHitIndex = 0; // only one row of freeze hit anims
              }
              else
              {
                switch (ar.rating)
                {
                  case Arrow::MARVELLOUS: animBitmapSrc = gfx.images[Graphics::MarvellousHit]; break;
                  case Arrow::PERFECT: animBitmapSrc = gfx.images[Graphics::PerfectHit]; break;
                  case Arrow::GREAT: animBitmapSrc = gfx.images[Graphics::GreatHit]; break;
                  case Arrow::GOOD: animBitmapSrc = gfx.images[Graphics::GoodHit]; break;
                  case Arrow::BOO: animBitmapSrc = gfx.images[Graphics::GoodHit]; break;  //TODO: BooHit?
                  default: break;
                }
              }
              
              // move arrow hit index along row to the right frame of animation
              arrowsHitIndex += (int)(7 * ((songTime - ar.animStartTime) / constants.arrowsHitAnimMs) );
              
              if (animBitmapSrc)
              {
                sys.vid.ApplySurface(xpos, screenYPos, animBitmapSrc, NULL, &gfx.arrowsHitFrames[arrowsHitIndex]);
              }
            }
          }
        }
        
        LOG(DEBUG_DETAIL, "done" << endl)
      }
    }
    
    // update base arrow
    for (int i = p.baseArrow; i < p.numArrows; i++)
    {
      if (p.arrows[i].hidden && p.arrows[i].animStartTime == -1)
      {
        p.baseArrow = i;
      }
      else
      {
        break;
      }
    }

    if(p.combo > p.longestCombo)
    {
      p.longestCombo = p.combo;
    }
  }

  if (sound.MusicFinished())
  {
    RunPlayCleanup();
    state = SCORE;
  }

  if (CheckAbort())
  {
    sound.PlaySample(Sound::RecordScratch);
    RunPlayCleanup();
    state = SCORE;
  }

  LOG(DEBUG_DETAIL, "Game::Play() done " << endl)
}

bool Game::CheckAbort()
{
  bool abortHeld = false;
  for (int i = 0; i < numPlayers; i++)
  {
    if (players[i].inputs.buttonHeld[InputChannel::Button5] && players[i].inputs.buttonHeld[InputChannel::Button7])
    {
      abortHeld = true;
      break;
    }
  }
  if (abortHeld)
  {
    if (songAbortStartTime == -1)
    {
      songAbortStartTime = SDL_GetTicks();
    }
    else
    {
      if ((long)SDL_GetTicks() - songAbortStartTime > constants.songAbortDelay)
      {
        songAbortStartTime = -1;
        return true;
      }
    }
  }
  else
  {
    songAbortStartTime = -1;
  }
  return false;
}

void Game::InitialFrame()
{
  LOG(DEBUG_MINOR, "Game::InitialFrame" << endl)

  // add an offset to song start time.  if offset is positive,
  // first frame will calculate a negative current song time, allowing
  // actual song playback to be on sync if it starts a little later
  // than song start function call
  songStartTime = SDL_GetTicks() + constants.songStartOffset;
  
  // viewport offset starts increasingly negative as song offset goes up,
  // so that after offset time it is at 0.  this synchronises rating and drawing
  viewportOffset = - ((constants.songStartOffset) * pixelsPerMsAtCurrentBpm);
  
  // not taking into account partial pixels for initial offset. 
  // fractional pixels are only important over time
  pixelsLeftToScroll = 0.0;
}

void Game::Frame()
{
  // process song time
  long oldSongTime = songTime;
  songTime = SDL_GetTicks() - songStartTime;

  // due to constants.songStartOffset make songTime negative for the first few frames,
  // on the very first frame, oldSongTime is after songTime.  this bugs the viewport by a few pixels.
  if (oldSongTime > songTime)
  {
    oldSongTime = songTime - 1;
  }


  // process partial frames
  long partialFrameTimeBegin = oldSongTime;
  long frameTimeEnd = songTime;
  
  int frameEndBpmChange = currentBpmChange;
  while ( frameEndBpmChange+1 < (int)songs[currentSong].bpmChanges.size() 
    && songs[currentSong].bpmChanges[frameEndBpmChange+1].timestamp <= frameTimeEnd)
  {
    LOG(DEBUG_DETAIL, "inc frameEndBpmChange because next bpm change timestamp " \
      << songs[currentSong].bpmChanges[frameEndBpmChange+1].timestamp << " < end of current frame " \
      << frameTimeEnd << endl)

    frameEndBpmChange++;

    // update play data with the partial scroll between current
    // position and the position of the bpm change
    //TODO: float to long :(  how much imprecision does this bring exactly?  aim == millisecond precision to end of 6 min. song
    long partialFrameTimeEnd = songs[currentSong].bpmChanges[frameEndBpmChange].timestamp;
    
    PartialFrame(partialFrameTimeBegin, partialFrameTimeEnd);
    
    // update variables needed to calculate next partial
    partialFrameTimeBegin = partialFrameTimeEnd;

    LOG(DEBUG_DETAIL, "partial frame updated, new vars partialFrameTimeBegin: " << partialFrameTimeBegin << " frameEndBpmChange: " << frameEndBpmChange << endl)

    // optimise pixels per second scroll calculation
    pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * songs[currentSong].bpmChanges[frameEndBpmChange].bpm;
    LOG(DEBUG_DETAIL, "A new pixelsPerMsAtCurrentBpm:" << pixelsPerMsAtCurrentBpm << endl)
    
    currentBpmChange = frameEndBpmChange;
    
    beatTimeElapsedAtPreviousBPMs += partialFrameTimeEnd - lastBeatTime - beatTimeElapsedAtPreviousBPMs;
    beatFractionAtPreviousBPMs = currentBeatFraction;
  }
  
  // process final partial frame (entire frame in case where no bpm changes
  // occurred during frame)
  PartialFrame(partialFrameTimeBegin, frameTimeEnd);

  LOG(DEBUG_DETAIL, "frame end, currentBpmChange = " << currentBpmChange << endl)

  //TEMP:
  frameTime = songTime - oldSongTime;

  for (int i = 0; i < numPlayers; ++i)
  {
    RateArrows(players[i]);
  }
}


void Game::PartialFrame(long begin, long end)
{
  // process viewport scroll
  float pixelsToScroll = (end - begin) * pixelsPerMsAtCurrentBpm;
  
  // add partial pixels left from last scroll's truncation
  pixelsToScroll += pixelsLeftToScroll;  
  
  long wholePixelsToScroll = (long)pixelsToScroll;
  pixelsLeftToScroll = pixelsToScroll - wholePixelsToScroll;

  viewportOffset += wholePixelsToScroll;

  // within this context, the current bpm does not change.
  // however, time elapsed sinc thee last beat was not necessarily elapsed
  // at the current bpm, so special vars account for the current values
  // as of the last bpm change.
  long timeElapsedAtCurrentBPM = end - lastBeatTime - beatTimeElapsedAtPreviousBPMs;
  currentBeatFraction = partialBeatFraction + (timeElapsedAtCurrentBPM / Song::MsPerBeatFromBPM(songs[currentSong].bpmChanges[currentBpmChange].bpm) + beatFractionAtPreviousBPMs);
  if ((currentBeatFraction + 0.00001) > 1)
  {
    ++currentBeat;
    partialBeatFraction = currentBeatFraction - 1.0;
    currentBeatFraction -= 1.0;
    lastBeatTime = end;
    beatTimeElapsedAtPreviousBPMs = 0;
    beatFractionAtPreviousBPMs = 0.0;
    if (currentBeat >= 4)
    {
      ++currentMeasure;
      currentBeat = 0;
    }
  }
}



void Game::RateArrows(Player& p)
{
  LOG(DEBUG_GUTS, "RateArrows" << endl)
  LOG(DEBUG_GUTS, "f: " << p.firstVisibleArrow << " l: " << p.lastVisibleArrow << endl)
  if (p.firstVisibleArrow == -1)
  {
    return;
  }
  
  for(int a = p.firstVisibleArrow; a <= p.lastVisibleArrow; a++)
  {
    LOG(DEBUG_GUTS, "f: " << p.firstVisibleArrow << " l: " << p.lastVisibleArrow << endl)
    Arrow& ar = p.arrows[a];
    if (ar.rating == Arrow::RATING_NONE && songTime - ar.time > constants.booDelay)
    {
      ar.rating = Arrow::MISS;
      if (ar.type == Arrow::HOLD)
      {
        ar.freezeRating = Arrow::FREEZE_FAILED;
      }
      p.combo=0;
    }
  }

  for(int b=0; b<4; b++)
  {
    if (p.inputs.directionDown[b])
    {
      for(int a = p.firstVisibleArrow; a <= p.lastVisibleArrow; a++)
      {
        Arrow& ar = p.arrows[a];
        
        // when an arrow is reached that is too far ahead, don't bother looping any further
        if (ar.time - songTime > constants.booDelay) break;
        
        if (ar.direction == b && ar.rating == Arrow::RATING_NONE)
        {
          Arrow::Rating newRating = Arrow::RATING_NONE;
          if(labs(ar.time - songTime) <= constants.marvellousDelay)
          {
            newRating = Arrow::MARVELLOUS;
          }
          else if(labs(ar.time - songTime) <= constants.perfectDelay)
          {
            newRating = Arrow::PERFECT;
          }
          else if(labs(ar.time-songTime) <= constants.greatDelay)
          {
            newRating = Arrow::GREAT;
          }
          else if(labs(ar.time-songTime) <= constants.goodDelay)
          {
            newRating = Arrow::GOOD;
          }
          else if(labs(ar.time-songTime) <= constants.booDelay)
          {
            newRating = Arrow::BOO;
          }
          else
          {
            newRating = Arrow::MISS;
          }
          

          // jump rating: all arrows present in the same row are considered
          // part of a jump group.  all jump directions must be active for
          // these to be rated and they count as one arrow with one rating
          
          //TODO: possible improvement, degrade rating from e.g. marvellous
          // if the time between inputs for all elements of the jump is 
          // greater than the marvellous delay.  code as-is counts two steps
          // as a jump if they arrive <= jump delay from each other, but this
          // jump delay can be > marvellous delay...
          int minJumpArrowIndex = a;
          int maxJumpArrowIndex = a;
          while (minJumpArrowIndex-1 >= p.firstVisibleArrow)
          {
            if (p.arrows[minJumpArrowIndex-1].time != ar.time)
            {
              break;
            }
            --minJumpArrowIndex;
          }
          while (maxJumpArrowIndex+1 <= p.lastVisibleArrow)
          {
            if (p.arrows[maxJumpArrowIndex+1].time != ar.time)
            {
              break;
            }
            ++maxJumpArrowIndex;
          }
          if (minJumpArrowIndex != maxJumpArrowIndex)
          {
            // more than one arrow on the same line.  are jump flags
            // for each necessary direction active?
            for (int i = minJumpArrowIndex; i <= maxJumpArrowIndex; i++)
            {
              if (p.directionJumpActive[p.arrows[i].direction] == false)
              //if (p.control_data[p.arrows[i].direction].jump_active == false)
              {
                newRating = Arrow::RATING_NONE;
              }
            }
            // if the rating is still good, apply it to all arrows in the jump
            if (newRating != Arrow::RATING_NONE)
            {
              for (int i = minJumpArrowIndex; i <= maxJumpArrowIndex; i++)
              {
                Arrow& jmpAr = p.arrows[i];
                
                jmpAr.animStartTime = songTime;
                jmpAr.rating = newRating;
                ++p.combo;
                if (jmpAr.length == 0)
                {
                  jmpAr.hidden = true;
                }
              }
            }
          }
          else
          {
            // non-jump arrow case
            if (newRating != Arrow::RATING_NONE)
            {
              ar.animStartTime = songTime;
              ar.rating = newRating;
              ++p.combo;
              if (ar.length == 0)
              {
                ar.hidden = true;
              }
            }
            LOG(DEBUG_DETAIL, "arrow rated at song time " << songTime << " viewP: " << viewportOffset << "  arrow time: " << ar.time << " ypos: " << ar.yPos << " rated to " << (int)newRating << endl)
          }
          
          if (newRating != Arrow::RATING_NONE)
          {
            // create Decal for rating visuals
            Decal d(p.playerNumber - 1, gfx.images[Graphics::Ratings], Decal::Static, songTime, 800);
            switch(newRating)
            {
            case Arrow::MARVELLOUS:
              d.animType = Decal::Loop;
              d.frameDuration = 3;
              d.frameRects.push_back(&gfx.ratingsFrames[0]);
              d.frameRects.push_back(&gfx.ratingsFrames[1]);
              break;

            case Arrow::PERFECT:
              d.frameRects.push_back(&gfx.ratingsFrames[2]);
              break;

            case Arrow::GREAT:
              d.frameRects.push_back(&gfx.ratingsFrames[3]);
              break;

            case Arrow::GOOD:
              d.frameRects.push_back(&gfx.ratingsFrames[4]);
              break;
            
            case Arrow::BOO:
              d.frameRects.push_back(&gfx.ratingsFrames[5]);
              break;
            
            case Arrow::MISS:
              d.frameRects.push_back(&gfx.ratingsFrames[6]);
              break;
              
            default:
              break;
            }
            ratingDecals.push_back(d);
            
            break;
          }
        }
      }
    }
  }

  for(int b=0;b<4;b++)
  {
    if (p.inputs.directionHeld[b])
    {
      for(unsigned int a = p.baseArrow;a<p.arrows.size();a++)
      {
        Arrow& ar = p.arrows[a];
        if (ar.direction == b && ar.type == Arrow::HOLD && ar.length != 0 && ar.rating != Arrow::RATING_NONE && ar.rating != Arrow::MISS && ar.freezeRating == Arrow::FREEZE_NONE)
        {
          if (labs(viewportOffset - ar.yPos) >= ar.length)
          {
            // don't give additional credit for zero-length freeze arrows.
            // the note type hold is used on these to enforce the rule that
            // all notes on the same row as a freeze arrow have freeze graphics
            if (ar.length != 0)
            {
              ar.freezeRating = Arrow::FREEZE_OK;
            }
            ar.hidden = true;
            ar.animStartTime = songTime;
          }
        }
      }
    }
    else
    {
      for(unsigned int a=p.baseArrow;a<p.arrows.size();a++)
      {
        Arrow& ar = p.arrows[a];
        if (ar.direction == b && ar.type == Arrow::HOLD && ar.length != 0 && ar.rating != Arrow::RATING_NONE && ar.rating != Arrow::MISS && ar.freezeRating == Arrow::FREEZE_NONE)
        {
          // don't fail freeze arrow too close to end, it looks funny and
          // this is supposed to be a fun game anyway ;)
          if (ar.length - labs(viewportOffset - ar.yPos) > constants.freezeLengthAllow)
          {
            ar.freezeRating = Arrow::FREEZE_FAILED;
            // when a freeze arrow is failed, it grows a new head at the 
            // point where it was failed for the rest of the darkened
            // fail state drawing
            long oldYPos = ar.yPos;
            ar.yPos += viewportOffset - ar.yPos;
            ar.length -= ar.yPos - oldYPos;
          }
          else
          {
            //TODO: above loop that detects success is filtering on the pad
            // direction being pressed... investigate eliminating this duplication
            if (labs(viewportOffset - ar.yPos) >= ar.length)
            {
              // don't give additional credit for zero-length freeze arrows.
              // the note type hold is used on these to enforce the rule that
              // all notes on the same row as a freeze arrow have freeze graphics
              if (ar.length != 0)
              {
                ar.freezeRating = Arrow::FREEZE_OK;
              }
              ar.hidden = true;
              ar.animStartTime = songTime;
            }
          }
        }
      }
    }
  }  
  
  //TODO: all rating of arrows should be looping by arrow not by direction
  // might be nice to be able to do if (pl.control_active[ar.direction])
}

bool Game::CanInterrupt()
{
  return state != PLAY_PREP1 && state != PLAY_PREP2 && state != PLAY;
}

}
