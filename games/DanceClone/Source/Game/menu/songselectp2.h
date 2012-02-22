/*
bool diff0available;
bool diff1available;
bool diff2available;
bool diff3available;
bool diff4available;
*/
void Game_menu_songselectp2()
{
  string temp_string = current_song.name();
  temp_string = temp_string.substr(0, lownumber(temp_string.size(), 50));
  WiiDash_spritetext(25, 45, (char*)temp_string.c_str(), 1);

  if(gamestatechange)
  {
    /*
    diff0available=0;
    diff1available=0;
    diff2available=0;
    diff3available=0;
    diff4available=0;
    temp_string = "Music/"; temp_string += song_filename; temp_string += ".dc";
    ifstream indata;
    indata.open(temp_string.c_str());
    while(indata.good())
    {
      getline(indata, temp_string);
      if (temp_string == "<startbeginner>") diff0available = 1;
      if (temp_string == "<starteasy>") diff1available = 1;
      if (temp_string == "<startmedium>") diff2available = 1;
      if (temp_string == "<starthard>") diff3available = 1;
      if (temp_string == "<startchallenge>") diff4available = 1;
    }
    indata.close();
    
    highscore0 = 0;
    highscore1 = 0;
    highscore2 = 0;
    highscore3 = 0;
    highscore4 = 0;
    temp_string = "Music/"; temp_string += song_filename; temp_string += ".score";
    ifstream indata2;
    indata2.open(temp_string.c_str());
    indata2 >> highscore0;
    indata2 >> highscore1;
    indata2 >> highscore2;
    indata2 >> highscore3;
    indata2 >> highscore4;
    indata2.close();
    */
  }

  int y=55;

  char buf[100];
  if(current_song.difficulty_is_available(0))
  {
    y += 40;
    if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 0;
    }
    WiiDash_spritetext(75, y-10, (char*)"play beginner", 1);
    sprintf(buf, "%d%s", highscore0, "%");
    WiiDash_spritetext(rmode->viWidth-75, y-10, (char*)buf, 3);
  }
  if(current_song.difficulty_is_available(1))
  {
    y += 40;
    if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 1;
    }
    WiiDash_spritetext(75, y-10, (char*)"play easy", 1);
    sprintf(buf, "%d%s", highscore1, "%");
    WiiDash_spritetext(rmode->viWidth-75, y-10, (char*)buf, 3);
  }
  if(current_song.difficulty_is_available(2))
  {
    y += 40;
    if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 2;
    }
    WiiDash_spritetext(75, y-10, (char*)"play medium", 1);
    sprintf(buf,"%d%s", highscore2, "%");
    WiiDash_spritetext(rmode->viWidth-75, y-10, (char*)buf, 3);
  }
  if(current_song.difficulty_is_available(3))
  {
    y += 40;
    if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 3;
    }
    WiiDash_spritetext(75, y-10, (char*)"play hard", 1);
    sprintf(buf, "%d%s", highscore3, "%");
    WiiDash_spritetext(rmode->viWidth-75, y-10, (char*)buf, 3);
  }
  if(current_song.difficulty_is_available(4))
  {
    y += 40;
    if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 4;
    }
    WiiDash_spritetext(75, y-10, (char*)"play challenge", 1);
    sprintf(buf, "%d%s", highscore4, "%");
    WiiDash_spritetext(rmode->viWidth-75, y-10, (char*)buf, 3);
  }

  if(    current_song.difficulty_is_available(0)
      || current_song.difficulty_is_available(1)
      || current_song.difficulty_is_available(2)
      || current_song.difficulty_is_available(3)
      || current_song.difficulty_is_available(4)
    )
  {
    y += 40;
  }

  y += 40;
  if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)"create new step chart"))
  {
    gamestate = 5;
  }
  y += 40;
  if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)"import step chart"))
  {
    gamestate = 6;
  }

  if(WiiDash_button(rmode->viWidth-100-40, rmode->viHeight-10-40, 100, 10, 0, 1, (char*)"Back"))
  {
    gamestate = 3;
  }
}
