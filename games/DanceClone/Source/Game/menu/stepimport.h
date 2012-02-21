#include "import/sm.h"
int stepmenuscroll=0;

void Game_menu_stepimport(){
  
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
    sprintf(temptext, "%s", direntries[b]->filename);
    
    if(charmatchend(charuppercase((char*)temptext), (char*)".SM"))
    {
      if(a == 0 && stepmenuscroll > 0)
      {
        if(WiiDash_button(rmode->viWidth/2, 55 + a*40, 500, 10, 1, 1, (char*)""))
        {
          stepmenuscroll--;
        }
        apply_surface(rmode->viWidth/2-15/2-1, 55+a*40-16/2-1, WDarrowsimage, screen, &WDarrowsframes[2]);
      }
      else if(a - stepmenuscroll == 8)
      {
        if(WiiDash_button(rmode->viWidth/2, 55+(a-stepmenuscroll)*40, 500, 10, 1, 1, (char*)""))
        {
          stepmenuscroll++;
        }
        apply_surface(rmode->viWidth/2-15/2-1, 55+(a-stepmenuscroll)*40-16/2-1, WDarrowsimage, screen, &WDarrowsframes[0]);
        a++;
      }
      else if(a >= stepmenuscroll && a - stepmenuscroll < 7)
      {
        sprintf(temptext, "%s", direntries[b]->filename);
        sprintf(temptext, "%s", charpartial(temptext, 0, lownumber(strlen(temptext)-3, 50)));
        if(WiiDash_button(rmode->viWidth/2, 55+(a-stepmenuscroll+1)*40, 600, 10, 1, 1, (char*)temptext))
        {
          Game_menu_stepimport_sm((char*)direntries[b]->filename);
          gamestate=4;
        }
      }
      a++;
    }
  }

  if(WiiDash_button(rmode->viWidth-100-40, rmode->viHeight-10-40, 100, 10, 0, 1, (char*)"Back"))
  {
    gamestate = 4;
  }
}
