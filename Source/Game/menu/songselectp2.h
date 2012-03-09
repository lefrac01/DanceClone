void Game_menu_songselectp2()
{
  string temp_string = current_play_data.current_song.name();
  temp_string = temp_string.substr(0, lownumber(temp_string.size(), 50));
  WiiDash_spritetext(25, 45, (char*)temp_string.c_str(), 1);


  int y=55;

  char buf[100];
  if(current_play_data.current_song.difficulty_is_available(0))
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
  if(current_play_data.current_song.difficulty_is_available(1))
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
  if(current_play_data.current_song.difficulty_is_available(2))
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
  if(current_play_data.current_song.difficulty_is_available(3))
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
  if(current_play_data.current_song.difficulty_is_available(4))
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

  if(    current_play_data.current_song.difficulty_is_available(0)
      || current_play_data.current_song.difficulty_is_available(1)
      || current_play_data.current_song.difficulty_is_available(2)
      || current_play_data.current_song.difficulty_is_available(3)
      || current_play_data.current_song.difficulty_is_available(4)
    )
  {
    y += 40;
  }

  y += 40;
  if(WiiDash_button(rmode->viWidth/2, y, 600, 10, 1, 1, (char*)"create new step chart"))
  {
    gamestate = 5;
  }


  if(WiiDash_button(rmode->viWidth-100-40, rmode->viHeight-10-40, 100, 10, 0, 1, (char*)"Back"))
  {
    gamestate = 3;
  }
}
