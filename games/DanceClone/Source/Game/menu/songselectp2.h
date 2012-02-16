bool diff0available;
bool diff1available;
bool diff2available;
bool diff3available;
bool diff4available;

void Game_menu_songselectp2()
{
	
	char temptext[1000];

	sprintf(temptext, "%s", songfilename);
	sprintf(temptext, "%s", charpartial(temptext, 0, lownumber(strlen(temptext) - 4, 50)));
	WiiDash_spritetext(25, 45, (char*)temptext, 1);

	if(gamestatechange)
  {
		diff0available=0;
		diff1available=0;
		diff2available=0;
		diff3available=0;
		diff4available=0;
		sprintf(temptext, "%s%s%s", "Music/", songfilename, ".dc");
		ifstream indata;
		indata.open(temptext);
		while(indata.good())
    {
			indata >> temptext;
			if(charmatchstart((char*)temptext, (char*)"<startbeginner>")) diff0available = 1;
			if(charmatchstart((char*)temptext, (char*)"<starteasy>")) diff1available = 1;
			if(charmatchstart((char*)temptext, (char*)"<startmedium>")) diff2available = 1;
			if(charmatchstart((char*)temptext, (char*)"<starthard>")) diff3available = 1;
			if(charmatchstart((char*)temptext, (char*)"<startchallenge>")) diff4available = 1;
		}
		indata.close();
		highscore0 = 0;
		highscore1 = 0;
		highscore2 = 0;
		highscore3 = 0;
		highscore4 = 0;
		sprintf(temptext, "%s%s%s", "Music/", songfilename, ".score");
		ifstream indata2;
		indata2.open(temptext);
		indata2 >> highscore0;
		indata2 >> highscore1;
		indata2 >> highscore2;
		indata2 >> highscore3;
		indata2 >> highscore4;
		indata2.close();
	}

	int y=55;

	if(diff0available)
  {
    y += 40;
		if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 0;
    }
		WiiDash_spritetext(75, y-10, (char*)"play beginner", 1);
		sprintf(temptext, "%d%s", highscore0, "%");
		WiiDash_spritetext(640-75, y-10, (char*)temptext, 3);
	}
	if(diff1available)
  {
    y += 40;
		if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 1;
    }
		WiiDash_spritetext(75, y-10, (char*)"play easy", 1);
		sprintf(temptext, "%d%s", highscore1, "%");
		WiiDash_spritetext(640-75, y-10, (char*)temptext, 3);
	}
	if(diff2available)
  {
    y += 40;
		if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 2;
    }
		WiiDash_spritetext(75, y-10, (char*)"play medium", 1);
		sprintf(temptext,"%d%s", highscore2, "%");
		WiiDash_spritetext(640-75, y-10, (char*)temptext, 3);
	}
	if(diff3available)
  {
    y += 40;
		if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 3;
    }
		WiiDash_spritetext(75, y-10, (char*)"play hard", 1);
		sprintf(temptext, "%d%s", highscore3, "%");
		WiiDash_spritetext(640-75, y-10, (char*)temptext, 3);
	}
	if(diff4available)
  {
    y += 40;
		if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)""))
    {
      gamestate = 7;
      difficulty = 4;
    }
		WiiDash_spritetext(75, y-10, (char*)"play challenge", 1);
		sprintf(temptext, "%d%s", highscore4, "%");
		WiiDash_spritetext(640-75, y-10, (char*)temptext, 3);
	}

	if(diff0available || diff1available || diff2available || diff3available || diff4available)
  {
    y += 40;
  }

	y += 40;
  if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)"create new step chart"))
  {
    gamestate = 5;
  }
	y += 40;
  if(WiiDash_button(320, y, 600, 10, 1, 1, (char*)"import step chart"))
  {
    gamestate = 6;
  }

	if(WiiDash_button(640-100-40, 480-10-40, 100, 10, 0, 1, (char*)"Back"))
  {
    gamestate = 3;
  }
}
