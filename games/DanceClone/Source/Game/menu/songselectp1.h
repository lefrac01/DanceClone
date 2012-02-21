int songmenuscroll = 0;

void Game_menu_songselectp1()
{
  if(gamestatechange)
  {
#ifdef WIN
    dirlist("Music/*.*");
#endif
#ifdef WII
    dirlist((char*)"/apps/DanceClone/Music/");
#endif
  }

  int a = 0;
  char temptext[MAXPATHLEN];

  for(int b = 0; b < n_direntries; b++)
  {
    if (direntries[b]->folder == 0)
    {
      sprintf(temptext, "%s", direntries[b]->filename);
        
      if(charmatchend(charuppercase((char*)temptext), (char*)".MP3"))
      {

        if(a == 0 && songmenuscroll > 0)
        {
          if(WiiDash_button(rmode->viWidth/2, 55 + a*40, 500, 10, 1, 1, (char*)""))
          {
            songmenuscroll--;
          }
          apply_surface(rmode->viWidth/2 - 15/2 - 1, 55 + a*40 - 16/2 - 1, WDarrowsimage, screen, &WDarrowsframes[2]);
        }
        else if(a - songmenuscroll == 8)
        {
          if(WiiDash_button(rmode->viWidth/2, 55 + (a-songmenuscroll)*40, 500, 10, 1, 1, (char*)""))
          {
            songmenuscroll++;
          }
          apply_surface(rmode->viWidth/2 - 15/2 - 1, 55 + (a-songmenuscroll)*40 - 16/2 - 1, WDarrowsimage, screen, &WDarrowsframes[0]);
          a++;
        }
        else if(a >= songmenuscroll && a-songmenuscroll < 7)
        {
          sprintf(temptext, "%s", direntries[b]->filename);
          sprintf(temptext, "%s", charpartial(temptext, 0, lownumber(strlen(temptext)-4, 50)));
          if(WiiDash_button(rmode->viWidth/2, 55 + (a-songmenuscroll+1)*40, 600, 10, 1, 1, (char*)temptext))
          {
            sprintf(songfilename, "%s", direntries[b]->filename);
            gamestate = 4;
          }
        }
        a++;
      }
    }
  }

  if(WiiDash_button(rmode->viWidth - 100 - 40, rmode->viHeight - 10 - 40, 100, 10, 0, 1, (char*)"Back"))
  {
    gamestate = 0;
  }
}
