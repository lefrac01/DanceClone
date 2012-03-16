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
  numPlayers = 0;
  for (unsigned int i = 0; i < sys.input.inputChannels.size(); ++i)
  {
    Player temp(sys.input.inputChannels[i]);
    temp.Init(i + 1); //initialise as player # starting at 1
    players.push_back(temp);
    ++numPlayers;
  }
  
  selectedSongIndex = -1;
  
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
  sound.Cleanup();
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
  case CHOOSE_NUM_PLAYERS:
    RunChooseNumPlayers();
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
}

Game::GameState Game::State()
{
  return state;
}


/*
    //IDEA 1 - basic
    // specify all position details statically, closest to actual code e.g.
    GUI::Image titleImage(gfx.titleImage);
    titleImage.SetExtents(sys.vid.ScreenWidth()/2-300/2, 30, NULL, NULL); // NULL meaning image width + height
    
    //IDEA 2 - semi OO
    // specify position details in a relative way, referencing gui details by parameters e.g.
    GUI::Image titleImage(gfx.titleImage);
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
    
    gui.SetSpriteTextColour(gfx.sdlBlack);
    
    // FOR NOW, IDEA 1.
    Container title;
    int border = 40;
    
    Image titleImage(gfx.titleImage, sys.vid.ScreenWidth()/2-300/2, 30, 0, 0);
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
        state = CHOOSE_NUM_PLAYERS;
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

    Label cred1("v0.50 Programming and graphics by ThatOtherPerson", 40, 60+20*0, 0, 0);
    Label cred2("thatotherdev.wordpress.com", 40, 60+20*1, 0, 0);

    Label cred3("v0.57 enhancements by Carl Lefrancois", 40, 60+20*3, 0, 0);

    Label cred4("Dance, NOW! music by RekcahDam", 40, 60+20*5, 0, 0);
    Label cred5("rekcahdam.blogspot.com", 40, 60+20*6, 0, 0);
    
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
  const int continueButtonTag = 8;
  static bool ready = false;

  if (gameStateChanged)
  {
    LOG(DEBUG_BASIC, "Game::RunLoadingSong setting up" << endl)
    Container loadingSong;

    Label message("LOADING...", 240, 200+20*5, 0, 0);

    loadingSong.Add(message);
    
    gui.SetScreen(loadingSong);
    gui.SetSpriteTextColour(gfx.sdlWhite);
    c=-2;
    ready = false;
  }
  
  if (c==-2) c=-3;
  else if (c==-3)
  {
    LOG(DEBUG_BASIC, "Game::RunLoadingSong setting up" << endl)
    Container loadingSong;

    Label message("LOADING...  READY!", 240, 200+20*5, 0, 0);

    loadingSong.Add(message);
    
    if(songs[currentSong].Prepare())
    {
      Button back("Continue", sys.vid.ScreenWidth()-200-40, sys.vid.ScreenHeight()-10-40, 100, 10, continueButtonTag);
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
      case continueButtonTag:
        state = PLAY_PREP1;
        break;
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
    LOG(DEBUG_BASIC, "Game::RunDebugScreen setting up" << endl)
    Container debug;

    int y=40;

    Label title("debug", 25, y, 0, 0);
    debug.Add(title);

   
    Image freezeHit(gfx.freezeHitImage, 30, 70, 0, 0);
    debug.Add(freezeHit);
    
    Button back("Back", sys.vid.ScreenWidth()-200-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    debug.Add(back);
    
    gui.SetScreen(debug);

    gui.SetSpriteTextColour(gfx.sdlCyan);
  }


  sys.vid.ApplySurface(80, 260, gfx.freezeArrowsTailImage, NULL, &gfx.freezeTailFrames[3]); //12
  sys.vid.ApplySurface(80, 132, gfx.freezeArrowsBodyImage, NULL, &gfx.freezeBodyFrames[3]); //12
  sys.vid.ApplySurface(80, 100, gfx.freezeArrowsHeadImage, NULL, &gfx.freezeHeadFrames[3]); //16

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

    gui.SetSpriteTextColour(gfx.sdlBlack);
    
    // Detect max input channels and those with an active dance mat
    // add i < 4 to not use the wii balance board
    int tempx = 55;
    for (unsigned int i = 0; i < sys.input.inputChannels.size() && i < 4; ++i)
    {
      char temp[100];
      sprintf(temp, "%d", i + 1);
      Button b(temp, tempx, 300, 80, 60, basePlayerChoiceTag + i);
      b.active = sys.input.inputChannels[i].danceMatActive;
      if (b.active)
      {
        b.colour = gfx.sdlGreen;
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
          state = SELECT_SONG;
          LOG(DEBUG_BASIC, "Game::RunChooseNumPlayers got " << numPlayers << endl)
        }
        break;
      }
    }
  }
}

void Game::RunSelectSong()
{
  const int backButtonTag = 6;
  
  if (gameStateChanged)
  {
    LOG(DEBUG_MINOR, "Game::RunSelectSong setting up" << endl)

    gui.SetSpriteTextColour(gfx.sdlBlack);
    
    // if there are no choices, preload songs.  worst case there are no songs
    // and the preload is done every time the menu is entered...
    if (songMenuItems.size() == 0)
    {
      PreloadSongs();
    }

    Container songSelect1;
    if (songMenuItems.size() > 0)
    {

      int entryNumber = 0;
      string temp;
    
      SimpleSongScroller fileList;

      for(unsigned int i = 0; i < songMenuItems.size(); i++)
      {
        SongMenuItem& smi = songMenuItems[i];
        
        fileList.AddSongChoice(smi.song.Name(), constants.baseFileButtonTag + entryNumber);
        ++entryNumber;
      }
      
      fileList.Recalculate();
      
      songSelect1.Add(fileList);
    }
    else
    {
      Label noFiles1("No files with valid dance-single steps could be read", 40, 40+20*2, 0, 0);
      noFiles1.colour = gfx.sdlWhite;
      char temp[100];
      sprintf(temp, "%d mp3 files were found in the following directory:", (int)songs.size());
      Label noFiles2(temp, 40, 40+20*4, 0, 0);
      noFiles2.colour = gfx.sdlWhite;
      Label noFiles3(constants.musicFileRoot.c_str(), 40, 40+20*5, 0, 0);
      noFiles3.colour = gfx.sdlWhite;

      songSelect1.Add(noFiles1);
      songSelect1.Add(noFiles2);
      songSelect1.Add(noFiles3);
    }
    
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
        if (b.tag >= constants.baseFileButtonTag)
        {
          selectedSongIndex = b.tag - constants.baseFileButtonTag;
          currentSong = songMenuItems[selectedSongIndex].song.filename;
          state = SELECT_DIFFICULTY;
LOG(DEBUG_BASIC, "chose song " << selectedSongIndex << endl)
        }
        break;
      }
    }
  }
}

void Game::RunSelectDifficulty()
{
  const int backButtonTag = 8;
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
    
    Button back("Back", sys.vid.ScreenWidth()-100-40, sys.vid.ScreenHeight()-10-40, 100, 10, backButtonTag);
    selectDifficulty.Add(back);
    
    gui.SetScreen(selectDifficulty);
  }

  // Apply player difficulty selection cursor according to currently selected difficulty
  for (int i = 0; i < numPlayers; ++i)
  {
      sys.vid.ApplySurface(x + w + 20 + i * 50, baseY - 16 + players[i].difficulty * rowHeight, gfx.difficultyCursorImage, NULL, &gfx.difficultyCursorFrames[players[i].ready ? 2+i : i]);
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
        // play feedback wav
      }
      else if (p.inputs.directionDown[InputChannel::DOWN] && p.difficulty < maxDifficulty)
      {
        while (!songs[currentSong].DifficultyIsAvailable(++p.difficulty));
        // play feedback wav
      }
      if (p.inputs.buttonDown[InputChannel::Button4])  // A button
      {
        p.ready = true;
        // play feedback wav
      }
    }
    else if (p.inputs.buttonDown[InputChannel::Button3])  // B button
    {
      p.ready = false;
      // play feedback wav
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
  else
  {
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
          // song has valid steps so also create menu choice
          SongMenuItem temp(songs[en.filename]);
          songMenuItems.push_back(temp);
        }
      }
    }
  }
}

void Game::RunPlayPrep()
{

  if (state == PLAY_PREP1)
  {
    LOG(DEBUG_MINOR, "Game::RunPlayPrep setting up" << endl)
    
    // Initialise music
    sound.PrepMusic(songs[currentSong].Path());
    
    preStartTime = -1;
  
    Container blank;
    gui.SetScreen(blank);
    gui.hideCursor = true;
    state = PLAY_PREP2;
  }  
  
  //TEMP: ripped code from drawing routine to at least show home arrows during pre-play pause
  // other 15 16ths of quarter, draw lit home arrow only if player presses it
  sys.vid.ApplySurface(185, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[6]);
  sys.vid.ApplySurface(250, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[2]);
  sys.vid.ApplySurface(320, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[10]);
  sys.vid.ApplySurface(385, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[14]);
  
  // getready.png 300 x 120
  sys.vid.ApplySurface(gfx.screenWidth/2 - 300/2, gfx.screenHeight/2 - 120/2, gfx.getReadyImage, NULL, NULL);
  
  if (PreStartDelayFinished())
  {
    LOG(DEBUG_MINOR, "preparing for PLAY state" << endl)
    currentBeatTick = -1;
    currentBpmChange = -1;
    //songStartTime = WDgametime+(SDL_GetTicks()-WDruntime);
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
    //TODO: receive players vector and use to augment this:
   
    numBeatTicks = songs[currentSong].beatTicks.size();
    if (numBeatTicks > 0)
    {
      currentBeatTick = 0;
    }
    numBpmChanges = songs[currentSong].bpmChanges.size();
    if (numBpmChanges > 0)
    {
      currentBpmChange = 0;
      pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * songs[currentSong].bpmChanges.front().bpm;
    }
    
    if (songs[currentSong].prepared && currentBeatTick == 0 && currentBpmChange == 0)
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
      sound.FreeMusic();
      state = SELECT_SONG;
      gui.hideCursor = false;
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
  //#//TODO: multiplayer
  //#player_data& pd = current_player_data[0];  
//#
  //#char temptext[100];
  //#sprintf(temptext,"%d%s",p.marvellous," MARVELLOUS");
  //#WiiDash_spritetext(rmode->viWidth/2,440-18*8,temptext,2);
  //#sprintf(temptext,"%d%s",p.perfect," PERFECT");
  //#WiiDash_spritetext(rmode->viWidth/2,440-18*7,temptext,2);
  //#sprintf(temptext,"%d%s",p.great," GREAT");
  //#WiiDash_spritetext(rmode->viWidth/2,440-18*6,temptext,2);
  //#sprintf(temptext,"%d%s",p.good," GOOD");
  //#WiiDash_spritetext(rmode->viWidth/2,440-18*5,temptext,2);
  //#sprintf(temptext,"%d%s",p.boo," BOO");
  //#WiiDash_spritetext(rmode->viWidth/2,440-18*4,temptext,2);
  //#sprintf(temptext,"%d%s",p.longest_combo," MAX COMBO");
  //#WiiDash_spritetext(rmode->viWidth/2,440-18*1,temptext,2);
//#
//#
  //#if(WiiDash_button(rmode->viWidth-300-40,sys.vid.ScreenHeight()-10-40,100,10,0,1,(char*)"temp no score - Back"))gamestate=4;

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
  
  // display ratings
  /*
  if (DEBUG_LEVEL >= DEBUG_DETAIL)
  {
    //#log.open("debug", std::ios_base::app);
    log << "applying ratings surfaces" << endl;
    //#log.close();
  }
  for(int a=0;a<n_ratings;a++){
    sys.vid.ApplySurface(ratings[a]->posx,ratings[a]->posy,ratingsimage,screen,&ratingsframes[ratings[a]->type]);
    ratings[a]->posy=ratings[a]->posy-(int)((double)timehaspast/10);
    if(ratings[a]->posy<0-50){
      delete ratings[a];
      for(int b=a; b<n_ratings; b++)ratings[b]=ratings[b+1];
      n_ratings=n_ratings-1;
      a=a-1;
    }
  }
  */
  
  
  //TODO: multiplayer
  Player& p = players[0];  
  
  // animate home arrows per user input
  float beatFraction = fabs(songs[currentSong].beatTicks[currentBeatTick].beat) - (long)fabs(songs[currentSong].beatTicks[currentBeatTick].beat);
  // if we are before beat 0, correct tick whole calculation 
  if (songs[currentSong].beatTicks[currentBeatTick].beat < 0)
  {
    beatFraction = 0.99-beatFraction;
  }
  int tick_whole = 0;
  while (beatFraction >= 0.25)
  {
    ++tick_whole;
    beatFraction -= 0.25;
  }
  
  if (tick_whole == 0)
  {
    // first 16th of a quarter note, draw flashing home arrows
    sys.vid.ApplySurface(185, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[4]);
    sys.vid.ApplySurface(250, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[0]);
    sys.vid.ApplySurface(320, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[8]);
    sys.vid.ApplySurface(385, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[12]);
  }
  else
  {
    // other 15 16ths of quarter, draw lit home arrow only if player presses it
    sys.vid.ApplySurface(185, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::LEFT]  ? 4  : 6]);
    sys.vid.ApplySurface(250, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::DOWN]  ? 0  : 2]);
    sys.vid.ApplySurface(320, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::UP]    ? 8  : 10]);
    sys.vid.ApplySurface(385, constants.goalOffset, gfx.homeArrowsImage, NULL, &gfx.arrowsFrames[p.inputs.directionDown[InputChannel::RIGHT] ? 12 : 14]);
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

    LOG(DEBUG_DETAIL, "firstVisibleArrow: " << p.firstVisibleArrow << " lastVisibleArrow: = " << p.lastVisibleArrow << endl)

    
    // draw arrows from last to first, updating first if it is offscreen
    if (p.firstVisibleArrow != -1)
    {
      for (int a = p.lastVisibleArrow; a >= p.firstVisibleArrow; a--)
      {
        Arrow& ar = p.arrows[a];
        //NOTE: hold arrow isn't offscreen until tail is!
        if (ar.yPos + ar.length <= offscreenLow)
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
            LOG(DEBUG_DETAIL, "incrementing first visible: " << p.firstVisibleArrow << " because ar.yPos " << ar.yPos << " + ar.length " << ar.length << " <= offscreenLow " << offscreenLow << endl)
            ++p.firstVisibleArrow;
          }
        }
        else
        {
          //arrows become hidden if the player clears them
          if (!ar.hidden)
          {
            int screenYPos = ar.yPos - viewportOffset + constants.goalOffset;
            
            int arrowsFramesIndex = -1;
            int xpos = -1;
            switch (ar.direction)
            {
              case 0: xpos = 185; arrowsFramesIndex =  4+tick_whole; break;
              case 1: xpos = 250; arrowsFramesIndex =  0+tick_whole; break;
              case 2: xpos = 320; arrowsFramesIndex =  8+tick_whole; break;
              case 3: xpos = 385; arrowsFramesIndex = 12+tick_whole; break;
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
                  freeze_src_rect.y = 64 - current_freeze_length;
                  freeze_dest_rect.h = current_freeze_length;
                  freeze_dest_rect.y = current_freeze_bottom - current_freeze_length;
                }

                SDL_BlitSurface(gfx.freezeArrowsTailImage, &freeze_src_rect, sys.vid.screen, &freeze_dest_rect);
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
                  SDL_BlitSurface(gfx.freezeArrowsBodyImage, &freeze_src_rect, sys.vid.screen, &freeze_dest_rect);
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
              
              sys.vid.ApplySurface(xpos, screenYPos, gfx.freezeArrowsHeadImage, NULL, &gfx.freezeHeadFrames[freeze_head_frame_index]);
            }
            else
            {
              SDL_Surface* arrowBitmapSrc = NULL;
              switch(ar.type)
              {
              case Arrow::QUARTER:
                arrowBitmapSrc = gfx.quarterArrowsImage;
                break;
              case Arrow::EIGHTH:
                arrowBitmapSrc = gfx.eighthArrowsImage;
                break;
              case Arrow::QUARTER_TRIPLET:
                arrowBitmapSrc = gfx.quarterTripletArrowsImage;
                break;
              case Arrow::SIXTEENTH:
                arrowBitmapSrc = gfx.sixteenthArrowsImage;
                break;
              case Arrow::EIGHTH_TRIPLET:
                arrowBitmapSrc = gfx.eighthTripletArrowsImage;
                break;
              case Arrow::THIRTYSECOND:
                arrowBitmapSrc = gfx.thirtysecondArrowsImage;
                break;
              case Arrow::SIXTEENTH_TRIPLET:
                arrowBitmapSrc = gfx.sixteenthTripletArrowsImage;
                break;
              case Arrow::SIXTYFOURTH:
                arrowBitmapSrc = gfx.sixtyfourthArrowsImage;
                break;
              //NOTE: same graphics used for 96th and 192nd
              case Arrow::THIRTYSECOND_TRIPLET:
              case Arrow::SIXTYFOURTH_TRIPLET:
                arrowBitmapSrc = gfx.sixtyfourthTripletArrowsImage;
                break;

              default:
                LOG(DEBUG_BASIC, "fell to default arrow type.  type=" << ar.type << endl)
                arrowBitmapSrc = gfx.quarterArrowsImage;
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
                  case 0: xpos = 185; arrowsHitIndex = 8; break;
                  case 1: xpos = 250; arrowsHitIndex = 0; break;
                  case 2: xpos = 320; arrowsHitIndex = 16; break;
                  case 3: xpos = 385; arrowsHitIndex = 24; break;
                  default: break;
                }
                SDL_Surface* animBitmapSrc = NULL;
                if (ar.type == Arrow::HOLD && ar.length != 0)
                {
                  animBitmapSrc = gfx.freezeHitImage;
                  arrowsHitIndex = 0; // only one row of freeze hit anims
                }
                else
                {
                  switch (ar.rating)
                  {
                    case Arrow::MARVELLOUS: animBitmapSrc = gfx.marvellousHitImage; break;
                    case Arrow::PERFECT: animBitmapSrc = gfx.perfectHitImage; break;
                    case Arrow::GREAT: animBitmapSrc = gfx.greatHitImage; break;
                    case Arrow::GOOD: animBitmapSrc = gfx.goodHitImage; break;
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

  if (sound.MusicFinished())
  {
    //TODO: score
    //state = SCORE;
    state = SELECT_SONG;
    gui.hideCursor = false;
  }

  CheckAbort();

  LOG(DEBUG_DETAIL, "Game::Play() done " << endl)
}

void Game::CheckAbort()
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
        sound.StopMusic();
        state = SELECT_SONG;
        gui.hideCursor = false;
      }
    }
  }
  else
  {
    songAbortStartTime = -1;
  }
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

  LOG(DEBUG_DETAIL, "frame oldSongTime: " << oldSongTime << " songTime:" << songTime << endl)

 
  // process partial frames
  long partialFrameTimeBegin = oldSongTime;
  long frameTimeEnd = songTime;
  
  // process beat ticks
  int frameEndBeatTick = currentBeatTick;
  while (
    frameEndBeatTick + 1 < numBeatTicks 
    &&  frameTimeEnd >= songs[currentSong].beatTicks[frameEndBeatTick + 1].timestamp
  )
  {
    frameEndBeatTick++;
  }

  // process bpm changes
  int frameEndBpmChange = currentBpmChange;
  if (frameEndBeatTick != currentBeatTick)
  {
    LOG(DEBUG_DETAIL, "new beat tick(s) before new time: " << songTime << " (curre: " << currentBeatTick << " new: " << frameEndBeatTick << ")" << endl)
    
    // a new beat will be encountered.  check for bpm change
    for (int i = currentBeatTick+1; i <= frameEndBeatTick; i++)
    {
      //NOTE: account for possibility of more than one bpm change per
      // tick, but only use the last bpm change.
      // for robustness because I'm unsure of SM file format specs.
      //NOTE: beat 0 bpm change is not detected.  this is ok, beat 0
      // bpm is assigned at song init so is not actually a change in bpm.
      while (
        frameEndBpmChange + 1 < numBpmChanges 
        &&  songs[currentSong].beatTicks[i].beat 
            >= songs[currentSong].bpmChanges[frameEndBpmChange + 1].beat
      )
      {
        LOG(DEBUG_DETAIL, "inc frameEndBpmChange because " \
            << endl << "songs[currentSong].beatTicks[i].beat " << songs[currentSong].beatTicks[i].beat \
            << " >= songs[currentSong].bpmChanges[frameEndBpmChange+1].beat " << songs[currentSong].bpmChanges[frameEndBpmChange+1].beat \
            << " i=" << i << ", frameEndBpmChange=" << frameEndBpmChange << endl)
        frameEndBpmChange++;
      }
      
      // process bpm change
      if (frameEndBpmChange != currentBpmChange)
      {
        // update play data with the partial scroll between current
        // position and the position of the bpm change
        long partialFrameTimeEnd = songs[currentSong].bpmChanges[frameEndBpmChange].timestamp;
        PartialFrame(partialFrameTimeBegin, partialFrameTimeEnd);
        
        // update variables needed to calculate next partial
        partialFrameTimeBegin = partialFrameTimeEnd;

        LOG(DEBUG_DETAIL, "partial frame updated, new vars partialFrameTimeBegin: " << partialFrameTimeBegin << " currentBpmChange: " << currentBpmChange << endl)

        // optimise pixels per second scroll calculation
        if (currentBpmChange != frameEndBpmChange)
        {
          pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * songs[currentSong].bpmChanges[frameEndBpmChange].bpm;
          LOG(DEBUG_DETAIL, "A new pixelsPerMsAtCurrentBpm:" << pixelsPerMsAtCurrentBpm << endl)
        }
        currentBpmChange = frameEndBpmChange;
      }
    }
  }
  
  // process final partial frame (entire frame in case where no bpm changes
  // occurred during frame)
  if (partialFrameTimeBegin < frameTimeEnd)
  {
    PartialFrame(partialFrameTimeBegin, frameTimeEnd);
  }

  // optimise pixels per second scroll calculation
  if (currentBpmChange != frameEndBpmChange)
  {
    pixelsPerMsAtCurrentBpm = constants.pixelsPerMsAt1Bpm * songs[currentSong].bpmChanges[frameEndBpmChange].bpm;
    LOG(DEBUG_DETAIL, "A new pixelsPerMsAtCurrentBpm:" << pixelsPerMsAtCurrentBpm << endl)
  }
  LOG(DEBUG_DETAIL, "frame end, updating currentBpmChange=" << frameEndBpmChange << " currentBeatTick=" << frameEndBeatTick << endl)

  currentBpmChange = frameEndBpmChange;
  currentBeatTick = frameEndBeatTick;
  
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
}



void Game::RateArrows(Player& p)
{
  LOG(DEBUG_GUTS, "RateArrows" << endl)
  LOG(DEBUG_GUTS, "f: " << p.firstVisibleArrow << " l: " << p.lastVisibleArrow << endl)
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
    //if (p.control_data[b].down)
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
            newRating = Arrow::MISS;
          }
          

          // jump rating: all arrows present in the same row are considered
          // part of a jump group.  all jump directions must be active for
          // these to be rated and they count as one arrow with one rating
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
              break;
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
              break;
            }
          }
        }
      }
    }
  }

  for(int b=0;b<4;b++)
  {
    if (p.inputs.directionHeld[b])
    //if (p.control_data[b].held)
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

}
